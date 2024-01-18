#include "proverapi.hpp"
#include "nlohmann/json.hpp"
#include "logging.hpp"
#include <pistache/http_defs.h>

using namespace Pistache;
using json = nlohmann::json;


void ProverAPI::postProve(const Rest::Request& request, Http::ResponseWriter response) {
    LOG_TRACE("starting prove");
    try {
      json j = fullProver.prove(request.body());
      LOG_DEBUG(j.dump().c_str());
      response.send(Http::Code::Ok, j.dump(), MIME(Application, Json));
    } catch (FullProverError e) {
      response.send(e.code, errorToJson(e).dump(), MIME(Application, Json));
    }
}

json ProverAPI::errorToJson(FullProverError e) {
    json j = { 
        {"status", "error"},
        {"code", e.code },
        {"message", e.message},
        {"details", e.details}
    };

    return j;
}


