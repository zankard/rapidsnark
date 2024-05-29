#include "fullprover.hpp"
#include <iostream>

int main(int argc, char* argv[])
{
    std::cout << "Aleks";
    std::cout << "\n";


    auto zkey_file = argc > 1 ? argv[1] : "./testdata/circuit_final.zkey";

    // return 0;

    // FullProver prover("../main_00004.zkey");
    // FullProver prover("./testdata/circuit_final.zkey");
    FullProver prover(zkey_file);

    while (1)
    {
        auto       ret = prover.prove("./witness.wtns");

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

}
