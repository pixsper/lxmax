set(C74_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/../../../support")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${C74_LIBRARY_OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${C74_LIBRARY_OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${C74_LIBRARY_OUTPUT_DIRECTORY}")

include(../../max-sdk-base/script/max-pretarget.cmake)

add_library(${PROJECT_NAME} SHARED)

target_include_directories(${PROJECT_NAME} PUBLIC
		"${MAX_SDK_INCLUDES}"
		"${MAX_SDK_MSP_INCLUDES}"
		"${MAX_SDK_JIT_INCLUDES}"
		"${LXMAX_COMMON_INCLUDES}"
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_include_directories(${PROJECT_NAME} PRIVATE
		"${CMAKE_CURRENT_SOURCE_DIR}/src"
)

target_sources(${PROJECT_NAME} PRIVATE
	"${LXMAX_COMMON_INCLUDES}/lx_common_syms.c"
	"${MAX_SDK_INCLUDES}/common/commonsyms.c"
	"src/lx_common.c"
	"src/lx_dmx.c"
	"src/lx_helpers.cpp"
	"src/lx_net.c"
	"src/lx_net_macos.c"
	"src/lx_net_win.c"
	"src/lx_serial.c"
	"src/lx_serial_macos.cpp"
	"src/lx_serial_win.c"
	"src/lx_service.c"
	"src/lx_universe_config.c"
	"src/main.c"
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

if (APPLE)

	find_library(
		MSP_LIBRARY "MaxAudioAPI"
		REQUIRED
		PATHS "${MAX_SDK_MSP_INCLUDES}"
		NO_DEFAULT_PATH
		#only use the specific path above, don't look in system root
		#this enables cross compilation to provide an alternative root
		#but also find this specific path
		NO_CMAKE_FIND_ROOT_PATH
	)
	target_link_libraries(${PROJECT_NAME} PUBLIC ${MSP_LIBRARY})
	
	find_library(
		JITTER_LIBRARY "JitterAPI"
		REQUIRED
		PATHS "${MAX_SDK_JIT_INCLUDES}"
		NO_DEFAULT_PATH
		NO_CMAKE_FIND_ROOT_PATH
	)

	find_library(IOKIT_LIBRARY IOKit)
	find_library(FOUNDATION_LIBRARY Foundation)

	target_link_libraries(${PROJECT_NAME} PUBLIC ${JITTER_LIBRARY} ${FOUNDATION_LIBRARY} ${IOKIT_LIBRARY})

elseif (WIN32)

	target_link_libraries(${PROJECT_NAME} PUBLIC ${MaxAPI_LIB})
	target_link_libraries(${PROJECT_NAME} PUBLIC ${MaxAudio_LIB})
	target_link_libraries(${PROJECT_NAME} PUBLIC ${Jitter_LIB})

	target_link_libraries(${PROJECT_NAME} PRIVATE Iphlpapi.lib Ws2_32.lib Setupapi.lib)

	if (MSVC)
		# warning about constexpr not being const in c++14
		set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/wd4814")

		# do not generate ILK files
		set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/INCREMENTAL:NO")

		# allow parallel builds
		set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "/MP")
	endif ()

	target_compile_definitions(${PROJECT_NAME} PRIVATE "-DEXCLUDE_FROM_COLLECTIVES" "-DNOMINMAX")

endif ()