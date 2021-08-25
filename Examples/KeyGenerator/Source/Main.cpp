#include <JuceHeader.h>

#include "KeyGenerator.h"

int main(int argc, char* argv[])
{
    cox2::KeyGenerator keyGenerator;

    // Your code goes here!
    juce::ignoreUnused(argc, argv);

    while (true)
    {

        const juce::String seed = keyGenerator.generateSeed();

        const juce::String generated_key = keyGenerator.generateKey(seed);
        
        std::cout << generated_key.toStdString() << std::endl;

        juce::SystemClipboard::copyTextToClipboard(generated_key);

        std::cout << "Key was copied to clipboard." << std::endl;

        std::cin.get();
    }

    return 0;
}
