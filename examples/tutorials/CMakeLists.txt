add_executable(data data.cpp)
target_link_libraries(data viennameshpp)

add_executable(file_read_write file_read_write.cpp)
target_link_libraries(file_read_write viennameshpp)

add_executable(triangle_mesher triangle_mesher.cpp)
target_link_libraries(triangle_mesher viennameshpp)

add_executable(triangle_hull_mesher triangle_hull_mesher.cpp)
target_link_libraries(triangle_hull_mesher viennameshpp)

add_executable(tetgen_mesher tetgen_mesher.cpp)
target_link_libraries(tetgen_mesher viennameshpp)

add_executable(extract_boundary extract_boundary.cpp)
target_link_libraries(extract_boundary viennameshpp)

add_executable(symmetry_2d symmetry_2d.cpp)
target_link_libraries(symmetry_2d viennameshpp)

add_executable(symmetry_3d symmetry_3d.cpp)
target_link_libraries(symmetry_3d viennameshpp)

#My Own Files
add_executable(lg_mesh lg_mesh.cpp)
target_link_libraries(lg_mesh viennameshpp)

add_executable(lg_mesh_partitioner lg_mesh_partitioner.cpp)
target_link_libraries(lg_mesh_partitioner viennameshpp)

add_executable(lg_convert_poly_to_vtu lg_convert_poly_to_vtu.cpp)
target_link_libraries(lg_convert_poly_to_vtu viennameshpp)

#MY FIRST TASK
# Find OpenMP Package
#find_package(OpenMP)
#if (OPENMP_FOUND)
#    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
#    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#    add_definitions(-DHAVE_OPENMP)
#endif()

# Set directory of VTK to the most recent build
set(VTK_DIR "/home/lgnam/Desktop/software/VTK-build")
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

add_executable(pragmatic_refiner pragmatic_refiner.cpp)
target_link_libraries(pragmatic_refiner viennameshpp)
target_link_libraries(pragmatic_refiner ${VTK_LIBRARIES})

add_executable(pragmatic_smoother pragmatic_smoother.cpp)
target_link_libraries(pragmatic_smoother viennameshpp)
target_link_libraries(pragmatic_smoother ${VTK_LIBRARIES})

add_executable(pragmatic_swapper pragmatic_swapper.cpp)
target_link_libraries(pragmatic_swapper viennameshpp)
target_link_libraries(pragmatic_swapper ${VTK_LIBRARIES})

add_executable(tetgen_refiner tetgen_refiner.cpp)
target_link_libraries(tetgen_refiner viennameshpp)

add_executable(triangle_refiner triangle_refiner.cpp)
target_link_libraries(triangle_refiner viennameshpp)

#ViennaMesh to Pragmatic Test
#add_executable(lg_viemesh_pragmatic lg_viemesh_pragmatic.cpp)
#target_link_libraries(lg_viemesh_pragmatic viennameshpp)
#target_link_libraries(lg_viemesh_pragmatic ${VTK_LIBRARIES})
