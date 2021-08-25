#include <JuceHeader.h>

#include "KeyValidater.h"

int main(int argc, char* argv[])
{
    cox2::KeyValidater keyValidater;

    // Your code goes here!
    juce::ignoreUnused(argc, argv);

    while (true)
    {
        std::cout << "Please input key: " << std::endl;

        std::string input_key;

        std::cin >> input_key;
        
        std::cout << "Key validation result: " << std::boolalpha << keyValidater.isKeyValid(juce::String(input_key)) << std::endl;
    }

    return 0;
}
