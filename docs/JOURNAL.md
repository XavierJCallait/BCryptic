# Development of BCryptic
Here is a little journal of journey in developing the BCryptic password manager. 

## Notes

### Entry 19/09/2025
Finished the security questions UI page, almost done with the master password entry UI page. Need to encrypt and hash proper information once database entry is completed and tested. Most of my time has been spent understanding how Qt works, and I quite like it so far...

### Entry 20/09/2025
I always forget to add the newly created files in CMakeLists.txt, so I always end up with the same error on compilation. I'm confused for a few seconds, but then I remember. Never forget...

### Entry 21/09/2025
I have the flow between pages down, however, now I get to do the actual work of managing passwords and encrypting everything for security. Who would have thought that the business logic is the hard part?

### Entry 22/09/2025
Creating the vault key and recreating it was a bit more complicated than I was expecting, but it was a cool process to learn. Starting to debug...now for the fun part I guess! ... Actually debugging wasn't too bad, but I'm starting to realize I may need to change the way I handle logic between the different pages. We shall see how it goes...

### Entry 24/09/2025
Been stuck on a little bug where I can encrypt but not decrypt despite having the same salts and information. I guess I just need to spend a bit more time debugging...

### Entry 25/09/2025
The bug was using the wrong constants for encrypt/decryption. I swear I'm smart!

### Entry 26/09/2025
Today we are refactoring the flow between pages to make it more organized and leverage Qt's offerings. It's a good day, so far...

### Entry 28/09/2025
Working on the encryption/decryption of items in the database based on the vaultKey. Pretty straight forward, reusing the same encryption/decryption as the vaultKey. Need to add more database operations, which is easy, but annoying at times. Thinking about using an ORM, but given the limited number of fields in each object, might not be worth it...

## Challenges

### Environment Setup
The development was done on Windows 11. Set up of a C++ environment on Windows with a CMakeList should be easy, but I struggled as I wanted to use the GCC compiler through MingW64 instead of using the MSVC. Since MSVC was the default compiler, I had to isolate the environment. Thus, the fix was to use the MingW64 terminal directly when compiling and building. 