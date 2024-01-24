#include <iostream>
#include <fstream>
#include <gmp.h>
#include <memory>
#include <stdexcept>
#include <nlohmann/json.hpp>

#include <alt_bn128.hpp>
#include "binfile_utils.hpp"
#include "zkey_utils.hpp"
#include "wtns_utils.hpp"
#include "groth16.hpp"
#include "fullprover.hpp"

using json = nlohmann::json;

#define handle_error(msg) \
           do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char **argv)
{
    if (argc != 5) {
        std::cerr << "Invalid number of parameters:\n";
        std::cerr << "Usage: prover <circuit.zkey> <witnessBinaryPath> <proof.json>\n";
        return EXIT_FAILURE;
    }

    mpz_t altBbn128r;

    mpz_init(altBbn128r);
    mpz_set_str(altBbn128r, "21888242871839275222246405745257275088548364400416034343698204186575808495617", 10);

    try {
        std::string zkeyFilename = argv[1];
        std::string witnessBinaryPath = argv[2];
        std::string inputFilename = argv[3];
        std::string proofFilename = argv[4];

        FullProver fullProver(zkeyFilename, witnessBinaryPath);

        std::ifstream inputFile;
        inputFile.open(inputFilename);

        std::stringstream buffer;
        buffer << inputFile.rdbuf();

        json j = fullProver.prove(buffer.str());

        std::ofstream proofFile;
        proofFile.open (proofFilename);
        proofFile << j;
        proofFile.close();


    } catch (std::exception* e) {
        mpz_clear(altBbn128r);
        std::cerr << e->what() << '\n';
        return EXIT_FAILURE;
    } catch (std::exception& e) {
        mpz_clear(altBbn128r);
        std::cerr << e.what() << '\n';
        return EXIT_FAILURE;
    }

    mpz_clear(altBbn128r);
    exit(EXIT_SUCCESS);
}
