#pragma once

#include <cstdlib>
#include <memory>

#include "groth16.hpp"

#include "logging.hpp"
#include "nlohmann/json.hpp"
#include "wtns_utils.hpp"

#include "alt_bn128.hpp"
#include "binfile_utils.hpp"
#include "groth16.hpp"
#include "zkey_utils.hpp"


class FullProverImpl;

enum ProverResponseType
{
    SUCCESS,
    ERROR
};

enum FullProverState
{
    OK,
    ZKEY_FILE_LOAD_ERROR,
    UNSUPPORTED_ZKEY_CURVE
};

enum ProverError
{
    NONE,
    PROVER_NOT_READY,
    INVALID_INPUT,
    WITNESS_GENERATION_INVALID_CURVE
};

struct ProverResponseMetrics
{
    int prover_time;
};

struct ProverResponse
{
    ProverResponseType    type;
    char const*           raw_json;
    ProverError           error;
    ProverResponseMetrics metrics;

private:
    static char const* const empty_string;

public:
    ProverResponse(ProverError _error);
    ProverResponse(const char* _raw_json, ProverResponseMetrics _metrics);

    ~ProverResponse();
};

class FullProverImpl
{
    // bool unsupported_zkey_curve; never used

    std::string circuit;

    std::unique_ptr<Groth16::Prover<AltBn128::Engine>> prover;
    std::unique_ptr<ZKeyUtils::Header>                 zkHeader;
    std::unique_ptr<BinFileUtils::BinFile>             zKey;

    mpz_t altBbn128r;

public:
    FullProverImpl(const char* _zkeyFileName);
    ~FullProverImpl();
    ProverResponse prove(const char* input) const;
};

class FullProver
{
    std::unique_ptr<FullProverImpl> impl;
    FullProverState                 state;

public:
    FullProver() = delete;
    FullProver(const char* _zkeyFileName);
    // ~FullProver();
    ProverResponse prove(const char* input) const;
};
