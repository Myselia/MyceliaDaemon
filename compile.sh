#!/bin/sh

g++ -o MyceliaDaemon -std=c++11 \
main.cpp src/MyceliaDaemon.cpp \
src/communication/Atom.cpp src/communication/AtomConverter.cpp src/communication/Transmission.cpp \
../MyseliaCppCommon/src/cpp/Generic.cpp ../MyseliaCppCommon/src/cpp/Json.cpp ../MyseliaCppCommon/src/cpp/IO.cpp \
-lboost_system -lboost_thread
