#include "proverapi.hpp"
#include "nlohmann/json.hpp"
#include "logging.hpp"
#include <pistache/http_defs.h>

using namespace Pistache;
using json = nlohmann::json;


void ProverAPI::postProve(const httplib::Request &req, httplib::Response &res) {
    LOG_TRACE("starting prove");
    try {
      json j = fullProver.prove(req.body);
      LOG_DEBUG(j.dump().c_str());
      res.set_content(j.dump(), "application/json");
    } catch (FullProverError e) {
      res.set_content(errorToJson(e).dump(), "application/json");
      res.status = e.code;
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


