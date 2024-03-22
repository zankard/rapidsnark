#pragma once

#include <cstdlib>
#include <memory>

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

    ~ProverResponse()
    {
        if (raw_json != empty_string) // Was allocated by strdup(),
                                      // and needs to be freed
        {
            // Not a pretty solution, but works
            free(const_cast<char*>(raw_json));
        }
    }
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
