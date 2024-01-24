#include <httplib.h>
#include "fullprover.hpp"


class ProverAPI {
    FullProver &fullProver;
    json errorToJson(FullProverError e);

public:
    ProverAPI(FullProver &_fullProver) : fullProver(_fullProver) {};
    void postProve(const httplib::Request &req, httplib::Response &res);

};
