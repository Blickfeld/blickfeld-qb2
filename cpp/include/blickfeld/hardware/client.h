#pragma once

#include <blickfeld/base/grpc_defs.h>
#include <blickfeld/hardware/services/identification.grpc.pb.h>
#include <blickfeld/secure/token_factory.h>
#include <chrono>
#include <grpc++/create_channel.h>
#include <memory>
#include <string>

namespace blickfeld {
namespace hardware {

/**
 * @brief Connect to Qb2 device
 *
 * @param fqdn_or_ip Fully qualified domain name e.g. qb2-xxxxx.blickfeld.com, hostname, or IP address
 * @param serial_number Unique serial number assigned of Qb2 device (corresponding serial number is written on the device label)
 * @param application_key Application key used for user-authentication & access-token injection for this gRPC Channel.
 * @param timeout Timeout value for gRPC connection. Can be used to tune the time limit on the attempt to connect to the device.
 * @return std::shared_ptr<grpc::Channel> gRPC Channel which can be used with generated gRPC stubs
 */
std::shared_ptr<grpc::Channel> connect_to_device(const std::string&                        fqdn_or_ip,
                                                 const std::string&                        serial_number   = "",
                                                 const std::string&                        application_key = "",
                                                 const std::chrono::duration<unsigned int> timeout         = base::GRPC_DEFAULT_CONNECTION_TIMEOUT) {
    if(application_key != "" && serial_number == "")
        throw std::runtime_error("Invalid Parameters: `serial_number` has to be set when `application_key` should be used.");

    auto deadline = std::chrono::system_clock::now() + std::chrono::duration_cast<std::chrono::seconds>(timeout);

    std::string cert_serial_number = serial_number;

    // Fetch serial number
    if(cert_serial_number == "") {
        auto endpoint = fqdn_or_ip + ":" + std::to_string(base::GRPC_DEFAULT_INSECURE_PORT);
        auto channel  = grpc::CreateChannel(fqdn_or_ip + ":" + std::to_string(base::GRPC_DEFAULT_INSECURE_PORT), grpc::InsecureChannelCredentials());
        auto service  = blickfeld::hardware::services::Identification::NewStub(channel);
        grpc::ClientContext context;
        context.set_deadline(deadline);
        blickfeld::hardware::services::IdentificationGetResponse response;
        auto                                                     status = service->Get(&context, google::protobuf::Empty(), &response);

        if(!status.ok())
            throw std::runtime_error("Connection to Qb2 device '" + endpoint +
                                     "' failed (insecure pre-handshake serial number fetch): " + status.error_message());

        cert_serial_number = response.identification().serial_number();
    }

    // create ssl channel credentials for secure TLS communication to TLS server
    auto ssl_credentials_options           = grpc::SslCredentialsOptions();
    ssl_credentials_options.pem_root_certs = base::GRPC_DEFAULT_DEVICE_CA;
    auto ssl_credentials                   = grpc::SslCredentials(ssl_credentials_options);

    // Pass serial number as DNS name to SSL handshake
    // This authenticates that we are connected to a Blickfeld Qb2
    // When the serial number was not fetched but passed as an argument, this also checks that we are connected to the correct Qb2.
    std::transform(cert_serial_number.begin(), cert_serial_number.end(), cert_serial_number.begin(), [](unsigned char c) { return std::tolower(c); });
    grpc::ChannelArguments channel_arguments;
    channel_arguments.SetSslTargetNameOverride(cert_serial_number + ".qb2.blickfeld.com");

    auto channel_credentials = ssl_credentials;

    if(application_key != "") {
        auto token_factory  = grpc::MetadataCredentialsFromPlugin(std::unique_ptr<grpc::MetadataCredentialsPlugin>(
            new blickfeld::secure::TokenFactory(connect_to_device(fqdn_or_ip, cert_serial_number), application_key)));
        channel_credentials = grpc::CompositeChannelCredentials(ssl_credentials, token_factory);
    }

    // create custom gRPC channel secured with TLS credentials
    auto endpoint = fqdn_or_ip + ":" + std::to_string(base::GRPC_DEFAULT_SECURE_PORT);
    auto channel  = grpc::CreateCustomChannel(endpoint, channel_credentials, channel_arguments);

    // Wait for connection
    grpc_connectivity_state state;
    while((state = channel->GetState(true)) != GRPC_CHANNEL_READY && state != GRPC_CHANNEL_TRANSIENT_FAILURE) {
        if(!channel->WaitForStateChange(state, deadline)) {
            throw std::runtime_error("Connection to Qb2 device '" + endpoint + "' failed. Deadline exceeded.");
        }
    }
    if(state != grpc_connectivity_state::GRPC_CHANNEL_READY) {
        throw std::runtime_error("Connection to Qb2 device '" + endpoint + "' failed. Network failure.");
    }

    return channel;
}

} // namespace hardware
} // namespace blickfeld
