#include "source/extensions/filters/listener/proxy_protocol/proxy_protocol.h"

#include "test/extensions/filters/listener/common/fuzz/listener_filter_fuzzer.h"
#include "test/extensions/filters/listener/proxy_protocol/proxy_protocol_fuzz_test.pb.validate.h"
#include "test/fuzz/fuzz_runner.h"

namespace Envoy {
namespace Extensions {
namespace ListenerFilters {
namespace ProxyProtocol {

DEFINE_PROTO_FUZZER(
    const test::extensions::filters::listener::proxy_protocol::ProxyProtocolTestCase& input) {
  Stats::IsolatedStoreImpl store;
  ConfigSharedPtr cfg;
  try {
    TestUtility::validate(input);
    // Config constructor can throw as it validates proto config.
    cfg = std::make_shared<Config>(*store.rootScope(), input.config());
  } catch (const ProtoValidationException& e) {
    ENVOY_LOG_MISC(debug, "ProtoValidationException: {}", e.what());
    return;
  }

  auto filter = std::make_unique<Filter>(std::move(cfg));

  ListenerFilterWithDataFuzzer fuzzer;
  fuzzer.fuzz(std::move(filter), input.fuzzed());
}

} // namespace ProxyProtocol
} // namespace ListenerFilters
} // namespace Extensions
} // namespace Envoy
