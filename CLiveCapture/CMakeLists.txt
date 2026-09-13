# CMakeList.txt : CMake project for CLiveCapture

add_executable(CLiveCapture
    "CLiveCapture.cpp"
    "CLiveCapture.h"
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET CLiveCapture PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(CLiveCapture PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
)