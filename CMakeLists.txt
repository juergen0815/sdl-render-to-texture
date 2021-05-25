cmake_minimum_required(VERSION 3.0.0)
project(sdl-render-to-texture VERSION 0.1.0)

include(CTest)
enable_testing()

file(GLOB _FILES
    "data/*.cpp"
    "src/*.cpp"
)

# These must match the engine settings!
set( Boost_USE_STATIC_RUNTIME ON )
set( Boost_USE_STATIC_LIBS ON )
set( Boost_LIB_PREFIX "")
set( Boost_DEBUG ON )

if(WIN32)
    if(MSVC)
        set(VCPKG_ROOT "D:/Workspaces/PEngIne/vcpkg")
        set(INCLUDE_DIRECTORIES 
            "${VCPKG_ROOT}/installed/x64-windows-static/include"
        )
    else()
        # Placeholder!
        set(VCPKG_ROOT "~/vcpkg")
        set(INCLUDE_DIRECTORIES "${VCPKG_ROOT}/installed/x64-linux/include")
    endif()
elseif(UNIX)
    set(VCPKG_ROOT "~/vcpkg")
    set(INCLUDE_DIRECTORIES "${VCPKG_ROOT}/installed/x64-linux/include")

    target_link_directories(${PROJECT_NAME} PRIVATE 
        "${VCPKG_ROOT}/installed/x64-windows-static/debug/lib"
    )

endif()

include_directories(
        ${INCLUDE_DIRECTORIES} 
        "src" 
        "data"
)

add_executable(${PROJECT_NAME} ${_FILES})

if(WIN32)
    if(MSVC)
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

        target_link_directories(${PROJECT_NAME} PRIVATE 
            "${VCPKG_ROOT}/installed/x64-windows-static/debug/lib"
            "${VCPKG_ROOT}/installed/x64-windows-static/lib"
        )

        target_link_libraries(${PROJECT_NAME} PRIVATE                
            "OpenGL32"
            "glew32"
            "SDL"
            "winmm"
            "dxguid"
            "boost_thread-vc140-mt-gd"
            "boost_system-vc140-mt-gd"
            "boost_filesystem-vc140-mt-gd"          
            "boost_iostreams-vc140-mt-gd"
        )
    else()
        # not setup for mingw64 (yet)
        set(INCLUDE_DIRECTORIES "${VCPKG_ROOT}/installed/x64-windows-static/include")

        target_link_directories(${PROJECT_NAME} PRIVATE 
            "${VCPKG_ROOT}/installed/x64-windows-static/debug/lib"
        )

        target_link_libraries(${PROJECT_NAME} PRIVATE                
            "SDL"
            "boost_thread-vc140-mt-gd"
            "boost_system-vc140-mt-gd"
        )
    endif()
# WIN32
elseif(UNIX)
    set(INCLUDE_DIRECTORIES "${VCPKG_ROOT}/installed/x64-linux/include")

    target_link_directories(${PROJECT_NAME} PRIVATE 
        "${VCPKG_ROOT}/installed/x64-linux/lib"
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE                
        "GL"
        "GLEW"
        "SDL"
        "SDLmain"
        "boost_thread"
        "boost_system"
        "boost_iostreams"
        "boost_filesystem"
  )
endif()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
