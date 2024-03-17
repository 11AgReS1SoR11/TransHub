cmake_minimum_required(VERSION 3.7)
project(ComponentsCore5alpha VERSION 1.6.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core REQUIRED)

add_library(ComponentsCore5alpha SHARED
    Changelist.txt
    ComponentManager
    componentmanager_p.h
    componentspec.h
    IComponent
    icomponent_p.cpp
    version.h
    componentmanager.cpp
    componentscore_global.h
    componentspec_p.h
    icomponent.cpp
    icomponent_p.h
    componentmanager.h
    componentspec.cpp
    icomponent.h
    Version
    invoker.h
    Invoker
    )

target_link_libraries(ComponentsCore5alpha PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_compile_definitions(ComponentsCore5alpha PRIVATE COMPONENTSCORE5ALPHA_LIBRARY)

set_target_properties(ComponentsCore5alpha
    PROPERTIES VERSION ${PROJECT_VERSION} SOVERSION ${PROJECT_VERSION_MAJOR})
set_target_properties(ComponentsCore5alpha
	PROPERTIES PUBLIC_HEADER "ComponentManager;Version;IComponent;Invoker;componentscore_global.h;icomponent.h;componentmanager.h;componentspec.h;invoker.h")
install(TARGETS ComponentsCore5alpha
    LIBRARY DESTINATION lib64
    PUBLIC_HEADER DESTINATION include/ComponentsCore5alpha)

