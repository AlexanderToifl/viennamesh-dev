/* ============================================================================
   Copyright (c) 2011-2014, Institute for Microelectronics,
                            Institute for Analysis and Scientific Computing,
                            TU Wien.

                            -----------------
                ViennaMesh - The Vienna Meshing Framework
                            -----------------

                    http://viennamesh.sourceforge.net/

   License:         MIT (X11), see file LICENSE in the base directory
=============================================================================== */

#include "merge_meshes.hpp"

#include "viennagrid/algorithm/geometry.hpp"

namespace viennamesh
{
  template<bool mesh_is_const>
  void merge_meshes_impl(viennagrid::base_mesh<mesh_is_const> const & src_mesh,
                         viennagrid::mesh_t const & dst_mesh)
  {
    viennagrid::result_of::element_copy_map<>::type copy_map(dst_mesh);

    typedef viennagrid::base_mesh<mesh_is_const> SrcMeshType;
    typedef viennagrid::mesh_t DstMeshType;

    typedef typename viennagrid::result_of::element<SrcMeshType>::type CellType;

    typedef typename viennagrid::result_of::const_cell_range<SrcMeshType>::type ConstCellRangeType;
    typedef typename viennagrid::result_of::iterator<ConstCellRangeType>::type ConstCellIteratorType;

    typedef typename viennagrid::result_of::region_range<SrcMeshType>::type SrcRegionRangeType;
    typedef typename viennagrid::result_of::iterator<SrcRegionRangeType>::type SrcRegionRangeIterator;
    typedef typename viennagrid::result_of::region_id<DstMeshType>::type DstRegionIDType;

    DstRegionIDType region_id_offset = src_mesh.region_count();

    ConstCellRangeType cells(src_mesh);
    for (ConstCellIteratorType cit = cells.begin(); cit != cells.end(); ++cit)
    {
      CellType cell = copy_map(*cit);

      SrcRegionRangeType region_range(src_mesh, *cit);
      for (SrcRegionRangeIterator rit = region_range.begin(); rit != region_range.end(); ++rit)
      {
        viennagrid::add( dst_mesh.get_make_region((*rit).id()+region_id_offset), cell );
      }
    }
  }



  merge_meshes::merge_meshes() {}
  std::string merge_meshes::name() { return "merge_meshes"; }

  bool merge_meshes::run(viennamesh::algorithm_handle &)
  {
    mesh_handle input_mesh0 = get_required_input<mesh_handle>("mesh0");
    if (!input_mesh0)
      return false;

    mesh_handle input_mesh1 = get_required_input<mesh_handle>("mesh1");
    if (!input_mesh1)
      return false;

    mesh_handle output_mesh = make_data<mesh_handle>();

    merge_meshes_impl( input_mesh0(), output_mesh() );
    merge_meshes_impl( input_mesh1(), output_mesh() );


    return true;
  }

}
