# tinker
Collection of C Abstract Data Types targeted at Embedded Systems

## Motivation
Why another C library you may ask?

* First, I want a handful of Abstract Data Types and Algortihms that are testable and easily added to embedded systems

* Second, I want new developers to use this as a reference.

## License
GPLv3 - In the future I may provide a commercial license with a Patreon Subscription.

## Building
If you're using Eclipse, point your paths; KEIL, add the files to your project. I'll eventually research how to do this on Arduino

## Testing
On windows, I recommend having VS2019 Community Edition, and Ninja installed

Run the vcvars64.bat (usually located in `C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxillary\Build\vcvars64.bat`)

`cd tests`

`mkdir build`

`cd build`

`cmake -G Ninja ..`

`ninja && ctest . --verbose`