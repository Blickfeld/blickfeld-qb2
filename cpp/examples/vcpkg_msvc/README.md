# Example for Microsoft Visual C++ (MSVC)

To simplify the integration in Microsoft Visual C++ (MSVC) projects, the blickfeld-qb2 library is published to the [vcpkg package registry](https://vcpkg.io/en/package/blickfeld-qb2).
It will automatically compile and install the necessary gRPC dependencies.

This folder contains an usage example based on the official [vcpkg integration guide](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started).
This guide uses CMake and the PowerShell terminal, please refer to the offical guide for other terminals (e.g. CMD) and integrations (e.g. MSBuild).

## Set up the example project

    git clone https://github.com/Blickfeld/blickfeld-qb2.git
    cd cpp/examples/vcpkg_msvc

    
## Set up vcpkg

Clone and bootstrap the vcpkg repository.

    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg; .\bootstrap-vcpkg.bat; cd ..

Configure the VCPKG_ROOT environment variable.

    $env:VCPKG_ROOT = "$PWD\vcpkg"
    $env:PATH = "$env:VCPKG_ROOT;$env:PATH"

## Build the example project

Configure the project with the preset file. It uses the environment variable to locate vcpkg.

    cmake --preset=vcpkg

Build the project.

    cmake --build build

Execute the example.

    ./build/vcpkg_msvc-example

## How to add blickfeld-qb2 to an existing MSVC project

Please follow the official [vcpkg integration guide](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started).

With the following commands the library has to be added to the vcpkg dependencies.

    vcpkg new --application
    vcpkg add port blickfeld-qb2

In CMake, the libary can be then found with the find_package and linked to your library.

    find_package(blickfeld-qb2 REQUIRED)
    target_link_libraries(<your-library-or-executable-target> PUBLIC blickfeld-qb2)

Please note that its crucial to configure the CMAKE_TOOLCHAIN_FILE which can also be done manually without the CMakePresets.json.

    cmake -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake ..