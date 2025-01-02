## Prerequistes

* Python 2.7.2
* cocos2dx 4.0
* cmake
[More to be added as I figure them out... mainly android stuff]

## Setting up

* Clone this repository `git clone https://github.com/UltimadeiGit/Hexii`

* Copy the cocos source files from [path-to-cocos2dx-v4.0/] to [Hexii/cocos2d/]
Note: Do not replace files that are already in Hexii/cocos2d/ as these have been purpose modified for Hexii

* Run cmake to generate project files
```
cd Hexii/proj.win32
cmake -A Win32 ..
```

## Android Studio

* Under Project Structure, Set: 
Android Gradle Plugin Version 7.3.1
Gradle Version 7.4

* Install SDK Version 28 and NDK
Under Tools -> Languages & Frameworks -> Android SDK

### To compile

* Build > Refresh Linked C++ Projects
This refreshes the cmake build targets (the CMakeLists is configured to use recursion to add all source files)

* Build > Make project
Runs gradle build and compiles the project