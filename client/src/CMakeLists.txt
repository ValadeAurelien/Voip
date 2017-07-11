
qt5_wrap_cpp(windows_SRC ${CLIENT_INC_DIR}/windows.h)
qt5_wrap_cpp(message_SRC ${CLIENT_INC_DIR}/message.h)
qt5_wrap_cpp(application_SRC ${CLIENT_INC_DIR}/application.h)

add_executable(main main.cpp windows.cpp message.cpp application.cpp ${windows_SRC} ${message_SRC} ${application_SRC})
target_link_libraries(main Qt5::Widgets Qt5::Network)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

