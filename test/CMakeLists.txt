include_directories (${TEST_SOURCE_DIR}/src)

include_directories(scheduler)

add_executable (s6test test_base.cpp
        commons.cpp
        ../src/hardware/io/UARTInterface.hpp
        ../src/interfaces/IUART.h
        hardware/devices/MCP39F511/commands_spec.cpp
        hardware/devices/MCP39F511/datatypes_spec.cpp
        hardware/devices/MCP39F511/consts_spec.cpp
        network/topics_spec.cpp
        hardware/sensors/sensorValue_spec.cpp)

add_test (NAME S6UnitTest COMMAND s6test)

