set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true )

if ( COMMAND cmake_policy )
  cmake_policy( SET CMP0003 NEW )
endif()

set(CGAL_DONT_OVERRIDE_CMAKE_FLAGS TRUE CACHE BOOL "" FORCE)
find_package( CGAL QUIET COMPONENTS  )
if ( NOT CGAL_FOUND )
  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()
endif()
include( ${CGAL_USE_FILE} )

find_package( Boost REQUIRED )
if ( NOT Boost_FOUND )
  message(STATUS "This project requires the Boost library, and will not be compiled.")
  return()
endif()

VIENNAMESH_ADD_PLUGIN(viennamesh-module-cgal plugin.cpp
                      cgal_mesh.cpp
                      cgal_simplify_mesh.cpp )

target_link_libraries(viennamesh-module-cgal ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES} )
