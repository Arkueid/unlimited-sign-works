add_library(core STATIC
    base/Cylinder.hpp
    base/Cylinder.cpp
    base/Stiffener.hpp
    base/Stiffener.cpp
    base/Cube.hpp
    base/Cube.cpp
    base/RoundedCube.cpp
    base/RoundedCube.hpp
    base/RoundedRegTriPrism.hpp
    base/RoundedRegTriPrism.cpp
    base/Mesh.hpp

    part/pole/Pole.hpp
    part/pole/Pole.cpp
    part/pole/Pole89.hpp
    part/pole/Pole89.cpp
    part/pole/Pole168.hpp
    part/pole/Pole168.cpp
    part/Part.hpp
    part/Part.cpp

    core.hpp
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(core PRIVATE UNICODE _UNICODE)