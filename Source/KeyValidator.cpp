#include "KeyValidator.h"
#include "KeyParameters.h"

namespace cox2
{

//==============================================================================
KeyValidator::KeyValidator()
{
}

KeyValidator::~KeyValidator()
{
}

//==============================================================================
bool KeyValidator::isKeyValid(const juce::String& key)
{
    if (!isKeyFormatValid(key))
    {
        return false;
    }

    const juce::String hyphen_removed = key.replace("-", "");
    const juce::String serial = hyphen_removed.substring(0, 16);
    const juce::String checksum = hyphen_removed.substring(16, 20);

    if (!isSerialChecksumValid(serial, checksum))
    {
        return false;
    }

    const juce::String seed = serial.substring(0, 8);
    if (!isSeedFormatValid(seed))
    {
        return false;
    }

    // Verify 0th subkey
    {
        const juce::String expected = getSubkeyFromSeed(seed, subkey0_param_a, subkey0_param_b, subkey0_param_c);
        const juce::String actual = serial.substring(8, 10);
        if (!actual.equalsIgnoreCase(expected))
        {
            return false;
        }
    }

    // Verify 1th subkey
    {
        const juce::String expected = getSubkeyFromSeed(seed, subkey1_param_a, subkey1_param_b, subkey1_param_c);
        const juce::String actual = serial.substring(10, 12);
        if (!actual.equalsIgnoreCase(expected))
        {
            return false;
        }
    }

    // Verify 2th subkey
    {
        const juce::String expected = getSubkeyFromSeed(seed, subkey2_param_a, subkey2_param_b, subkey2_param_c);
        const juce::String actual = serial.substring(12, 14);
        if (!actual.equalsIgnoreCase(expected))
        {
            return false;
        }
    }

    // Verify 3th subkey
    {
        const juce::String expected = getSubkeyFromSeed(seed, subkey3_param_a, subkey3_param_b, subkey3_param_c);
        const juce::String actual = serial.substring(14, 16);
        if (!actual.equalsIgnoreCase(expected))
        {
            return false;
        }
    }

    return true;
}

juce::String KeyValidator::toFixedHex(int num, int length)
{
    return juce::String::toHexString(num).toUpperCase().paddedLeft('0', length).substring(0, length);
}

//==============================================================================
juce::String KeyValidator::getSubkeyFromSeed(const juce::String& seed, int paramA, int paramB, int paramC)
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

juce::String KeyValidator::getChecksumForSerial(const juce::StringRef serial)
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

bool KeyValidator::isKeyFormatValid(const juce::String& key)
{
    return key.length() == 24 &&
        key.replace("-", "").length() == 20;
}

bool KeyValidator::isSeedFormatValid(const juce::String& seed)
{
    return seed.containsOnly("0123456789ABCDEF");
}

bool KeyValidator::isSerialChecksumValid(const juce::String& serial, const juce::String& checksum)
{
    juce::String actual = getChecksumForSerial(serial);
    return actual.equalsIgnoreCase(checksum);
}

}
