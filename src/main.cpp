#include <iostream>
#include "fullprover.hpp"

int main()
{
    std::cout << "Aleks";
    std::cout << "\n";

    FullProver prover("./testdata/circuit_final.zkey");
    auto ret = prover.prove("./testdata/witness.wtns");

    if (ret.type == SUCCESS)
    {
        std::cout << "OK\n";
    }
    else
    {
        std::cout << "ERROR\n";
    }

    std::cout << ret.raw_json << "\n";

}
