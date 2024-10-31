#include <linux/module.h>
#include <linux/i2c.h>
#include <linux/of.h>
#include <linux/kernel.h>
#include <linux/err.h>
#include <linux/delay.h>


#define I2C_BUS_AVAILABLE   (           2)
#define SLAVE_DEVICE_NAME   (   "st7567s")
#define ST7567S_SLAVE_ADDR  (        0x3F*2)

static struct i2c_adapter *ext_i2c_adapter          = NULL;
static struct i2c_client  *ext_i2c_client_st7567s   = NULL;

static int st7567s_device_probe(struct i2c_client *client, const struct i2c_device_id *id)
{    
    u8 cmd = 0xE2;
    int ret = i2c_master_send(client, &cmd, 1);
    if (ret < 0) {
        printk(KERN_ERR "Failed to turn on display\n");
        return ret;
    }
    printk(KERN_INFO "Display turned on\n");
    return 0;
}

static int st7567s_device_remove(struct i2c_client *client)
{
    u8 cmd = 0xAE;
    int ret = i2c_master_send(client, &cmd, 1);
    if (ret < 0) {
        printk(KERN_ERR "Failed to turn off display\n");
        return ret;
    }
    printk(KERN_INFO "Display turned off\n");
    return 0;
}

static const struct i2c_device_id st7567s_device_id[] = {
    { SLAVE_DEVICE_NAME, 0 },
    {}
};

MODULE_DEVICE_TABLE(i2c, st7567s_device_id);

static struct i2c_driver st7567s_device_driver = {
    .driver = {
        .name = SLAVE_DEVICE_NAME,
        .owner = THIS_MODULE,
    },
    .probe = st7567s_device_probe,
    .remove = st7567s_device_remove,
    .id_table = st7567s_device_id,
};

static struct i2c_board_info st7567s_i2c_board_info = {
    I2C_BOARD_INFO(SLAVE_DEVICE_NAME, ST7567S_SLAVE_ADDR)
};

static int __init st7567s_driver_init(void) {
    int ret;
    ext_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);

    if (!ext_i2c_adapter) {
        printk(KERN_ERR "Failed to get i2c adapter, bus: %d\n", I2C_BUS_AVAILABLE);
        return -ENODEV;
    }
    
    ext_i2c_client_st7567s = i2c_new_client_device(ext_i2c_adapter, &st7567s_i2c_board_info);

    if (!ext_i2c_client_st7567s) {
        printk(KERN_ERR "Failed to add new i2c client device\n");
        i2c_put_adapter(ext_i2c_adapter); // Release adapter reference on failure
        return -ENODEV;
    }
    
    ret = i2c_add_driver(&st7567s_device_driver);
    if (ret < 0) {
        printk(KERN_ERR "Failed to add i2c driver: %d\n", ret);
        i2c_unregister_device(ext_i2c_client_st7567s);
        i2c_put_adapter(ext_i2c_adapter);
    } else {
        printk(KERN_INFO "Driver added: %d\n", ret);
    }
    return ret;
}

static void __exit st7567s_driver_exit(void) {
    i2c_unregister_device(ext_i2c_client_st7567s);
    i2c_del_driver(&st7567s_device_driver);
    printk(KERN_INFO "Driver removed\n");
}

module_init(st7567s_driver_init);
module_exit(st7567s_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("dnguyen24498@gmail.com");
MODULE_DESCRIPTION("A simple I2C device driver for ST7567S");
MODULE_VERSION("1.0");
