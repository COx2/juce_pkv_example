#include "KeyGenerator.h"
#include "KeyParameters.h"

namespace cox2
{

//==============================================================================
KeyGenerator::KeyGenerator()
{
}

KeyGenerator::~KeyGenerator()
{
}

//==============================================================================
juce::String KeyGenerator::generateKey(const juce::String& seed)
{
    // Build a list of subkeys (bit twiddling params are arbitrary but can never change)
    juce::StringArray subkeys;
    subkeys.add(getSubkeyFromSeed(seed, subkey0_param_a, subkey0_param_b, subkey0_param_c));
    subkeys.add(getSubkeyFromSeed(seed, subkey1_param_a, subkey1_param_b, subkey1_param_c));
    subkeys.add(getSubkeyFromSeed(seed, subkey2_param_a, subkey2_param_b, subkey2_param_c));
    subkeys.add(getSubkeyFromSeed(seed, subkey3_param_a, subkey3_param_b, subkey3_param_c));

    // Build the serial (seed + subkeys)
    const juce::String serial = seed + subkeys.joinIntoString("");

    // Build the key (serial + checksum)
    const juce::String key = serial + getChecksumForSerial(serial);

    return formatKey(key);
}

juce::String KeyGenerator::generateSeed()
{
    juce::Random random;
    return juce::String::toHexString(random.nextInt()).toUpperCase();
}

juce::String KeyGenerator::toFixedHex(int num, int length)
{
    return juce::String::toHexString(num).toUpperCase().paddedLeft('0', length).substring(0, length);
}

//==============================================================================
juce::String KeyGenerator::getSubkeyFromSeed(const juce::String& seed, int paramA, int paramB, int paramC)
{
    int seed_value = seed.getHexValue64();
    
    const int subkey_range_max_param_a = 25;
    const int subkey_range_max_param_b = 3;

    paramA = paramA % subkey_range_max_param_a;
    paramB = paramB % subkey_range_max_param_b;

    int subkey;
    if (paramA % 2 == 0)
    {
        subkey = ((seed_value >> paramA) & 0x000000ff) ^ ((seed_value >> paramB) | paramC) & 0x000000ff;
    }
    else
    {
        subkey = ((seed_value >> paramA) & 0x000000ff) ^ ((seed_value >> paramB) & paramC) & 0x000000ff;
    }

    return toFixedHex(subkey, 2);
}

juce::String KeyGenerator::getChecksumForSerial(const juce::StringRef serial)
{
    // Use magic numbers
    int right = checksum_magic_number_right;
    int left = checksum_magic_number_left;

    for (int str_idx = 0; str_idx < serial.length(); str_idx++)
    {
        right += serial[str_idx];
        if (right > 0x000000ff)
        {
            right -= 0x000000ff;
        }

        left += right;
        if (left > 0x000000ff)
        {
            left -= 0x000000ff;
        }
    }

    return toFixedHex((left << 8) + right, 4);
}

juce::String KeyGenerator::formatKey(const juce::String& key)
{
    juce::StringArray sections;

    const int num_characters_in_section = 4;
    const int num_complete_sections = key.length() / num_characters_in_section;
    const int num_fraction_character = key.length() % num_characters_in_section;
    for (int section_idx = 0; section_idx < num_complete_sections; section_idx++)
    {
        const int head = section_idx * num_characters_in_section;
        const int tail = head + num_characters_in_section;
        sections.add(key.substring(head, tail));
    }

    if (num_fraction_character > 0)
    {
        const int head = num_complete_sections * num_characters_in_section;
        const int tail = head + num_characters_in_section;
        sections.add(key.substring(head, tail));
    }

    return sections.joinIntoString("-");
}
}
