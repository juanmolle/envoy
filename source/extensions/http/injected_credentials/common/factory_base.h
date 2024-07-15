#pragma once

#include "envoy/server/filter_config.h"

#include "source/extensions/http/injected_credentials/common/credential.h"
#include "source/extensions/http/injected_credentials/common/factory.h"

namespace Envoy {
namespace Extensions {
namespace Http {
namespace InjectedCredentials {
namespace Common {

template <class ConfigProto>
class CredentialInjectorFactoryBase : public NamedCredentialInjectorConfigFactory {
public:
  CredentialInjectorSharedPtr createCredentialInjectorFromProto(
      const Protobuf::Message& proto_config, const std::string& stats_prefix,
      Server::Configuration::ServerFactoryContext& server_context) override {
    return createCredentialInjectorFromProtoTyped(
        MessageUtil::downcastAndValidate<const ConfigProto&>(
            proto_config, server_context.messageValidationVisitor()),
        stats_prefix, server_context);
  }

  ProtobufTypes::MessagePtr createEmptyConfigProto() override {
    return std::make_unique<ConfigProto>();
  }

  std::string name() const override { return name_; }

protected:
  CredentialInjectorFactoryBase(const std::string& name) : name_(name) {}

private:
  virtual CredentialInjectorSharedPtr
  createCredentialInjectorFromProtoTyped(const ConfigProto&, const std::string&,
                                         Server::Configuration::ServerFactoryContext&) PURE;

  const std::string name_;
};

} // namespace Common
} // namespace InjectedCredentials
} // namespace Http
} // namespace Extensions
} // namespace Envoy
