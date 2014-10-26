CPSC410
=======

To start the server, nevigate to the Tetraminos directory.
```bash
cd Tetraminos
npm start
```
NodeJS now is running at:
```
http://localhost:3000

Dynamic Analyzer
================

The Dynamic Analyzer is reliant on the user having Dyninst installed on their system. It can be installed here: http://www.dyninst.org/downloads/dyninst-8.x

Once installation is complete, compile (on Ubuntu) using the following command (adjust source files as needed)

g++ -I/usr/local/dyninst/include main.cpp -L/usr/local/dyninst/lib -ldyninstAPI -lcommon -lsymtabAPI -lpatchAPI -lparseAPI -lstackwalk -lpcontrol -linstructionAPI -ldynC_API -ldynDwarf -ldynElf -lsymLite -lsymtabAPI
```
