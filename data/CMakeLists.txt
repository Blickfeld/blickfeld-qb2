include(GNUInstallDirs)

set(BF_TLS_DEVICE_CA_PATH ${CMAKE_CURRENT_SOURCE_DIR}/tls/device_ca_cert.pem CACHE STRING "Path to TLS root CA key")

install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/tls/device_ca_cert.pem
    DESTINATION ${CMAKE_INSTALL_DATADIR}/blickfeld-base/tls
)
