add_library(command STATIC command.cpp)

target_link_libraries(command PRIVATE builtins)
