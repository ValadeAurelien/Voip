
qt5_wrap_cpp(serverapp_SRC ${SERVER_INC_DIR}/serverapp.h)

add_executable(main main.cpp serverapp.cpp ${serverapp_SRC})
target_link_libraries(main Qt5::Widgets)
target_link_libraries(main Qt5::Network)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

