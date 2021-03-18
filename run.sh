#!/bin/bash

g++ -g main.cpp -o out/main lib/glad.o -lglfw -ldl && out/main
exit 0;