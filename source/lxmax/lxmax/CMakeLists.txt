set(C74_BUILD_MAX_EXTENSION yes)

include(../../max-sdk-base/script/max-pretarget.cmake)

include_directories(
		"${MAX_SDK_INCLUDES}"
		"${MAX_SDK_MSP_INCLUDES}"
		"${MAX_SDK_JIT_INCLUDES}"
		"${LXMAX_COMMON_INCLUDES}"
)

set(PROJECT_SRC
		"${LXMAX_COMMON_INCLUDES}/lx_common_syms.c"
		"${MAX_SDK_INCLUDES}/common/commonsyms.c"
		"lx.service.cpp"
		"internals/dmx_buffer.cpp"
		"internals/dmx_io_protocol.cpp"
		"internals/dmx_io_service.cpp"
        "internals/udp_socket_service.cpp"
		"internals/protocols/dmx_artnet_io_protocol.cpp"
		"internals/protocols/dmx_sacn_io_protocol.cpp"

)

add_library(
		${PROJECT_NAME}
		MODULE
		${PROJECT_SRC}
)

target_link_libraries(
		${PROJECT_NAME}
		PUBLIC
		liblxmax
		asio::asio
)

set(EXCLUDE_FROM_COLLECTIVES yes)

include(../../max-sdk-base/script/max-posttarget.cmake)