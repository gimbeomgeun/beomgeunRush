cmake_minimum_required(VERSION 3.17)
project(ZPEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

####################### SET UP #######################
set(EXECUTABLE_NAME ZPEngineTest)
set(LIBRARY_NAME ZPLib_x86)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include/)
set(
        SRC_FILES
        src/ZeroWindow.cpp
        src/ZeroLogger.cpp
        src/ZeroWrapper.cpp
        src/ZeroResourcesManager.cpp
        src/ZeroMath.cpp
        src/ZeroInputMgr.cpp
        src/ZeroSceneMgr.cpp
        src/ZeroSystem.cpp
        src/ZeroTimeMgr.cpp
        src/ZeroEntityIDManager.cpp
        src/ZeroEntity.cpp
        src/ZeroComponentArray.cpp
        src/ZeroComponent.cpp
        src/ZeroGameObject.cpp
        src/ZeroScene.cpp
        src/ZeroComponentManager.cpp
        src/Transform.cpp
        src/Sprite2DRenderer.cpp
        src/Camera2D.cpp
        src/RigidBody2D.cpp
        src/ZeroColliderManager.cpp
        src/ZeroRenderManager.cpp
        src/ZeroColliderGroup.cpp
        src/UIRenderer.cpp
        src/Button.cpp
        src/AnimationController.cpp
        src/UITextRenderer.cpp
        src/ZeroSoundDevice.cpp
        src/AudioPlayer.cpp
        src/RectTransform.cpp
        src/ZeroEventManager.cpp
        src/ZeroEventManager.cpp)
set(
        INCLUDE_FILES
        include/stdafx.h
        include/ZeroCore.h
        include/ZeroWindow.h
        include/ZeroLogger.h
        include/ZeroWrapper.h
        include/ZeroResourcesManager.h
        include/ZeroMath.h
        include/ZeroInputMgr.h
        include/ZeroSceneMgr.h
        include/ZeroScene.h
        include/ZeroGameObject.h
        include/ZeroComponent.h
        include/ZeroComponentManager.h
        include/ZeroComponentArray.h
        include/ZeroEntity.h
        include/ZeroEntityIDManager.h
        include/ZeroTimeMgr.h
        include/ZeroDefine.h
        include/ZeroSystem.h
        include/ZeroColliderGroup.h
        include/ZeroColliderManager.h
        include/ZeroRenderManager.h
        include/ZeroSoundDevice.h
        include/ZeroAudioLogger.h
        include/AudioPlayer.h
        include/AnimationController.h
        include/Button.h
        include/UITextRenderer.h
        include/UIRenderer.h
        include/RigidBody2D.h
        include/Camera2D.h
        include/Sprite2DRenderer.h
        include/Transform.h
        include/RectTransform.h
        include/ZeroEventManager.h)

####################### [[ STATIC ZPLib ]] #######################
add_library(
        ${LIBRARY_NAME}
        STATIC
        ${SRC_FILES}
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/Debug)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/Release)
endif()

set_target_properties(${LIBRARY_NAME} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

find_package(GLUT REQUIRED)
target_link_libraries(${LIBRARY_NAME} PRIVATE GLUT::GLUT)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${LIBRARY_NAME} PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${LIBRARY_NAME} PRIVATE glm::glm)

find_package(SndFile CONFIG REQUIRED)
target_link_libraries(${LIBRARY_NAME} PRIVATE SndFile::sndfile)

find_package(GLEW REQUIRED)
target_link_libraries(${LIBRARY_NAME} PRIVATE GLEW::GLEW)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${LIBRARY_NAME} PRIVATE glad::glad)

find_package(OpenAL CONFIG REQUIRED)
target_link_libraries(${LIBRARY_NAME} PRIVATE OpenAL::OpenAL)

find_package(freetype CONFIG REQUIRED)
target_link_libraries(${LIBRARY_NAME} PRIVATE freetype)

if(WIN32)
    target_link_libraries(${LIBRARY_NAME} PRIVATE winmm.lib)
endif()

####################### [[ ZPEngineTest ]] #######################
add_executable(
        ${EXECUTABLE_NAME}
        main.cpp
        ${INCLUDE_FILES}
)

####################### ADD ZPLib #######################
target_link_libraries(${EXECUTABLE_NAME} PRIVATE ${LIBRARY_NAME})

####################### ADD LIBRARIES(VCPKG) #######################
find_package(GLUT REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE GLUT::GLUT)

if(APPLE)
    find_package(OpenGL HINTS "/usr/local/var/vcpkg" REQUIRED)
else()
    find_package(OpenGL REQUIRED)
endif()

include_directories(${OPENGL_INCLUDE_DIR})

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE glfw)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE glm::glm)

find_package(SndFile CONFIG REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE SndFile::sndfile)

find_package(GLEW REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE GLEW::GLEW)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE glad::glad)

find_package(OpenAL CONFIG REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE OpenAL::OpenAL)

find_package(freetype CONFIG REQUIRED)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE freetype)

if(WIN32)
    target_link_libraries(${EXECUTABLE_NAME} PRIVATE winmm.lib)
endif()