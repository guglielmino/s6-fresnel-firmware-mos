include_directories (${TEST_SOURCE_DIR}/src)

include_directories(scheduler)

add_executable (s6test test_base.cpp scheduler/schedule_spec.cpp scheduler/scheduler_spec.cpp ../src/hardware/io/UARTInterface.hpp)

add_test (NAME MyTest COMMAND s6test)

