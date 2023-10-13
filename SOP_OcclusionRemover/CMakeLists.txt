set(PLUGIN_NAME "SOP_OcclusionRemover")
set(PLUGIN_SOURCE 
SOP_OcclusionRemover.cpp
)
add_library(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCE})
target_include_directories(${PLUGIN_NAME} PRIVATE ${HOUDINI_HEADER_PATH})
target_link_libraries(${PLUGIN_NAME} Houdini)
target_link_libraries(${PLUGIN_NAME} Houdini::Dep::embree_sidefx)
#target_link_libraries(${PLUGIN_NAME} Houdini::Dep::tbb)


houdini_configure_target(${PLUGIN_NAME})
