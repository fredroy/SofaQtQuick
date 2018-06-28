cmake_minimum_required(VERSION 2.8.12)
project(SofaQt3D)

set(SofaQt3D_MAJOR_VERSION 0)
set(SofaQt3D_MINOR_VERSION 1)
set(SofaQt3D_VERSION ${SofaQt3D_MAJOR_VERSION}.${SofaQt3D_MINOR_VERSION})

set(SOURCE_FILES
    SofaQt3D/Qt3DModel.cpp
    initSofaQt3d.cpp
)

set(HEADER_FILES
    SofaQt3D/Qt3DModel.h
    SofaQt3D.h
)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5 COMPONENTS Core Gui Widgets Quick QuickControls2 Qml REQUIRED PATHS "${SOFA-EXTERNAL_QT5_PATH}")
find_package(Qt5 COMPONENTS 3DCore 3DRender 3DExtras REQUIRED PATHS "${SOFA-EXTERNAL_QT5_PATH}")

find_package(SofaBase REQUIRED)

add_library(${PROJECT_NAME} SHARED ${HEADER_FILES}  ${SOURCE_FILES})


if(NOT MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "$<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:${CXX11_FLAG}>")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "-DSOFA_BUILD_SOFAQT3D")

target_link_libraries(${PROJECT_NAME} SofaCore SofaBaseVisual  Qt5::Core Qt5::Gui Qt5::Widgets Qt5::QuickControls2 Qt5::Qml Qt5::Quick Qt5::3DCore Qt5::3DRender Qt5::3DExtras)

target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>")
target_include_directories(${PROJECT_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/./>")

## Install rules for the library; CMake package configurations files
sofa_create_package(${PROJECT_NAME} ${SofaQt3D_VERSION} ${PROJECT_NAME} ${PROJECT_NAME})
