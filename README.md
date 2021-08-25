# juce_pkv_example
An example of PKV(Partial Key Verification) implementation that made with JUCE.

## What is PKV?
Partial Key Verification (PKV) is a software licensing key technique refer to: http://www.brandonstaggs.com/2007/07/26/implementing-a-partial-serial-number-verification-system-in-delphi/

## NOTES
When use this library, you should change some parameters to your secret values by compile definitions in `Source/CMakeLists.txt`.
If you don't set your own parameters and use them in your product, your product's security will be vulnerable!

## Requirement
+ CMake 3.15 or later
+ Git

## How to build
+ Get Sources
~~~
$ git clone https://github.com/COx2/juce_pkv_example.git
$ cd juce_pkv_example
$ git submodule update --init --recursive
~~~

+ Generate and Build project by CMake

Shortcut: For Visual Studio 2019 users
~~~
.\Scripts\build_vs2019.bat
~~~

Shortcut: For Xcode users
~~~
./Scripts/build_xcode.command
~~~
