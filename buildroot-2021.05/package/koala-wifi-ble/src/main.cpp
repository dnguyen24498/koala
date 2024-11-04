#include <iostream>
#include <memory>
#include <cstdlib>
#include <atomic>
#include <CommonAPI/CommonAPI.hpp>

#include "KoalaWifiBleStubImpl.hpp"

using namespace std;
using namespace v1::com::koala;

int main() {
    CommonAPI::Runtime::setProperty("LogContext", "KoalaWifiBle");
    CommonAPI::Runtime::setProperty("LogApplication", "KoalaWifiBle");
    CommonAPI::Runtime::setProperty("LibraryBase", "KoalaWifiBle");

    std::shared_ptr<CommonAPI::Runtime> runtime = CommonAPI::Runtime::get();
     
    std::shared_ptr<KoalaWifiBleStubImpl> service;
    std::string domain = "local";
    std::string instance = "koala-wifi-ble";
    std::string connection = "koala-wifi-ble-conn";

    service = std::make_shared<KoalaWifiBleStubImpl>();
    bool ret = runtime->registerService(domain, instance, service, connection);

    while (!ret) {
        cout << "Registered service failed, trying again in 100 miliseconds...";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        ret = runtime->registerService(domain, instance, service, connection);
    }

    cout << "Registered successfully" << endl;

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(60));
    }
    return 0;
}