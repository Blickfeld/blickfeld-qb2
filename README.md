# Blickfeld Qb2

Client library to communicate securely with Qb2 LiDAR devices of Blickfeld GmbH.

This project provides several source and data files which are required to build a client for Qb2 devices.
In general, a TLS-secured [gRPC](https://grpc.io/) protocol is used as network protocol.
The modern framework allows a simple and efficient integration in customer applications and client
side libraries are available for various languages and platforms.

The "Getting Started" guides can be found at https://docs.blickfeld.com/qb2/.

## Source Code Components

### Data

This folder contains static files which are required to connect to a Qb2.
At the moment, this only contains the public key of the Blickfeld Device Root Authority.
All Qb2 certificate chains are signed by this root certificate.
It is used to authenticate the connections to the devices.

In the helper libraries, if no serial number is given, the SSL authentication will only check if it is connected to a Blickfeld Qb2.
If the serial number is provided, the libraries will also validate during the SSL handshake that the connection is performed to the Blickfeld Qb2 with the given serial number.

### Protocol

This folder contains all the gRPC protocol files thus the API protocol.
gRPC generators are used to generate client library stubs from these files to interact with a Qb2.

Note that this library has a nested gRPC structure with multiple submodules which group the API.
The search method at https://docs.blickfeld.com/qb2 can be used to scan through the API.
Several guides also explain the API usage.

For Python package "blickfeld_qb2" was generated from this protocol and can be used for fast prototyping.

### C++

To simplify the integration in C++ projects, this project brings header files and CMake support to automate the protocol generation.

For CMake examples, please refer to [C++ README](cpp/README.md).
