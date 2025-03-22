add_executable(usw 
    Main.cpp
    win/scene/GLScene.cpp
    utils/Camera.cpp
    render/Drawable.cpp

    logo.rc
)

target_include_directories(usw PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(usw PRIVATE "/Zc:__cplusplus" "/permissive-" "/utf-8")

set_target_properties(usw PROPERTIES
    LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
)

target_link_libraries(usw
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::OpenGLWidgets 
    core
)

set_target_properties(usw PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_ROOT}/x64/Debug
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_ROOT}/x64/Release
)

file(COPY ${PROJECT_ROOT}/assets/16x16.ico DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(MARKER $<IF:$<CONFIG:DEBUG>,${PROJECT_ROOT}/x64/Debug/.deployed,${PROJECT_ROOT}/x64/Release/.deployed>)

# 获取 windeployqt6 工具的路径
get_target_property(_qmake_executable Qt6::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(WINDEPLOYQT_EXECUTABLE windeployqt6 HINTS "${_qt_bin_dir}")

add_custom_command(
    TARGET usw
    POST_BUILD
    COMMAND
        if not exist ${MARKER} (${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:usw> && echo done. > ${MARKER}) else (echo deployed)
)