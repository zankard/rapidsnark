#define CPPHTTPLIB_THREAD_POOL_COUNT 1
#include <httplib.h>
#include <iostream>
#include <fstream>
#include <gmp.h>
#include <memory>
#include <stdexcept>
#include <nlohmann/json.hpp>
#include <pistache/router.h>
#include <pistache/endpoint.h>

#include <alt_bn128.hpp>
#include "binfile_utils.hpp"
#include "zkey_utils.hpp"
#include "wtns_utils.hpp"
#include "groth16.hpp"
#include "fullprover.hpp"


json errorToJson(FullProverError e) {
    json j = { 
        {"status", "error"},
        {"code", e.code },
        {"message", e.message},
        {"details", e.details}
    };

    return j;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cerr << "Invalid number of parameters:\n";
        std::cerr << "Usage: proverServer <port> <path/to/circuit.zkey> <path/to/witness_binary_folder> \n";
        return -1;
    }

    int port = std::stoi(argv[1]); // parse port
    std::string zkeyFileName = argv[2];
    std::string witnessBinaryPath = argv[3];

    FullProver fullProver(zkeyFileName, witnessBinaryPath);


    httplib::Server svr;

    svr.Post("/prove", [&](const httplib::Request& req, httplib::Response& res) {
        try {
        json j = fullProver.prove(req.body);
        res.set_content(j.dump(), "application/json");
        } catch (FullProverError e) {
        res.set_content(errorToJson(e).dump(), "application/json");
        res.status = e.code;
        }
    });

    std::string serverReady("Server ready on port " + std::to_string(port) + "...");

    svr.listen("0.0.0.0", port);

}
