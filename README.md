# CuteGit
CuteGit it's based on Qt framework Graphical User Interface with advanced functionality for Git SCM

# Build for windows
For building and run CuteGit on Windows, you need:
* cmake
* libgit2 library from here https://github.com/libgit2/libgit2 or site https://libgit2.github.com

## Instruction:
1. Check correct install cmake(system path for python, ...Qt\5.7\mingw53_32\bin, ...Qt\Tools\mingw530_32\bin)
2. Create new folder in libgit2 library directory folder(example: build)
3. Open "build" folder in command line and input follow commands:
    #cmake .. -G "MinGW Makefiles"
    #cmake --build .
4. Built file libgit2.dll copy to "lib" folder in root directory CuteGit, if the folder doesn't exist, then create a new one.
5. Copy "include" folder from libgit2 library to root directory CuteGit.
6. Build CuteGit.
