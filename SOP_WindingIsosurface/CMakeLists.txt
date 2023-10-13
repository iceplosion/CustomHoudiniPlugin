set( library_name SOP_WindingIsosurface )

houdini_generate_proto_headers( FILES SOP_WindingIsosurface.cpp )

# Add a library and its source files.
add_library( ${library_name} SHARED
    UT_SolidAngle.C
    UT_BVH.h
    UT_BVHImpl.h
    UT_SolidAngle.h
    SOP_WindingIsosurface.cpp
    SOP_WindingIsosurface.proto.h
    MarchingCube.h
    MarchingCube.cpp
)

# Link against the Houdini libraries, and add required include directories and
# compile definitions.
target_link_libraries( ${library_name} Houdini Houdini::Dep::openvdb_sesi)

# Include ${CMAKE_CURRENT_BINARY_DIR} for the generated header.
target_include_directories( ${library_name} PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_include_directories( ${library_name} PRIVATE ${HOUDINI_HEADER_PATH})

# Sets several common target properties, such as the library's output directory.
houdini_configure_target( ${library_name} )
