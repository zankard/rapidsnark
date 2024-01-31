#ifndef FULLPROVER_H
#define FULLPROVER_H

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <mutex>
#include "alt_bn128.hpp"
#include "groth16.hpp"
#include "binfile_utils.hpp"
#include "zkey_utils.hpp"
#include <httplib.h>


enum ProverResponseType {
  SUCCESS,
  ERROR
};

enum ProverError {
  NONE,
  INVALID_INPUT,
  WITNESS_GENERATION_BINARY_PROBLEM,
  WITNESS_GENERATION_INVALID_CURVE
};

struct ProverResponseMetrics {
  int prover_time;
  int witness_generation_time;

};

struct ProverResponse {
  ProverResponseType type;
  const char *raw_json;
  ProverError error;
  ProverResponseMetrics metrics;

  public:
    ProverResponse(ProverError _error);
    ProverResponse(const char *_raw_json, ProverResponseMetrics _metrics);

};



class FullProver {
    std::mutex mtx;

    std::string circuit;
    std::string witnessBinaryPath;

    std::unique_ptr<Groth16::Prover<AltBn128::Engine>> prover;
    std::unique_ptr<ZKeyUtils::Header> zkHeader;
    std::unique_ptr<BinFileUtils::BinFile> zKey;

    mpz_t altBbn128r;




public: 
    FullProver(const char *_zkeyFileName, const char *_witnessBinaryPath);
    ~FullProver();
    ProverResponse prove(const char *input);


};

#endif // FULLPROVER_H
