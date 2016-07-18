#include "pragmatic_refine_multi.hpp"

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

namespace viennamesh
{
		pragmatic_refine_multi::pragmatic_refine_multi()	{}
		std::string pragmatic_refine_multi::name() {return "pragmatic_refine_multi";}


		bool pragmatic_refine_multi::run(viennamesh::algorithm_handle &)
		{
		  std::cout << "Pragmatic refine multi" << std::endl;

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

		  //check if region_count has been provided
		  if (region_count.valid())
			std::cout << "pragmatic plugin called with region count = " << region_count() << std::endl;

		  else
			std::cout << "pragmatic plugin called with only 1 region" << std::endl;

		  //check if a value for refinement_passes has been provided, otherwise set it to a default value
		  if (refinement_passes.valid())
			no_of_passes = refinement_passes();
		  else
			no_of_passes = 10; 
	
		  //get the region pointers of all regions of a mesh
		  viennagrid_region_id * region_ids_begin;
		  viennagrid_region_id * region_ids_end;

		  viennagrid_mesh_regions_get(input_mesh().internal(), &region_ids_begin, &region_ids_end);

		  std::cout << *region_ids_begin << ", " << *region_ids_end << std::endl;

		  viennagrid_region_id  region_id = 1;
		  viennagrid_region * region;

		  viennagrid_mesh_region_get(input_mesh().internal(), region_id, region);

		  std::cout << *region << std::endl;

		  set_output("mesh", input_mesh());		

		  return true;
		} //end run()
}
