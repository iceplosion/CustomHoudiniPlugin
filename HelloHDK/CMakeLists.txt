set(PLUGIN_NAME "HelloHDK")
set(PLUGIN_SOURCE 
SOP_HelloHDK.C
)
add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCE})
target_include_directories(${PLUGIN_NAME} PRIVATE ${HOUDINI_HEADER_PATH})
target_link_libraries(${PLUGIN_NAME} Houdini)

# Configure several common target properties, such as its output directory.
# see also: https://www.sidefx.com/docs/hdk/_h_d_k__intro__compiling.html#HDK_Intro_Compiling_CMake
houdini_configure_target(${PLUGIN_NAME})
