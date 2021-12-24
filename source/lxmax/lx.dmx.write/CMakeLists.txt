include(../../max-sdk-base/script/max-pretarget.cmake)

include_directories(
		"${MAX_SDK_INCLUDES}"
		"${MAX_SDK_MSP_INCLUDES}"
		"${MAX_SDK_JIT_INCLUDES}"
		"${LXMAX_COMMON_INCLUDES}"
)

set(PROJECT_SRC
		"lx.dmx.write.c"
		"${LXMAX_COMMON_INCLUDES}/lx_common_syms.c"
		"${MAX_SDK_INCLUDES}/common/commonsyms.c"
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
)

set(EXCLUDE_FROM_COLLECTIVES yes)

include(../../max-sdk-base/script/max-posttarget.cmake)