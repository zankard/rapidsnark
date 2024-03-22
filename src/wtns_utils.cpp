#include "wtns_utils.hpp"

namespace WtnsUtils
{

Header::Header() {}

Header::~Header() { mpz_clear(prime); }

std::unique_ptr<Header> loadHeader(BinFileUtils::BinFile& bin_file)
{
    auto h = std::make_unique<Header>();
    mpz_init(h->prime);

    bin_file.startReadSection(1);

    h->n8 = bin_file.readU32LE();
    mpz_import(h->prime, h->n8, -1, 1, -1, 0, bin_file.read(h->n8));

    h->nVars = bin_file.readU32LE();

    bin_file.endReadSection();

    return h;
}

} // namespace WtnsUtils
