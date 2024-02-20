#include "qo_onboard/Randomness.h"

#include <iostream>

using namespace Quantinuum::QuantumOrigin::Onboard;

int main(int, char **)
{
    std::unique_ptr<Randomness> onboard;

    try
    {
        auto config_path = "/path/to/config/config.yml";
        onboard          = std::make_unique<Randomness>("Example", config_path);
    }
    catch (const std::exception &e)
    {
        std::cout << "Failed to initialize with config file. Error : " << e.what() << std::endl;
        return 1;
    }

    try
    {
        size_t randomness_amount = 64;                                         // Request 64 bytes of randomness
        auto randomness          = onboard->get_randomness(randomness_amount); // Generate 64bytes of randomness

        // Do something with randomness

        return 0;
    }
    catch (const std::exception &e)
    {
        std::cout << "Failed to generate randomness with error : " << e.what() << std::endl;
        return 1;
    }
}
