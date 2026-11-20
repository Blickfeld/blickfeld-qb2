# C++ CMake Client Library

To simplify the integration in C++ projects, this project brings header files and CMake support to automate the protocol generation.

## Dependencies

* CMake (minimum 3.16)
* Protobuf (minimum 3.12) with Protobuf Compiler, as the protocol has proto3 optional fields
* gRPC (minimum 1.30) with gRPC Compiler Plugin

In modern linux distributions (Ubuntu 22.04, Debian 11 and greater), the debian packages should be used:

```shell
sudo apt update
sudo apt install -y libgrpc++-dev protobuf-compiler-grpc libprotobuf-dev
```

For old distributions, the dependencies have to be compiled manually and installed into the system.

## How to add it to an existing CMake project

One of following approaches are recommended as they are tested.

### FetchContent

This uses the [CMake FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) module to populate the source code and target to your project.

```cmake
include(FetchContent)
FetchContent_Declare(blickfeld-qb2
    GIT_REPOSITORY https://github.com/Blickfeld/blickfeld-qb2.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(blickfeld-qb2)
```

The `blickfeld-qb2` target is then available in the CMake project.

### Install

In this approach, we clone the project, use CMake to configure it, build it and install it globally in the system.

```shell
git clone https://github.com/Blickfeld/blickfeld-qb2.git
mkdir blickfeld-qb2/build
cd blickfeld-qb2/build
cmake ..
make -j
sudo make install
```

The library is now installed in the system and can be found by your CMake project.

```cmake
find_package(blickfeld-qb2 REQUIRED)
```

The `blickfeld-qb2` target is then available in the CMake project.

## How to use

With the `target_link_libraries` statement, the `blickfeld-qb2` needs to be linked to your target executable or library.

```cmake
find_package(blickfeld-qb2 REQUIRED) # or FetchContent

add_executable(example main.cpp)
target_link_libraries(example PUBLIC blickfeld-qb2)
```

In the source code, the library method `connect_to_device` can then be used to connect to a device.
It takes the hostname or IP address, the serial number and an application key of the device.

```cpp
#include <blickfeld/hardware/client.h>

int main() {
    auto channel = blickfeld::hardware::connect_to_device("qb2-xxxxxxx", "serial-number-xxx", "application-key-xxxxxxx");
    ..
    return 0;
}
```

Without a serial number, `connect_to_device` reads it from the device over the unencrypted port 50051,
which a device with disabled insecure network ports does not answer.

The various API services can then be used with the established channel.
Note that the channel can also be re-used.

```cpp
#include <blickfeld/hardware/client.h>
#include <blickfeld/core_processing/services/point_cloud.grpc.pb.h>
#include <iostream>

int main() {
    auto channel = blickfeld::hardware::connect_to_device("qb2-xxxxxxx", "serial-number-xxx", "application-key-xxxxxxx");

    // Fetch one point cloud frame
    auto service = blickfeld::core_processing::services::PointCloud::NewStub(channel);
    grpc::ClientContext context;
    blickfeld::core_processing::services::PointCloudStreamResponse response;
    auto stream = service->Stream(&context, blickfeld::core_processing::services::PointCloudStreamRequest());
    if (!stream->Read(&response)) {
        std::cerr << "Stream failed: " << stream->Finish().error_message() << std::endl;
        return 1;
    }
    std::cout << "Received a frame with the ID " << response.frame().id() << std::endl;
    context.TryCancel();

    ..

    return 0;
}
```

The [examples](examples) show the authentication, the processing of a point cloud frame and the ways to add the library to a project.

Please refer to the [gRPC C++ documentation](https://grpc.io/docs/languages/cpp/quickstart/) and the [Blickfeld documentation](https://docs.blickfeld.com/qb2) (Guides & Protocol subsections).