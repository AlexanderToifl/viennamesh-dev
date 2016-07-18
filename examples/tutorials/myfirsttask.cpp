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
//#include <omp.h>	//is included in the pragmatic header files!
#include <cmath>

//test implementation
//#include <dirent.h>

//Pragmatic includes
#include "Mesh.h"
#include "VTKTools.h"
#include "MetricField.h"
#include "Coarsen.h"
#include "Refine.h"
#include "Smooth.h"
#include "Swapping.h"
#include "ticker.h"

//ViennaMesh includes
#include "viennameshpp/core.hpp"

int main(int argc, char **argv)
{
/*VIENNAMESH INPUT PARAMETERS*/
	// Check and read console input 
	int region_count = 0;
	std::string filename;

	if (argc < 3)
	{
		std::cout << "Parameters missing!" << std::endl;
		return -1;
	}

	else if (atoi(argv[1]))
	{
		region_count = atoi(argv[1]);
		filename = argv[2];
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

	double tic = omp_get_wtime();

	// Create context handle
	viennamesh::context_handle context;

	// Create algorithm handle for reading the mesh from a file and run it
	viennamesh::algorithm_handle mesh_reader = context.make_algorithm("mesh_reader");
	mesh_reader.set_input("filename", filename.c_str());
	mesh_reader.run();

	//Create algorithm handle for extracting the mesh boundary and run it
	viennamesh::algorithm_handle extract_boundary = context.make_algorithm("extract_boundary");
	extract_boundary.set_default_source(mesh_reader);
	extract_boundary.run();
/*

	//Create algorithm handle for line coarsening and run it
	viennamesh::algorithm_handle line_coarsening = context.make_algorithm("line_coarsening");
	line_coarsening.set_default_source(extract_boundary);
	line_coarsening.set_input("angle", 3.14);
	line_coarsening.run();

	cout << "LINE COARSENING DONE" << endl;

	// Create hull mesh
	viennamesh::algorithm_handle hull_mesher = context.make_algorithm("triangle_make_hull");
	hull_mesher.set_default_source(mesh_reader);
	hull_mesher.set_input("min_angle", 0.4);
	hull_mesher.set_input("cell_size", 1.0);
	hull_mesher.run();

	cout << "HULL MESHER DONE" << endl;
*/
/*	// Create algorithm handle for mesh partitioning, set its default source and input, and run it
	viennamesh::algorithm_handle mesh_partitioner = context.make_algorithm("metis_mesh_partitioning");
	mesh_partitioner.set_default_source(extract_boundary);
	mesh_partitioner.set_input("region_count", region_count);
	mesh_partitioner.set_input("multi_mesh_output", false);
	mesh_partitioner.run();*/

	//Create volume mesh
	viennamesh::algorithm_handle volume_mesh = context.make_algorithm("tetgen_make_mesh");
	volume_mesh.set_default_source(extract_boundary);
	volume_mesh.link_input("geometry", extract_boundary, "mesh");			// IMPORTANT: LINK GEOMETRY WITH MESH, SINCE TETGEN REQUIRES GEOMETRY AS INPUT!!!
	volume_mesh.set_input("cell_size", 100.0);
	volume_mesh.run();

	// Create algorithm handle for mesh partitioning, set its default source and input, and run it
	viennamesh::algorithm_handle mesh_partitioner = context.make_algorithm("metis_mesh_partitioning");
	mesh_partitioner.set_default_source(volume_mesh);
	mesh_partitioner.set_input("region_count", region_count);
	mesh_partitioner.set_input("multi_mesh_output", false);
	mesh_partitioner.run();
/*
	// Write mesh to file
	viennamesh::algorithm_handle mesh_writer = context.make_algorithm("mesh_writer");
	mesh_writer.set_default_source(volume_mesh);
  	mesh_writer.set_input( "filename", "../data/myfirsttask/tetgen_mesh.vtu" );
  	mesh_writer.run();
*/
	//Write partitioned mesh
	viennamesh::algorithm_handle write_partitioned_mesh = context.make_algorithm("mesh_writer");
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
	
/*END OF VIENNAMESH FRAMEWORK PART*/

/*PRAGMATIC INPUT PARAMETERS*/

// print all files in the directory ../data/test/output
/*
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir ("../data/test/output")) != NULL) 
	{
 		// print all the files and directories within directory 
  		while ((ent = readdir (dir)) != NULL) 
		{
    			printf ("%s\n", ent->d_name);
  		}
  		closedir (dir);
	} 
	else 
	{
  		// could not open directory
  		perror ("");
  		return EXIT_FAILURE;
	}
*/
/*END OF PRAGMATIC INPUT PARAMETERS*/

/* PRAGMATIC FRAMEWORK PART*/
/*
	cout << "START OF PRAGMATIC PART!!!" << endl;

	filename.clear();
	std::string outputfilename;

	for (size_t i = 0; i < region_count; ++i)
	{
		//create file_name
		filename += "../data/myfirsttask/lg_mesh_partitioner_";
		filename += std::to_string(i);
		filename += ".vtu";
		
		//cout << "Adapting " << filename << endl;

		// Read mesh from file
    		Mesh<double> *mesh=VTKTools<double>::import_vtu(filename.c_str());
		
		//cout << "Creating boundary" << endl;
		
		// Create boundary
    		mesh->create_boundary();

		//cout << "Creating metric field" << endl;

		// Create metric field
		MetricField<double,2> metric_field(*mesh);

		//cout << "setting metric field" << endl;

		// Set metric field
    		size_t NNodes = mesh->get_number_nodes();
    		for(size_t j=0; j<NNodes; ++j) 
		{
        		double m[] = {0.5, 0.0, 0.5};
        		metric_field.set_metric(m, j);
    		}
    
		//cout << "updating metric field" << endl;

		metric_field.update_mesh();

		//cout << "Mesh created" << endl;

		// Adapt the mesh, first coarsen, then swap, then refine, and after loop smooth the mesh    	
		Coarsen<double,2> coarsen(*mesh);
		Swapping<double,2> swapping(*mesh);
		Refine<double,2> refine(*mesh);
		Smooth<double,2> smooth(*mesh);

    		double L_up = sqrt(2.0); //std: sqrt(2.0)
    		double L_low = L_up*0.5;
		double alpha = sqrt(2.0)/2;
		double L_max = mesh->maximal_edge_length();

		//cout << "Preparation for adaption done" << endl;

		for (size_t k=0; k<20; ++k)
		{	
			//set L_ref
			double L_ref = std::max(alpha*L_max, L_up);			
			
			//Coarsening
			//cout << "Coarsening region " << i << " ..." << endl;
    			coarsen.coarsen(L_low, L_up);

			//Swapping
			//cout << "Swapping region " << i << " ..." << endl;
			swapping.swap(0.7); //std: 0.7

			//Refining
			//cout << "Refining region " << i << " ..." << endl;
			refine.refine(L_ref); //std: L_ref
		}

		// Defrag the mesh and write output file
    		mesh->defragment();

		//Smooth the mesh
		smooth.smart_laplacian(10);
		smooth.optimisation_linf(10);
		
		//create outputfile
		//.vtu is automatically appended!!!
		outputfilename += "../data/myfirsttask/lg_mesh_partitioner_adapted_";
		outputfilename += std::to_string(i);
		
    		VTKTools<double>::export_vtu(outputfilename.c_str(), mesh);

    		delete mesh;

		filename.clear();
		outputfilename.clear();
	}
	double toc = omp_get_wtime();

	cout << "END OF PRAGMATIC PART!!!" << endl;
	
	
/*END OF PRAGMATIC FRAMEWORK PART*/

/*MERGING THE ADAPTED SUBMESHES*/

	// Read the adapted submeshes
	//
	//how many submeshes?
	//for (size_t i = 0; i< region_count < ++i)
	//{
	/*	viennamesh::algorithm_handle submesh1 = context.make_algorithm("mesh_reader");	
		submesh1.set_input("filename", "../data/myfirsttask/lg_mesh_partitioner_adapted_0.vtu");
		submesh1.run();		

		viennamesh::algorithm_handle submesh2 = context.make_algorithm("mesh_reader");
		submesh2.set_input("filename", "../data/myfirsttask/lg_mesh_partitioner_adapted_1.vtu");
		submesh2.run();

		viennamesh::algorithm_handle submesh3 = context.make_algorithm("mesh_reader");
		submesh3.set_input("filename", "../data/myfirsttask/lg_mesh_partitioner_adapted_2.vtu");
		submesh3.run();

		viennamesh::algorithm_handle submesh4 = context.make_algorithm("mesh_reader");
		submesh4.set_input("filename", "../data/myfirsttask/lg_mesh_partitioner_adapted_3.vtu");
		submesh4.run();

	//} 	

	// Create algorithm handle for mesh merging, set its default source and input, and run it
	viennamesh::algorithm_handle mesh_merger = context.make_algorithm("merge_meshes");
	mesh_merger.set_default_source(submesh1);
	//mesh_merger.set_input("region_offset", true);
	//mesh_merger.set_input("tolerance", 1.0);
	mesh_merger.run();

	//Write merged mesh
	viennamesh::algorithm_handle write_merged_meshes = context.make_algorithm("mesh_writer");
	write_merged_meshes.set_default_source(mesh_merger);
  	write_merged_meshes.set_input( "filename", "../data/myfirsttask/lg_merged_mesh.vtu" );
  	write_merged_meshes.run();


/*END OF MERGING THE ADAPTED SUBMESHES*/
/*
	cout << "Running Time: " << toc - tic << " sec" << endl;
*/
    	return 0;
}
