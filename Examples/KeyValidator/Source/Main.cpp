#include <JuceHeader.h>

#include "KeyValidator.h"

int main(int argc, char* argv[])
{
    cox2::KeyValidator keyValidator;

    // Your code goes here!
    juce::ignoreUnused(argc, argv);

    while (true)
    {
        std::cout << "Please input key: " << std::endl;

        std::string input_key;

        std::cin >> input_key;
        
        std::cout << "Key validation result: " << std::boolalpha << keyValidator.isKeyValid(juce::String(input_key)) << std::endl;
    }

    return 0;
}
