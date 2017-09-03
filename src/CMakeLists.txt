qt5_wrap_cpp(serverapp_SRC ${INC_DIR}/serverapp.h)
qt5_wrap_cpp(application_SRC ${INC_DIR}/application.h)
qt5_wrap_cpp(message_SRC ${INC_DIR}/message.h)
qt5_wrap_cpp(windows_SRC ${INC_DIR}/windows.h)

add_executable(server mainserver.cpp serverapp.cpp identity.cpp ${serverapp_SRC})
add_executable(client mainclient.cpp application.cpp windows.cpp message.cpp identity.cpp ${application_SRC} ${message_SRC} ${windows_SRC})
target_link_libraries(server Qt5::Widgets Qt5::Network)
target_link_libraries(client Qt5::Widgets Qt5::Network)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build)
