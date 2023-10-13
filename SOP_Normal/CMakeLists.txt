set(PLUGIN_NAME "SOP_Normal")
set(PLUGIN_SOURCE 
SOP_Normal.cpp
)
add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCE})
target_include_directories(${PLUGIN_NAME} PRIVATE ${HOUDINI_HEADER_PATH})
target_link_libraries(${PLUGIN_NAME} Houdini)

houdini_configure_target(${PLUGIN_NAME})
