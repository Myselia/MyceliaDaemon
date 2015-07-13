# MyceliaDaemon
Node Management Service

## Dev Dependencies
1. Have a Unix-like OS
2. Run: `# apt-get install g++` or install g++ on your system any other way.
3. make `# apt-get install make`or install make on your system any other way.

## Application Dependencies
- MyseliaCppCommon project in the same directory as the MyceliaDaemon
- c++11
- boost : libboost-dev 1.55.0.2+ (`# apt-get install libboost-dev`)
- boost_system : libboost-system-dev 1.55.0.2+ (`# apt-get install libboost-system-dev`)
- boost_thread : libboost-thread-dev 1.55.0.2+ (`# apt-get install libboost-thread-dev`)
- boost_filesystem : libboost-filesystem-dev 1.55.0.2+ (`# apt-get install libboost-filesystem-dev`)

## How to Compile
### Command line
Run `make`

## How to setup IDE
1. Follow "Dev Dependencies"
2. Install Eclipse CDT
3. Import project in Eclipse.
  1. File > Import > Existing Code as Makefile Project
  2. Click on Browse and select folder that contains this git repo.
  3. Choose Cross GCC as a toolchain.
  4. Click "Next" and you are done :)
