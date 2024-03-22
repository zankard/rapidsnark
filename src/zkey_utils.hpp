#pragma once

#include <gmp.h>
#include <memory>

#include "binfile_utils.hpp"

namespace ZKeyUtils
{

class Header
{

public:
    std::uint32_t n8q;
    mpz_t         qPrime;
    std::uint32_t n8r;
    mpz_t         rPrime;

    std::uint32_t nVars;
    std::uint32_t nPublic;
    std::uint32_t domainSize;
    u_int64_t     nCoefs;

    void* vk_alpha1;
    void* vk_beta1;
    void* vk_beta2;
    void* vk_gamma2;
    void* vk_delta1;
    void* vk_delta2;

    Header() = default;
    ~Header();

    Header(Header const&)            = delete; // no copy constructor
    Header& operator=(Header const&) = delete;
};

std::unique_ptr<Header> loadHeader(BinFileUtils::BinFile& bin_file);

} // namespace ZKeyUtils
