// viennamesh-dev/examples/tutorials/lg_mesh_partitioner.cpp
//
// reads mesh from file, creates a triangle mesh, partitions it and saves it into files

#include <string>
#include <deque>
#include <vector>
#include <cassert>
#include <iostream>

#include "viennameshpp/core.hpp"

using namespace viennamesh;

int main(int argc, char* argv[])
{
	//check and read console input
	int region_count = 0;

	if (argc < 2)
	{
		std::cout << "Parameters <region count> missing!" << std::endl;
		return -1;
	}

	else if (atoi(argv[1]))
	{
		region_count = atoi(argv[1]);
	}
	
	else
	{
		std::cout << "Wrong type of input parameter <region count>" << std::endl;
		return -1;
	}
	
	//create context handle
	context_handle context;

	//Create algorithm handle for reading the mesh from a file and run it
	algorithm_handle mesh_reader = context.make_algorithm("mesh_reader");
	mesh_reader.set_input("filename", "../data/box200x200_decompressed.vtu");
	mesh_reader.run();

	//Create initial mesh
	algorithm_handle init_mesher = context.make_algorithm("triangle_make_mesh");
	init_mesher.set_default_source(mesh_reader);
	init_mesher.run();

	//Create algorithm handle for mesh partitioning, set its default source and input, and run it
	algorithm_handle mesh_partitioner = context.make_algorithm("metis_mesh_partitioning");
	mesh_partitioner.set_default_source(init_mesher);
	mesh_partitioner.set_input("region_count", region_count);
	mesh_partitioner.set_input("multi_mesh_output", true);
	mesh_partitioner.run();

	//Write mesh to file
	algorithm_handle mesh_writer = context.make_algorithm("mesh_writer");
	mesh_writer.set_default_source(mesh_partitioner);
  	mesh_writer.set_input( "filename", "lg_mesh_partitioner.vtu" );
  	mesh_writer.run();

	return 0;
}
