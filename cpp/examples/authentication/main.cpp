#include <blickfeld/hardware/client.h>
#include <blickfeld/secure/services/account.grpc.pb.h>

int main() {
    try {
        auto channel = blickfeld::hardware::connect_to_device("qb2-xxxxxxx", "serial-number-xxx", "application-key-xxxxxxx");

        // get authenticated account info
        auto service = blickfeld::secure::services::Account::NewStub(channel);

        grpc::ClientContext                             context;
        blickfeld::secure::services::AccountGetRequest  request;
        blickfeld::secure::services::AccountGetResponse response;

        auto status = service->Get(&context, request, &response);

        if(!status.ok()) {
            std::cerr << "Authentication failed: " << status.error_message() << std::endl;
            return 1;
        }
        std::cout << "Authentication successful: " << response.account().DebugString() << std::endl;
    } catch(std::exception& ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}