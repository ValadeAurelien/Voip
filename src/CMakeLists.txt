
qt5_wrap_cpp(mainwindow_SRC ${INC_DIR}/mainwindow.h)
qt5_wrap_cpp(auxwindows_SRC ${INC_DIR}/auxwindows.h)

add_executable(main mainwindow.cpp auxwindows.cpp main.cpp ${mainwindow_SRC} ${auxwindows_SRC})
target_link_libraries(main Qt5::Widgets Qt5::Network)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

