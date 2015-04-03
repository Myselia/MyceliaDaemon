#!/bin/sh

g++ -o MyceliaDaemon -std=c++11 \
main.cpp src/MyceliaDaemon.cpp \
src/communication/Atom.cpp src/communication/AtomConverter.cpp src/communication/Transmission.cpp \
src/cpp/Generic.cpp src/cpp/Json.cpp \
-lboost_system -lboost_thread
