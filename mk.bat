@echo off 

IF [%1] EQU [build] (
    cmake -B build/ -S . -G "Visual Studio 17 2022"
    cmake --build build/
) ELSE IF [%1] EQU [run] (
    .\build\Debug\pseudo_engine.exe
) ELSE echo Unknown command %1