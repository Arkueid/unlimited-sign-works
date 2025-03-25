add_library(core STATIC
    ${PROJECT_ROOT}/core/base/Cylinder.hpp
    ${PROJECT_ROOT}/core/base/Cylinder.cpp
    ${PROJECT_ROOT}/core/base/Stiffener.hpp
    ${PROJECT_ROOT}/core/base/Stiffener.cpp
    ${PROJECT_ROOT}/core/base/Cube.hpp
    ${PROJECT_ROOT}/core/base/Cube.cpp
    ${PROJECT_ROOT}/core/base/Mesh.hpp

    ${PROJECT_ROOT}/core/part/pole/Pole.hpp
    ${PROJECT_ROOT}/core/part/pole/Pole168.hpp
    ${PROJECT_ROOT}/core/part/pole/Pole168.cpp

    ${PROJECT_ROOT}/core/core.hpp
)

target_include_directories(core PUBLIC
    ${PROJECT_ROOT}/core
)

target_compile_definitions(core PRIVATE UNICODE _UNICODE)

target_link_libraries(core Qt6::Gui)
