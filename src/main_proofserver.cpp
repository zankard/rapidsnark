#include <pistache/router.h>
#include <pistache/endpoint.h>
#include "proverapi.hpp"
#include "fullprover.hpp"
#include "logging.hpp"

using namespace Pistache;
using namespace Pistache::Rest;

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Invalid number of parameters:\n";
        std::cerr << "Usage: proverServer <port> <path/to/circuit.zkey> <path/to/witness_binary_folder> \n";
        return -1;
    }

    LOG_INFO("Initializing server...");
    int port = std::stoi(argv[1]); // parse port
    std::string zkeyFileName = argv[2];
    std::string witnessBinaryPath = argv[3];

    FullProver fullProver(zkeyFileName, witnessBinaryPath);
    ProverAPI proverAPI(fullProver);
    Address addr(Ipv4::any(), Port(port));

    auto opts = Http::Endpoint::options().threads(1).maxRequestSize(128000000);
    Http::Endpoint server(addr);
    server.init(opts);
    Router router;
    Routes::Post(router, "/prove", Routes::bind(&ProverAPI::postProve, &proverAPI));
    server.setHandler(router.handler());
    std::string serverReady("Server ready on port " + std::to_string(port) + "...");
    LOG_INFO(serverReady);
    server.serve();
}
