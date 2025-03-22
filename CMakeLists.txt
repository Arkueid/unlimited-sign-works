cmake_minimum_required(VERSION 3.26)
project(unlimited-sign-works)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_PREFIX_PATH "D:/Qt/6.8.2/msvc2022_64/")
set(CMAKE_AUTOMOC ON)   # 自动处理 Q_OBJECT 宏的元对象编译（moc）
set(CMAKE_AUTOUIC ON)   # 自动编译 UI 文件（.ui → .h）
set(CMAKE_AUTORCC ON)   # 自动编译资源文件（.qrc → .cpp）

find_package(Qt6 COMPONENTS Core Widgets Gui OpenGLWidgets REQUIRED)


include(cmake/core.cmake)
include(cmake/app.cmake)