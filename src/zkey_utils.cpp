#include <stdexcept>

#include "zkey_utils.hpp"

namespace ZKeyUtils
{

Header::Header() {}

Header::~Header()
{
    mpz_clear(qPrime);
    mpz_clear(rPrime);
}

std::unique_ptr<Header> loadHeader(BinFileUtils::BinFile& bin_file)
{
    // A memory leak was here
    auto h = std::make_unique<Header>();

    bin_file.startReadSection(1);
    uint32_t protocol = bin_file.readU32LE();
    if (protocol != 1)
    {
        throw std::invalid_argument("zkey file is not groth16");
    }
    bin_file.endReadSection();

    bin_file.startReadSection(2);

    h->n8q = bin_file.readU32LE();
    mpz_init(h->qPrime);
    mpz_import(h->qPrime, h->n8q, -1, 1, -1, 0, bin_file.read(h->n8q));

    h->n8r = bin_file.readU32LE();
    mpz_init(h->rPrime);
    mpz_import(h->rPrime, h->n8r, -1, 1, -1, 0, bin_file.read(h->n8r));

    h->nVars      = bin_file.readU32LE();
    h->nPublic    = bin_file.readU32LE();
    h->domainSize = bin_file.readU32LE();

    h->vk_alpha1 = bin_file.read(h->n8q * 2);
    h->vk_beta1  = bin_file.read(h->n8q * 2);
    h->vk_beta2  = bin_file.read(h->n8q * 4);
    h->vk_gamma2 = bin_file.read(h->n8q * 4);
    h->vk_delta1 = bin_file.read(h->n8q * 2);
    h->vk_delta2 = bin_file.read(h->n8q * 4);
    bin_file.endReadSection();

    h->nCoefs = bin_file.getSectionSize(4) / (12 + h->n8r);

    return h;
}

} // namespace ZKeyUtils
