include_directories (${TEST_SOURCE_DIR}/src)

include_directories(scheduler)

add_executable (s6test test_base.cpp
        scheduler/schedule_spec.cpp
        scheduler/scheduler_spec.cpp
        ../src/hardware/io/UARTInterface.hpp
        ../src/interfaces/IUART.h
        hardware/devices/MCP39F511/commands_spec.cpp
        hardware/devices/MCP39F511/datatypes_spec.cpp
        network/topics_spec.cpp)

add_test (NAME MyTest COMMAND s6test)

