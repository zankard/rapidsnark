#ifndef FULLPROVER_H
#define FULLPROVER_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <mutex>
#include "alt_bn128.hpp"
#include "groth16.hpp"
#include "binfile_utils.hpp"
#include "zkey_utils.hpp"
#include <pistache/http_defs.h>

struct FullProverError {
  Pistache::Http::Code code;
  std::string message;
  std::string details;

  FullProverError(Pistache::Http::Code _code, std::string _message, std::string _details) 
    : code(_code), message(_message), details(_details) {}
};

static const FullProverError Invalid_Witness_Input 
  = FullProverError(
        Pistache::Http::Code::Bad_Request,
        "Invalid witness input",
        "Invalid witness input"
      );
static const FullProverError Witness_Generation_Binary_Problem
  = FullProverError(
        Pistache::Http::Code::Internal_Server_Error,
        "Witness generation problem",
        "There was a problem running the witness generation phase binary."
      );
static const FullProverError Witness_Generation_Invalid_Curve
  = FullProverError(
        Pistache::Http::Code::Internal_Server_Error,
        "Witness generation problem",
        "The generated witness file uses a different curve than bn128, which is currently the only supported curve."
      );

class FullProver {
    std::mutex mtx;

    std::string circuit;
    std::string witnessBinaryPath;

    std::unique_ptr<Groth16::Prover<AltBn128::Engine>> prover;
    std::unique_ptr<ZKeyUtils::Header> zkHeader;
    std::unique_ptr<BinFileUtils::BinFile> zKey;

    mpz_t altBbn128r;




public: 
    FullProver(std::string zkeyFileName, std::string _witnessBinaryPath);
    ~FullProver();
    json prove(std::string input);


};

#endif // FULLPROVER_H
