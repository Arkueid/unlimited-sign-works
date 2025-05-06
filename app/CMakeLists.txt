aux_source_directory(road/common ROAD_COMMON_SOURCES)
aux_source_directory(road/scene ROAD_SCENE_SOURCES)
aux_source_directory(road/road ROAD_ROAD_SOURCES)
aux_source_directory(road/geoSolid ROAD_GEO_SOLID_SOURCES)
aux_source_directory(road/terrain ROAD_TERRAIN_SOURCES)
aux_source_directory(road/bridge ROAD_BRIDGE_SOURCES)

add_executable(usw 
    ${ROAD_COMMON_SOURCES}
    ${ROAD_SCENE_SOURCES}
    ${ROAD_ROAD_SOURCES}
    ${ROAD_GEO_SOLID_SOURCES}
    ${ROAD_TERRAIN_SOURCES}
    ${ROAD_BRIDGE_SOURCES}

    Main.cpp
    win/scene/PoleScene.hpp
    win/scene/PoleScene.cpp

    win/scene/BoardScene.cpp
    win/scene/BoardScene.cpp

    win/scene/RoadScene.hpp
    win/scene/RoadScene.cpp

    win/pole/PoleDesigner.ui
    win/pole/PoleDesigner.hpp
    win/pole/PoleDesigner.cpp

    win/pole/AddPanel.ui
    win/pole/AddPanel.hpp
    win/pole/AddPanel.cpp

    win/MainWindow.ui
    win/MainWindow.hpp
    win/MainWindow.cpp

    sqlite3/sqlite3.h
    sqlite3/sqlite3.c
    sqlite3/SQLite.hpp
    sqlite3/SQLite.cpp

    win/list/DrawableListItem.ui
    win/list/DrawableListItem.hpp
    win/list/DrawableListItem.cpp

    win/texture/BoardDesigner.ui
    win/texture/BoardDesigner.hpp
    win/texture/BoardDesigner.cpp

    win/texture/TextureBase.ui
    win/texture/TextureBase.hpp
    win/texture/TextureBase.cpp

    utils/Camera.hpp
    utils/Camera.cpp
    utils/TextureManager.hpp    
    utils/TextureManager.cpp  
    utils/VertexBufferManager.hpp  
    utils/VertexBufferManager.cpp  
    utils/PoleJsonSerializer.hpp
    utils/PoleJsonSerializer.cpp

    template/TemplateCfg.hpp
    template/TemplateCfg.cpp
    
    render/PartDrawable.hpp
    render/PartDrawable.cpp

    render/AxisDrawable.hpp
    render/AxisDrawable.cpp

    render/BoxDrawable.hpp
    render/BoxDrawable.cpp

    render/PoleDrawable.hpp
    render/PoleDrawable.cpp

    win/road/RoadDesigner.ui
    win/road/RoadDesigner.hpp
    win/road/RoadDesigner.cpp

    win/road/PoleAddPanel.ui
    win/road/PoleAddPanel.hpp
    win/road/PoleAddPanel.cpp

    win/road/TextureSetPanel.ui
    win/road/TextureSetPanel.hpp
    win/road/TextureSetPanel.cpp

    logo.rc
)


target_compile_definitions(usw PRIVATE UNICODE _UNICODE)

target_include_directories(usw PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} road)

target_compile_options(usw PRIVATE "/Zc:__cplusplus" "/permissive-" "/utf-8")

set_target_properties(usw PROPERTIES
    LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup"
)

target_link_libraries(usw
    PRIVATE
    Qt6::Core
    Qt6::Widgets
    Qt6::OpenGLWidgets 
    Qt6::Gui
    core
    opengl32.lib
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