#pragma once

#include <blickfeld/base/grpc_defs.h>
#include <blickfeld/secure/services/authentication.grpc.pb.h>
#include <blickfeld/secure/services/session.grpc.pb.h>
#include <grpc++/channel.h>
#include <grpc++/security/credentials.h>

namespace blickfeld {
namespace secure {

/**
 * @brief TokenFactory: Authentication helper for accessing Qb2.
 *
 * Regular Use Example:
 *
 * The provided blickfeld-qb2 library method `blickfeld::hardware::connect_to_device(...)` makes use
 * of this class internally and configures it as part of the channel-credentials. In that scenario,
 * the TokenFactory instance ensures that for every request on the created channel a valid token is being used.
 *
 * See https://grpc.io/docs/guides/auth/#extending-grpc-to-support-other-authentication-mechanisms for more details.
 *
 * @param channel shared pointer to an established & TLS-secured gRPC Channel.
 * @param application_key Application key used for user-authentication & access-token injection.
 */
class TokenFactory : public grpc::MetadataCredentialsPlugin {
  public:
    TokenFactory(std::shared_ptr<grpc::Channel> channel, std::string application_key)
        : channel(channel), application_key(application_key), current_token(""){};

    // Inject "token" field in call metadata
    grpc::Status GetMetadata(grpc::string_ref                           service_url,
                             grpc::string_ref                           method_name,
                             const grpc::AuthContext&                   channel_auth_context,
                             std::multimap<grpc::string, grpc::string>* metadata) override {
        create_or_renew_token();
        metadata->insert(std::make_pair("token", current_token));
        return grpc::Status::OK;
    };
    // deconstructor
    virtual ~TokenFactory(){};

  private:
    std::shared_ptr<grpc::Channel> channel;
    std::string                    application_key;
    std::string                    current_token;

    // Validate current token, authenticate to renew token if required
    void create_or_renew_token() {
        // validate current token
        if(current_token != "") {
            grpc::ClientContext context;
            context.AddMetadata("token", current_token);
            blickfeld::secure::services::SessionGetNonceResponse get_nonce_response;

            auto session_service = blickfeld::secure::services::Session::NewStub(channel);
            auto session_status  = session_service->GetNonce(&context, google::protobuf::Empty(), &get_nonce_response);

            if(session_status.ok()) {
                return;
            }

            if(session_status.error_code() != grpc::PERMISSION_DENIED) {
                throw std::runtime_error("Token renewal failed: " + session_status.error_message());
            }

            current_token = "";
        }

        // token was never set or is no longer valid -> authenticate
        grpc::ClientContext                                      auth_context;
        blickfeld::secure::services::AuthenticationLoginRequest  auth_request;
        blickfeld::secure::services::AuthenticationLoginResponse auth_response;

        auth_request.mutable_application()->set_key(application_key);

        auto auth_service = blickfeld::secure::services::Authentication::NewStub(channel);
        auto auth_status  = auth_service->Login(&auth_context, auth_request, &auth_response);

        if(!auth_status.ok()) {
            throw std::runtime_error("Authentication failed: " + auth_status.error_message());
        }
        current_token = auth_response.token();
    }
};

} // namespace secure
} // namespace blickfeld