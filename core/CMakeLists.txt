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

    pole/Pole.hpp
    pole/Pole.cpp
    pole/Pole89.hpp
    pole/Pole89.cpp
    pole/Pole168.hpp
    pole/Pole168.cpp
    part/Part.hpp
    part/Part.cpp

    core.hpp
        ../shit/a.cpp
        ../shit/a.h
)

target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_definitions(core PRIVATE UNICODE _UNICODE)