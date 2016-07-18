// examples/tutorials/lg_viemesh_pragmatic.cpp
//
// Reads mesh from vtu-file and creates pragmatic data structure

#include <string>
#include <deque>
#include <vector>

#include <cassert>
#include <iostream>

#include "viennameshpp/core.hpp"
/*
//Pragmatic includes
#include "Mesh.h"
#include "VTKTools.h"
#include "MetricField.h"
#include "Coarsen.h"
#include "Refine.h"
#include "Smooth.h"
#include "Swapping.h"
#include "ticker.h"
*/
using namespace viennamesh;

int main(int argc, char **argv)
{
	//Check input parameters
	std::string filename;

	if (argc < 2)
	{
		std::cout << "Parameter <input_file>!" << std::endl;
		return -1;
	}

	else
	{
		filename = argv[1];
	}

	//Read mesh from file
	// Create context handle
	viennamesh::context_handle context;

	// Create algorithm handle for reading the mesh from a file and run it
	viennamesh::algorithm_handle mesh_reader = context.make_algorithm("mesh_reader");
	mesh_reader.set_input("filename", filename.c_str());
	mesh_reader.run();

	//Extract number of nodes from mesh
	double* ptr = nullptr;

/*	mesh_reader.internal();
	viennagrid_mesh_vertex_coords_pointer(mesh_reader, ptr);
*/

	return 0;
}
