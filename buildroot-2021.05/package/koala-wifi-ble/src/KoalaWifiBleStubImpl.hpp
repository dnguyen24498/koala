#include <iostream>
#include <CommonAPI/CommonAPI.hpp>
#include <v1/com/koala/KoalaWifiBleStubDefault.hpp>

using namespace std;
using namespace v1::com::koala;

class KoalaWifiBleStubImpl : public KoalaWifiBleStubDefault {
public:
    KoalaWifiBleStubImpl();
    virtual ~KoalaWifiBleStubImpl();
    virtual void turn_on_wifi(const std::shared_ptr<CommonAPI::ClientId> _client, turn_on_wifiReply_t _reply) override;
};