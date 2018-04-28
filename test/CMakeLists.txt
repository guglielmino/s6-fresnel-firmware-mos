include_directories (${TEST_SOURCE_DIR}/src)


add_executable (s6test
        test_base.cpp
        libs/frozen.c
        ../src/hardware/io/UARTInterface.hpp
        ../src/hardware/io/IUART.h
        hardware/devices/MCP39F511/frame_cmds_spec.cpp
        hardware/devices/MCP39F511/datatypes_spec.cpp
        hardware/devices/MCP39F511/consts_spec.cpp
        network/topics_spec.cpp
        network/messages_spec.cpp)

add_test (NAME S6UnitTest COMMAND s6test)

