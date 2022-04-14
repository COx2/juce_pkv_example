#pragma once

#include <juce_core/juce_core.h>

namespace cox2
{

//==============================================================================
class KeyValidator
{
public:
    //==============================================================================
    KeyValidator();
    ~KeyValidator();

    //==============================================================================
    bool isKeyValid(const juce::String& key);

    // Format a number to a fixed-length hexidecimal string
    static juce::String toFixedHex(int num, int length);

private:
    //==============================================================================
    // Derive a subkey from the seed (a, b, c being params for bit twiddling)
    juce::String getSubkeyFromSeed(const juce::String& seed, int paramA, int paramB, int paramC);

    // Get the checksum for a given serial string
    juce::String getChecksumForSerial(const juce::StringRef serial);

    bool isKeyFormatValid(const juce::String& key);

    bool isSeedFormatValid(const juce::String& seed);

    bool isSerialChecksumValid(const juce::String& serial, const juce::String& checksum);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KeyValidator)
};

}

/**
* Python example
function isKeyFormatValid(key) {
  return key.length === 24 && key.replace(/-/g, '').length === 20
}
 
function isSeedFormatValid(seed) {
  return seed.match(/[A-F0-9]{8}/) != null
}
 
function isSerialChecksumValid(serial, checksum) {
  const c = getChecksumForSerial(serial)
 
  return c === checksum
}
 
function isKeyValid(key) {
  if (!isKeyFormatValid(key)) {
    return false
  }
 
  const [, serial, checksum] = key.replace(/-/g, '').match(/(.{16})(.{4})/)
  if (!isSerialChecksumValid(serial, checksum)) {
    return false
  }
 
  const seed = serial.substring(0, 8)
  if (!isSeedFormatValid(seed)) {
    return false
  }
 
  // Verify 0th subkey
  const expected = getSubkeyFromSeed(seed, 24, 3, 200)
  const actual = serial.substring(8, 10)
  if (actual !== expected) {
    return false
  }
 
  return true
}
**/