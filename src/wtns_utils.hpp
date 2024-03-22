#pragma once

#include <cstdint>
#include <gmp.h>

#include "binfile_utils.hpp"

namespace WtnsUtils
{

class Header
{
public:
    std::uint32_t n8;
    mpz_t         prime;

    std::uint32_t nVars;

    Header();
    ~Header();
};

std::unique_ptr<Header> loadHeader(BinFileUtils::BinFile& f);

} // namespace WtnsUtils
