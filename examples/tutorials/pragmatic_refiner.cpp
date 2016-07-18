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

int main(int argc, char **argv)
{
/*VIENNAMESH INPUT PARAMETERS*/
	// Check and read console input 
	int region_count = 0;
	int refinement_passes = 0;
	std::string filename;

	if (argc < 4)
	{
		std::cout << "Parameters missing!" << std::endl;
		std::cout << "Correct use of parameters: <region_count> <refinement_passes> <filename>" << std::endl;
		return -1;
	}

	else if (atoi(argv[1]))
	{
		region_count = atoi(argv[1]);
		refinement_passes = atoi(argv[2]);
		filename = argv[3];
	}

	else if (!argv[2])
	{
		std::cout << "Parameter <input_file> missing!" << std::endl;
		return -1;
	}
	
	else
	{
		std::cout << "Wrong type of input parameter <region count>" << std::endl;
		return -1;
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
/*
	//Create algorithm handle for pragmatic refining
	viennamesh::algorithm_handle pragmatic_refine = context.make_algorithm("pragmatic_refine");
	pragmatic_refine.set_default_source(mesh_reader);
	pragmatic_refine.set_input("region_count", region_count);
	pragmatic_refine.set_input("refinement_passes", refinement_passes);
	pragmatic_refine.set_input("input_file", filename);
	pragmatic_refine.run();
/*
	//Create algorithm handle for pragmatic coarsening
	viennamesh::algorithm_handle pragmatic_coarsen = context.make_algorithm("pragmatic_coarsen");
	pragmatic_coarsen.set_default_source(mesh_reader);
	pragmatic_coarsen.set_input("region_count", region_count);
	pragmatic_coarsen.run();

	//Create algorithm handle for extracting the mesh boundary and run it
	viennamesh::algorithm_handle extract_boundary = context.make_algorithm("extract_boundary");
	extract_boundary.set_default_source(mesh_reader);
	extract_boundary.run();

/*	//Create algorithm handle for line coarsening and run it
	viennamesh::algorithm_handle line_coarsening = context.make_algorithm("line_coarsening");
	line_coarsening.set_default_source(extract_boundary);
	line_coarsening.set_input("angle", 3.14);	
	line_coarsening.run();

	// Create hull mesh
	viennamesh::algorithm_handle hull_mesher = context.make_algorithm("triangle_make_hull");
	hull_mesher.set_default_source(line_coarsening);
	hull_mesher.set_input("min_angle", 0.4);
	hull_mesher.set_input("cell_size", 1.0);
	hull_mesher.run();

	//Create volume mesh
	viennamesh::algorithm_handle volume_mesh = context.make_algorithm("tetgen_make_mesh");
	volume_mesh.set_default_source(extract_boundary);
	volume_mesh.link_input("geometry", extract_boundary, "mesh");			// IMPORTANT: LINK GEOMETRY WITH MESH, SINCE TETGEN REQUIRES GEOMETRY AS INPUT!!!
	volume_mesh.set_input("cell_size", 0.1);
	volume_mesh.run();

	// Create algorithm handle for mesh partitioning, set its default source and input, and run it
	viennamesh::algorithm_handle mesh_partitioner = context.make_algorithm("metis_mesh_partitioning");
	mesh_partitioner.set_default_source(volume_mesh);
	mesh_partitioner.set_input("region_count", region_count);
	mesh_partitioner.set_input("multi_mesh_output", false);
	mesh_partitioner.run();
*/
	// Create algorithm handle for pragmatic refining, set its defalt source and addiational inputs and finally run it
	viennamesh::algorithm_handle pragmatic_refine = context.make_algorithm("pragmatic_refine");
	pragmatic_refine.set_default_source(mesh_reader);
	pragmatic_refine.set_input("region_count", region_count);
	pragmatic_refine.set_input("refinement_passes", refinement_passes);
	pragmatic_refine.set_input("input_file", filename);
	pragmatic_refine.run();

/*	// Write mesh to file
	viennamesh::algorithm_handle mesh_writer = context.make_algorithm("mesh_writer");
	mesh_writer.set_default_source(pragmatic_refine);
  	mesh_writer.set_input( "filename", "/home/lgnam/Desktop/software/ViennaMesh/viennamesh-dev/build/examples/data/myfirsttask/pragmatic_refined_mesh.vtu" );
  	mesh_writer.run();

	//Write partitioned mesh
/*	viennamesh::algorithm_handle write_partitioned_mesh = context.make_algorithm("mesh_writer");
	write_partitioned_mesh.set_default_source(mesh_partitioner);
	write_partitioned_mesh.set_input("filename", "../data/myfirsttask/partitioned_mesh.vtu");
	write_partitioned_mesh.run();

	//Merge mesh
	viennamesh::algorithm_handle mesh_merger = context.make_algorithm("merge_meshes");
	mesh_merger.set_default_source(mesh_partitioner);
	mesh_merger.run();

	//Write partitioned mesh
	viennamesh::algorithm_handle write_merged_mesh = context.make_algorithm("mesh_writer");
	write_merged_mesh.set_default_source(volume_mesh);
	write_merged_mesh.set_input("filename", "../data/myfirsttask/merged_mesh.vtu");
	write_merged_mesh.run();
*/
/*END OF VIENNAMESH FRAMEWORK PART*/

    	return 0;
}
