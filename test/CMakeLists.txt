include_directories (${TEST_SOURCE_DIR}/src)

include_directories(scheduler)

add_executable (s6test scheduler/schedule_spec.cpp)

add_test (NAME MyTest COMMAND s6test)

