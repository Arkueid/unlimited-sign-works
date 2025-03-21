set(CMAKE_PREFIX_PATH "D:/Qt/6.8.2/msvc2022_64/")
set(CMAKE_AUTOMOC ON)   # 自动处理 Q_OBJECT 宏的元对象编译（moc）
set(CMAKE_AUTOUIC ON)   # 自动编译 UI 文件（.ui → .h）
set(CMAKE_AUTORCC ON)   # 自动编译资源文件（.qrc → .cpp）

find_package(Qt6 COMPONENTS Core Widgets Gui OpenGLWidgets REQUIRED)

add_executable(usw 
    Main.cpp
)

target_compile_options(usw PRIVATE "/Zc:__cplusplus" "/permissive-" "/utf-8")

target_compile_definitions(usw
    PRIVATE UNICODE _UNICODE
)

set_target_properties(usw PROPERTIES
    LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
)

target_link_libraries(usw
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::Gui
    Qt6::OpenGLWidgets 
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