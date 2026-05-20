# wiiuldr
An IDA Pro loader for the Wii U Cafe OS.

## Features
* Compressed section handling
* Creates extern segment for imported functions
* Symbol table loading
* Adds imports and exports

## Building
This loader now builds against the vendored IDA SDK submodule in `third_party/ida-sdk`.

Open [CMakeLists.txt](/C:/Projects/Decompilation/ida_game_elf_loaders/src/wiiu/CMakeLists.txt) in Visual Studio as a CMake project and build the `x64` `Release` configuration.

If you prefer the command line with the Visual Studio toolchain:

```powershell
& "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" -S . -B build -G "Visual Studio 18 2026" -A x64
& "C:\Program Files\Microsoft Visual Studio\18\Community\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe" --build build --config Release
```

## Todo
* Support RPL relocation
