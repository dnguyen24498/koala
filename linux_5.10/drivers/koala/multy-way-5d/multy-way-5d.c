#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/platform_device.h>
#include <linux/input.h>

#define DRIVER_NAME "multy-way-5d"

struct multy_way_5d_data {
    struct input_dev *input_dev;
    int gpio_up;
    int gpio_down;
    int gpio_left;
    int gpio_right;
    int gpio_center;
    int irq_up;
    int irq_down;
    int irq_left;
    int irq_right;
    int irq_center;  
    unsigned long last_interrupt_time;
};

static irqreturn_t multy_way_5d_irq_handler(int irq, void *dev_id)
{
    struct multy_way_5d_data *data = dev_id;
    unsigned long irq_flags = IRQF_TRIGGER_FALLING;

    /* Debouncing logic: only process the interrupt if enough time has passed */
    if (time_after(jiffies, data->last_interrupt_time + msecs_to_jiffies(200))) {
        data->last_interrupt_time = jiffies;

        if (irq == data->irq_up)
        {   
            printk(KERN_INFO "Key up pressed");
            input_report_key(data->input_dev, KEY_UP, 1);
        }
        else if (irq == data->irq_down)
        {
            printk(KERN_INFO "Key down pressed");
            input_report_key(data->input_dev, KEY_DOWN, 1);
        }
        else if (irq == data->irq_left)
        {
            printk(KERN_INFO "Key left pressed");
            input_report_key(data->input_dev, KEY_LEFT, 1);
        }
        else if (irq == data->irq_right)
        {
            printk(KERN_INFO "Key right pressed");
            input_report_key(data->input_dev, KEY_RIGHT, 1);
        }
        else if (irq == data->irq_center)
        {
            printk(KERN_INFO "Key center pressed");
            input_report_key(data->input_dev, KEY_ENTER, 1);
        }
        input_sync(data->input_dev);
    }
    return IRQ_HANDLED;
}

static int multy_way_5d_probe(struct platform_device *pdev)
{
    struct multy_way_5d_data *data;
    int ret;

    data = devm_kzalloc(&pdev->dev, sizeof(*data), GFP_KERNEL);
    if (!data)
        return -ENOMEM;

    data->input_dev = devm_input_allocate_device(&pdev->dev);
    if (!data->input_dev) {
        ret = -ENOMEM;
        goto err_alloc_input;
    }

    data->input_dev->name = "5-Way Navigation Button";
    data->input_dev->id.bustype = BUS_HOST;
    input_set_capability(data->input_dev, EV_KEY, KEY_UP);
    input_set_capability(data->input_dev, EV_KEY, KEY_DOWN);
    input_set_capability(data->input_dev, EV_KEY, KEY_LEFT);
    input_set_capability(data->input_dev, EV_KEY, KEY_RIGHT);
    input_set_capability(data->input_dev, EV_KEY, KEY_ENTER);

    ret = input_register_device(data->input_dev);
    if (ret) {
        dev_err(&pdev->dev, "Failed to register input device\n");
        goto err_register_input;
    }

    data->gpio_up = of_get_named_gpio(pdev->dev.of_node, "up-gpios", 0);
    data->gpio_down = of_get_named_gpio(pdev->dev.of_node, "down-gpios", 0);
    data->gpio_left = of_get_named_gpio(pdev->dev.of_node, "left-gpios", 0);
    data->gpio_right = of_get_named_gpio(pdev->dev.of_node, "right-gpios", 0);
    data->gpio_center = of_get_named_gpio(pdev->dev.of_node, "center-gpios", 0);

    if (!gpio_is_valid(data->gpio_up) || !gpio_is_valid(data->gpio_down) ||
        !gpio_is_valid(data->gpio_left) || !gpio_is_valid(data->gpio_right) ||
        !gpio_is_valid(data->gpio_center)) {
            dev_err(&pdev->dev, "Invalid GPIOs for %s\n", DRIVER_NAME);
            ret = -EINVAL;
            goto err_invalid_gpio;
    }

    /* Configure GPIOs as input */
    gpio_direction_input(data->gpio_up);
    gpio_direction_input(data->gpio_down);
    gpio_direction_input(data->gpio_left);
    gpio_direction_input(data->gpio_right);
    gpio_direction_input(data->gpio_center);

    data->irq_up = gpio_to_irq(data->gpio_up);
    data->irq_down = gpio_to_irq(data->gpio_down);
    data->irq_left = gpio_to_irq(data->gpio_left);
    data->irq_right = gpio_to_irq(data->gpio_right);
    data->irq_center = gpio_to_irq(data->gpio_center);

    /* Request IRQs for each GPIO */
    ret = devm_request_irq(&pdev->dev, data->irq_up, multy_way_5d_irq_handler, 
                           IRQF_TRIGGER_FALLING, "multy_way_5d_up", data);
    if (ret) return ret;

    ret = devm_request_irq(&pdev->dev, data->irq_down, multy_way_5d_irq_handler, 
                           IRQF_TRIGGER_FALLING, "multy_way_5d_down", data);
    if (ret) return ret;

    ret = devm_request_irq(&pdev->dev, data->irq_left, multy_way_5d_irq_handler, 
                           IRQF_TRIGGER_FALLING, "multy_way_5d_left", data);
    if (ret) return ret;

    ret = devm_request_irq(&pdev->dev, data->irq_right, multy_way_5d_irq_handler, 
                           IRQF_TRIGGER_FALLING, "multy_way_5d_right", data);
    if (ret) return ret;

    ret = devm_request_irq(&pdev->dev, data->irq_center, multy_way_5d_irq_handler, 
                           IRQF_TRIGGER_FALLING, "multy_way_5d_center", data);
    if (ret) return ret;

    platform_set_drvdata(pdev, data);
    dev_info(&pdev->dev, "5-Way Navigation Button driver loaded");

    return 0;

err_invalid_gpio:
    input_unregister_device(data->input_dev);
err_register_input:
err_alloc_input:
    return ret;
}

static int multy_way_5d_remove(struct platform_device *pdev)
{
    struct multy_way_5d_data *data = platform_get_drvdata(pdev);
    input_unregister_device(data->input_dev);
    dev_info(&pdev->dev, "5-Way Navigation Button driver removed");
    return 0;
}

static const struct of_device_id multy_way_5d_of_match[] = {
    { .compatible = "koala,multy-way-5d" },
    {}
};

MODULE_DEVICE_TABLE(of, multy_way_5d_of_match);

static struct platform_driver multy_way_5d_driver = {
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = multy_way_5d_of_match,
    },
    .probe = multy_way_5d_probe,
    .remove = multy_way_5d_remove,
};

module_platform_driver(multy_way_5d_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("dnguyen24498@gmail.com");
MODULE_DESCRIPTION("5-Way Navigation Button Driver with Debounce");
MODULE_VERSION("1.2");
