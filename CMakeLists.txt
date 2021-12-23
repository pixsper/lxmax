cmake_minimum_required(VERSION 3.19)
project("lxmax")

if (APPLE)
    if (${CMAKE_GENERATOR} MATCHES "Xcode")
        if (${XCODE_VERSION} VERSION_LESS 10)
            message(STATUS "Xcode 10 or higher is required. Please install from the Mac App Store.")
            return ()
        elseif(${XCODE_VERSION} VERSION_GREATER_EQUAL 12)
            set(C74_BUILD_FAT YES)
        endif ()
    elseif (${CMAKE_GENERATOR} MATCHES "Unix Makefiles")
        # This is for the benefit of CLion which identifies as Unix Makefiles
        set(C74_BUILD_FAT YES)
    endif ()

    if (NOT CMAKE_OSX_ARCHITECTURES)
        if(C74_BUILD_FAT)
            set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "macOS architecture" FORCE)
        else()
            set(CMAKE_OSX_ARCHITECTURES ${CMAKE_SYSTEM_PROCESSOR} CACHE STRING "macOS architecture" FORCE)
        endif()
        message("CMAKE_OSX_ARCHITECTURES set to ${CMAKE_OSX_ARCHITECTURES}")
    endif()
endif()

add_definitions("-std=c++17")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/0.17.0/conan.cmake"
                "${CMAKE_BINARY_DIR}/conan.cmake"
                EXPECTED_HASH SHA256=3bef79da16c2e031dc429e1dac87a08b9226418b300ce004cc125a82687baeef
                TLS_VERIFY ON)
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_configure(REQUIRES asio/1.21.0
                      GENERATORS cmake_find_package)

conan_cmake_autodetect(settings)

conan_cmake_install(PATH_OR_REFERENCE .
                    BUILD missing
                    REMOTE conancenter
                    SETTINGS ${settings})
					
find_package(asio)


add_subdirectory(source)
