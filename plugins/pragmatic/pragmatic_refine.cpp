#include "pragmatic_refine.hpp"

//standard includes
#include <vector>
#include <string>

//Pragmatic includes
#include "Refine.h"
#include "pragmatic_mesh.hpp"/*
#include "Mesh.h"
#include "MetricField.h"
#include "ticker.h"
#include "VTKTools.h"
*/

#ifdef HAVE_OPENMP
#include <omp.h>
#endif

typedef viennagrid::result_of::element<MeshType>::type      	    CellType;

namespace viennamesh
{
		pragmatic_refine::pragmatic_refine()	{}
		std::string pragmatic_refine::name() {return "pragmatic_refine";}

		void inline make_metric(Mesh<double> *mesh, size_t geometric_dimension)
		{
			if(geometric_dimension == 2)
			{
				MetricField<double,2> metric_field(*mesh);

    		  		size_t NNodes = mesh->get_number_nodes();
    		  		double eta=0.0001;

    		  		std::vector<double> psi(NNodes);

				for(size_t i=0; i<NNodes; i++) 
		  		{
        				double x = 2*mesh->get_coords(i)[0]-1;
        				double y = 2*mesh->get_coords(i)[1]-1;

        				psi[i] = 0.100000000000000*sin(50*x) + atan2(-0.100000000000000, (double)(2*x - sin(5*y)));			
    		  		}

				metric_field.add_field(&(psi[0]), eta, 1);
    		 		metric_field.update_mesh();
			}

			else
			{
				MetricField<double,3> metric_field(*mesh);

    		  		size_t NNodes = mesh->get_number_nodes();
    		  		double eta=0.0001;

    		  		std::vector<double> psi(NNodes);

				for(size_t i=0; i<NNodes; i++) 
		  		{
					psi[i] = pow(mesh->get_coords(i)[0], 4) + pow(mesh->get_coords(i)[1], 4) + pow(mesh->get_coords(i)[2], 4);
				}

				metric_field.add_field(&(psi[0]), eta, 1);
    		 		metric_field.update_mesh();
			}
		}//end make_metric

		void make_refinement(Mesh<double> *mesh, size_t geometric_dimension, size_t no_of_passes)
		{
			if (geometric_dimension == 2)
			{
				Refine<double,2> adapt(*mesh);
			
				for(int i=1; i<=no_of_passes; i++)
		  		{	
					//std::cout << "Pass " << i << " of " << no_of_passes << std::endl;	//DEFAULT VALUE IS i<3; only 3 passes for refinement!
        				adapt.refine(sqrt(2.0));						//DEFAULT VALUE IS SQRT(2.0)
    		 		}				
			}

			else
			{
				Refine<double,3> adapt(*mesh);
			
				for(int i=1; i<=no_of_passes; i++)
		  		{	
					//std::cout << "Pass " << i << " of " << no_of_passes << std::endl;	//DEFAULT VALUE IS i<3; only 3 passes for refinement!
        				adapt.refine(sqrt(2.0));						//DEFAULT VALUE IS SQRT(2.0)
    		 		}
			}
		} //end make_refinement
		
		bool pragmatic_refine::run(viennamesh::algorithm_handle &)
		{
		  double tic = omp_get_wtime();
		  //
		  // Define the necessary types:
		  //
		  typedef viennagrid::mesh                                          MeshType;
		  typedef viennagrid::result_of::region<MeshType>::type             RegionType;

		  typedef viennagrid::result_of::element<MeshType>::type            VertexType;
		  typedef viennagrid::result_of::point<MeshType>::type              PointType;
		  typedef viennagrid::result_of::element<MeshType>::type      	    CellType;

		  typedef viennagrid::result_of::cell_range<RegionType>::type       CellRange;
		  typedef viennagrid::result_of::iterator<CellRange>::type          CellIterator;
						
		  //
		  // Get the types for a global vertex range and the corresponding iterator.
		  // This allows to traverse all vertices in the mesh.
		  // The first template argument to *_range<> denotes the enclosing body (here: the mesh),
		  //
		  typedef viennagrid::result_of::vertex_range<MeshType>::type       VertexRange;
		  typedef viennagrid::result_of::iterator<VertexRange>::type        VertexIterator;
			
		  //create mesh_handle to read input mesh			
		  mesh_handle input_mesh = get_required_input<mesh_handle>("mesh");
			
		  //create data_handle for optional inputs
		  data_handle<int> region_count = get_input<int>("region_count");
		  data_handle<int> refinement_passes = get_input<int>("refinement_passes");

		  //create string_handle to get input filename, used for benchmark purposes to store the output in a file (see at the end of this file!!!)
		  string_handle input_file = get_input<string_handle>("input_file");

		  int no_of_passes;
		  /*
		  //check if region_count has been provided
		  if (region_count.valid())
			std::cout << "pragmatic plugin called with region count = " << region_count() << std::endl;

		  else
			std::cout << "pragmatic plugin called with only 1 region" << std::endl;
		  */
		  //check if a value for refinement_passes has been provided, otherwise set it to a default value
		  if (refinement_passes.valid())
			no_of_passes = refinement_passes();
		  else
			no_of_passes = 10; 
			
		  //get topologic_dimension and geometric_dimension as well as the number of vertices and elements in the mesh
		 			
		  size_t cell_dimension = viennagrid::cell_dimension( input_mesh() );
		  size_t geometric_dimension = viennagrid::geometric_dimension( input_mesh() );
		  size_t NNodes = viennagrid::vertex_count( input_mesh() );
		  size_t NElements = viennagrid::cells( input_mesh() ).size();
	
		  //set up vectors for ENList and x-, y- nad z-coordinates
		  std::vector<index_t> ENList;
		  std::vector<double> x, y, z;
		  
		  //Initialize x-, y- and z-coordinates
		  //
		  //create pointer to iterate over viennagrid_array
		  viennagrid_numeric *ptr_coords = nullptr;
						
		  //get pointer to coordinates array from the mesh
		  viennagrid_mesh_vertex_coords_pointer(input_mesh().internal(), &ptr_coords);			
			
		  //iterate over all nodes in the mesh and store the coordinates in the vectors needed by pragmatic
		  //(coordinates are stored in a big array in the following scheme [x0 y0 z0 x1 y1 z1 ... xn yn zn])
		  for (size_t i=0; i<NNodes; ++i)
		  {			  
		    x.push_back(*(ptr_coords++));
		    y.push_back(*(ptr_coords++));
		  
		    //depending on the dimension push_back data from the array or push_back 0 into the pragmatic data
		    if (geometric_dimension == 3)
		      z.push_back(*(ptr_coords++));
		    
		    else
		      z.push_back(0);
		  }
		  
		  //Iterate over all triangles in the mesh
		  viennagrid_element_id * vertex_ids_begin;
		  viennagrid_element_id * vertex_ids_end;
		  viennagrid_dimension topological_dimension = geometric_dimension;		//produces segmentation fault if not set to 2 for 2d and to 3 for 3d case
												//THE SOLUTION IN THE ROW ABOVE IS OPTIMIZABLE!!!
		  //get elements from mesh
		  viennagrid_mesh_elements_get(input_mesh().internal(), topological_dimension, &vertex_ids_begin, &vertex_ids_end);
			
		  viennagrid_element_id * boundary_vertex_ids_begin;
		  viennagrid_element_id * boundary_vertex_ids_end;
		  viennagrid_dimension boundary_topological_dimension = 0;
		  viennagrid_element_id triangle_id;
						
		  int counter = 0;
		  			
		  //outer for loop iterates over all elements with dimension = topological_dimension (2 for triangles)
		  //inner for loop iterates over all elements with dimension = boundary_topological_dimension (0 for vertices)
		  //this info is needed to build the NEList which is used to build the pragmatic data structure
		  for (viennagrid_element_id * vit = vertex_ids_begin; vit != vertex_ids_end; ++vit)
		  {
		    //get vertices of triangle
		    triangle_id = *vit;
		    viennagrid_element_boundary_elements(input_mesh().internal(), triangle_id, boundary_topological_dimension, &boundary_vertex_ids_begin, &boundary_vertex_ids_end);
			  			  
		    for (viennagrid_element_id * vit = boundary_vertex_ids_begin; vit != boundary_vertex_ids_end; ++vit)
		    {
		      viennagrid_element_id vertex_id = *vit;
		      ENList.push_back(vertex_id);
		    }
			  		  
		    ++counter;
		  }

		  //Create Pragmatic mesh data structure
		  Mesh<double> *mesh = nullptr;	
		  
		  if (geometric_dimension == 2)	
		  {
		  	mesh = new Mesh<double> (NNodes, NElements, &(ENList[0]), &(x[0]), &(y[0]));
		  }
		
		  else
		  {		
			mesh = new Mesh<double> (NNodes, NElements, &(ENList[0]), &(x[0]), &(y[0]), &(z[0]));
		  }
/*
		  std::cout << std::endl << "Initial Mesh" << std::endl;
		  std::cout << "----------------------" << std::endl;
		  std::cout << "NNodes: " << mesh->get_number_nodes() << std::endl;
		  std::cout << "NElements: " << mesh->get_number_elements() << std::endl;
		  std::cout << "----------------------" << std::endl;
/*
    		  MetricField<double,2> metric_field(*mesh);

    		  NNodes = mesh->get_number_nodes();
    		  double eta=0.0001;

    		  std::vector<double> psi(NNodes);

		  if (geometric_dimension == 2)
		  {
    		  	for(size_t i=0; i<NNodes; i++) 
		  	{
        			double x = 2*mesh->get_coords(i)[0]-1;
        			double y = 2*mesh->get_coords(i)[1]-1;

        			psi[i] = 0.100000000000000*sin(50*x) + atan2(-0.100000000000000, (double)(2*x - sin(5*y)));			
    		  	}
		  }

		  else
		  {
			for(size_t i=0; i<NNodes; i++) 
		  	{
				psi[i] = pow(mesh->get_coords(i)[0], 4) + pow(mesh->get_coords(i)[1], 4) + pow(mesh->get_coords(i)[2], 4);
			}
		  }

    		  metric_field.add_field(&(psi[0]), eta, 1);
    		  metric_field.update_mesh();
*/
		  //set up the metric
		  make_metric(mesh, geometric_dimension);

   		  // Refine<double,2> adapt(*mesh);
		  
    		  double tic_refine = 0;
	          double toc_refine = 0;    		  

		  tic_refine = omp_get_wtime();
/*
		  for(int i=1; i<=no_of_passes; i++)
		  {	
			std::cout << "Pass " << i << " of " << no_of_passes << std::endl;	//DEFAULT VALUE IS i<3; only 3 passes for refinement!
        		adapt.refine(sqrt(2.0));						//DEFAULT VALUE IS SQRT(2.0)
    		  }
*/		
		  //refine the mesh
		  make_refinement(mesh, geometric_dimension, no_of_passes);

		  toc_refine = omp_get_wtime();
		
   		  mesh->defragment();	  

		  double toc_wo_reconversion = omp_get_wtime();

		  //write refined mesh into vtu-file using pragmatic library and data structure
		  //VTKTools<double>::export_vtu("examples/data/myfirsttask/pragmatic_refine", mesh);

		  //convert pragmatic mesh datastructure back into viennamesh data structure
		  //this is a first simple 2d example!!!
		  MeshType output_mesh;	
		  std::vector<VertexType> vertex_handles(mesh->get_number_nodes());	  
		  double *x_out;
		  x_out = new double [3];
		  const index_t *ptr_ENList=nullptr;
		 
		  NNodes = mesh->get_number_nodes();
		  NElements = mesh->get_number_elements();
	
		 /* float tmp_nn = (NNodes / 100) * 1;
		  float tmp_ne = (NElements / 100) * 1;
		  */
		 // std::cout << "Creating Vertices" << std::endl;

		  for (size_t i = 0; i < NNodes; ++i)
		  {	
		        mesh->get_coords(i, x_out);
			if (geometric_dimension==2)
			  vertex_handles[i]=viennagrid::make_vertex(output_mesh, viennagrid::make_point(x_out[0], x_out[1]));
			
			else
			  vertex_handles[i]=viennagrid::make_vertex(output_mesh, viennagrid::make_point(x_out[0], x_out[1], x_out[2]));

			/*if(i%(int)tmp_nn==0)
			  std::cout << (i/tmp_nn) << " %" << std::endl;*/
		  } 
	
		 // std::cout << std::endl << "Creating Elements" << std::endl;

		  for (size_t i = 0; i < NElements; ++i)
		  {
			//std::cout << "Element " << i << " of " << NElements << std::endl;
			ptr_ENList = mesh->get_element(i);
			
			if (geometric_dimension==2)
			  CellType cell = viennagrid::make_triangle(output_mesh, vertex_handles[ptr_ENList[0]], vertex_handles[ptr_ENList[1]], vertex_handles[ptr_ENList[2]]);
			
			else
			  CellType cell = viennagrid::make_tetrahedron(output_mesh, vertex_handles[ptr_ENList[0]], vertex_handles[ptr_ENList[1]], vertex_handles[ptr_ENList[2]], vertex_handles[ptr_ENList[3]]);

			/*if(i%(int)tmp_ne==0)
			  std::cout << (i/tmp_ne) << " %" << std::endl;*/
		  }

		  //set output mesh
		  set_output("mesh", output_mesh);

		  double toc_w_reconversion = omp_get_wtime();

		  //output results
		  double overall_time_w_reconversion = toc_w_reconversion-tic;
		  double overall_time_wo_reconversion = toc_wo_reconversion-tic;
		  double refine_time = toc_refine - tic_refine;

		//  std::cout << std::endl << "Refined Mesh" << std::endl;
		//  std::cout << "----------------------" << std::endl;
		//  std::cout << "Number of Passes: " << no_of_passes << std::endl;
		//  std::cout << "NNodes: " << mesh->get_number_nodes() << std::endl;
		//  std::cout << "NElements: " << mesh->get_number_elements() << std::endl;
		//  std::cout << "----------------------" << std::endl;
		/*  std::cout << "Number of Vertices: " << mesh->get_number_nodes() << std::endl;
		  std::cout << "Number of Elements: " << mesh->get_number_elements() << std::endl; 
		  std::cout << "Overall time with reconversion : " << overall_time_w_reconversion << std::endl;
		  std::cout << "Overall time without reconversion : " << overall_time_wo_reconversion << std::endl;
		  std::cout << "Time for refining: " << refine_time << " seconds." << std::endl;*/
		  
		  std::ofstream output;
		  std::string filename;
		 
		  std::size_t pos1 = input_file().find_last_of("/");
		  std::size_t pos2 = input_file().find(".vtu");
		  std::string tmp = input_file().substr(pos1+1,(pos2-pos1-1));
		  
		  filename="/home/lgnam/Desktop/benchmarks/pragmatic_plugin/data/benchmark_pragmatic_refine_";
		  filename+=tmp;
	   	  filename+=".txt";
		  
		  output.open(filename.c_str(), std::ofstream::out | std::ios_base::app);
		  output << static_cast<int>(no_of_passes) << " " << static_cast<int>(mesh->get_number_nodes()) << " " << static_cast<int>(mesh->get_number_elements()) << " " << overall_time_w_reconversion <<  \
		  " " << overall_time_wo_reconversion << " " << refine_time << std::endl; 

		  /*output  << "----------------------------------------" << std::endl << "Number of Passes: " << no_of_passes << std::endl << "NNodes: " << mesh->get_number_nodes() << std::endl \
		  << "NElements: " << mesh->get_number_elements() << std::endl << "Overall time : " << overall_time << " s. " << std::endl << "Time for refining: " << refine_time << " s." << std::endl;
		  std::cout  << "----------------------------------------" << std::endl << "Number of Passes: " << no_of_passes << std::endl << "NNodes: " << mesh->get_number_nodes() << std::endl \
		  << "NElements: " << mesh->get_number_elements() << std::endl << "Overall time : " << overall_time << " s. " << std::endl << "Time for refining: " << refine_time << " s." << std::endl;*/

		  output.close();

		  delete mesh;
		  delete x_out;

		  return true;
		} //end run()
}
