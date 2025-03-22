add_library(core STATIC
    ${PROJECT_ROOT}/core/base/Cylinder.cpp
    ${PROJECT_ROOT}/core/base/Stiffener.cpp
    ${PROJECT_ROOT}/core/base/Cube.cpp

    ${PROJECT_ROOT}/core/part/pole/Pole168.cpp

    ${PROJECT_ROOT}/core/core.hpp
)

target_include_directories(core PUBLIC
    ${PROJECT_ROOT}/core
)

target_compile_definitions(core PRIVATE UNICODE _UNICODE)

target_link_libraries(core Qt6::Gui)
