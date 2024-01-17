#include <pistache/router.h>
#include <pistache/endpoint.h>
#include "fullprover.hpp"

using namespace Pistache;

class ProverAPI {
    FullProver &fullProver;
    json errorToJson(FullProverError e);

public:
    ProverAPI(FullProver &_fullProver) : fullProver(_fullProver) {};
    void postProve(const Rest::Request& request, Http::ResponseWriter response);

};
