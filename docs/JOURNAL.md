# Development of BCryptic
Here is a little journal of journey in developing the BCryptic password manager. 

## Notes

### Entry 19/09/2025
Finished the security questions UI page, almost done with the master password entry UI page. Need to encrypt and hash proper information once database entry is completed and tested. Most of my time has been spent understanding how Qt works, and I quite like it so far...

### Entry 20/09/2025
I always forget to add the newly created files in CMakeLists.txt, so I always end up with the same error on compilation. I'm confused for a few seconds, but then I remember. Never forget...

## Challenges

### Environment Setup
The development was done on Windows 11. Set up of a C++ environment on Windows with a CMakeList should be easy, but I struggled as I wanted to use the GCC compiler through MingW64 instead of using the MSVC. Since MSVC was the default compiler, I had to isolate the environment. Thus, the fix was to use the MingW64 terminal directly when compiling and building. 