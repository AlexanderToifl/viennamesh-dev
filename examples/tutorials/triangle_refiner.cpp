/*
/ MY FIRST TASK
/
/ file: src/myfirsttask.cpp
/  
/ This program reads in a mesh using ViennaMesh, partitions it with the ViennaMesh-Metis algorithm
/ and afterwards adapts it using algorithms implemented in the Pragmatic framework. Adaptation is 
/ achieved by coarsening, swapping, refining and finally smoothing.
/
/ Developer: 	Lukas Gnam
/		Institute for Microelectronics
/		Faculty of Electrical Engineering and Information Engineering
/		Vienna University of Technology
/		gnam@iue.tuwien.ac.at
/		2016
/
/
/
*/

#include <iostream>
#include <string>
#include <vector>
#include <cfloat>
#include <deque>
#include <cassert>
#include <cmath>

//ViennaMesh includes
#include "viennameshpp/core.hpp"

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

int main(int argc, char **argv)
{
/*VIENNAMESH INPUT PARAMETERS*/
	// Check and read console input 
	double cell_size=0.0;
	std::string filename;

	if (argc < 3)
	{
		std::cout << "Parameters missing!" << std::endl;
		std::cout << "Correct use of parameters: <filename>" << std::endl;
		return -1;
	}

	else
	{
		filename = argv[1];
		cell_size = atof(argv[2]);
	}

/*VIENNAMESH INPUT PARAMETERS*/

/*VIENNAMESH FRAMEWORK PART*/

	//double tic = omp_get_wtime();

	// Create context handle
	viennamesh::context_handle context;

	// Create algorithm handle for reading the mesh from a file and run it
	viennamesh::algorithm_handle mesh_reader = context.make_algorithm("mesh_reader");
	mesh_reader.set_input("filename", filename.c_str());
	mesh_reader.run();

/*	//Create algorithm handle for extracting the mesh boundary and run it
	viennamesh::algorithm_handle extract_boundary = context.make_algorithm("extract_boundary");
	extract_boundary.set_default_source(mesh_reader);
	extract_boundary.run();

/*	//Create algorithm handle for line coarsening and run it
	viennamesh::algorithm_handle line_coarsening = context.make_algorithm("line_coarsening");
	line_coarsening.set_default_source(extract_boundary);
	line_coarsening.set_input("angle", 3.14);	
	line_coarsening.run();
*/
/*	// Create hull mesh
	viennamesh::algorithm_handle hull_mesher = context.make_algorithm("triangle_make_hull");
	hull_mesher.set_default_source(line_coarsening);
	hull_mesher.set_input("min_angle", 0.4);
	hull_mesher.set_input("cell_size", 1.0);
	hull_mesher.run();
*/

	//Create volume mesh
	viennamesh::algorithm_handle triangle_mesh = context.make_algorithm("triangle_make_mesh");
	triangle_mesh.set_default_source(mesh_reader);
	triangle_mesh.set_input("cell_size", cell_size);
	triangle_mesh.run();
	
	// Write mesh to file
	viennamesh::algorithm_handle mesh_writer = context.make_algorithm("mesh_writer");
	mesh_writer.set_default_source(triangle_mesh);
  	mesh_writer.set_input( "filename", "/home/lgnam/Desktop/software/ViennaMesh/viennamesh-dev/build/examples/data/myfirsttask/triangle_mesh.vtu" );
  	mesh_writer.run();

/*END OF VIENNAMESH FRAMEWORK PART*/

    	return 0;
}
