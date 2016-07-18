// viennamesh-dev/examples/tutorials/lg_convert_poly_to_vtu.cpp
//
// Opens a .poly file and writes it into a .vtu file

#include <string>
#include <deque>
#include <vector>

#include <cassert>
#include <iostream>

#include "viennameshpp/core.hpp"

using namespace viennamesh;

int main()
{
	//Create context handle
	context_handle context;

	//Create algorithm handle for reading the mesh from a file and run it
	algorithm_handle mesh_reader = context.make_algorithm("plc_reader");
	mesh_reader.set_input( "filename", "../data/two_cubes.poly" );
	mesh_reader.run();

	viennamesh::algorithm_handle mesher = context.make_algorithm("triangle_make_hull");
  	mesher.set_default_source(mesh_reader);
  	mesher.run();

	//Create algorithm handle for writing the mesh to an output file, set its source and input, and finally write it
	algorithm_handle mesh_writer = context.make_algorithm("mesh_writer");
	mesh_writer.set_default_source(mesher);
	mesh_writer.set_input("filename", "../data/two_cubes.vtu");
	mesh_writer.run();

	return 0;
}
