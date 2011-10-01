/* =======================================================================
   Copyright (c) 2011, Institute for Microelectronics,
                       Institute for Analysis and Scientific Computing,
                       TU Wien.

                            -----------------
                     ViennaGrid - The Vienna Grid Library
                            -----------------

   Authors:      Karl Rupp                           rupp@iue.tuwien.ac.at
                 Josef Weinbub                    weinbub@iue.tuwien.ac.at
               
   (A list of additional contributors can be found in the PDF manual)

   License:      MIT (X11), see file LICENSE in the base directory
======================================================================= */

#ifndef VIENNAGRID_ALGORITHM_CELLNORMALS_GUARD
#define VIENNAGRID_ALGORITHM_CELLNORMALS_GUARD

#include "viennagrid/forwards.h"
#include "viennagrid/point.hpp"
#include "viennagrid/algorithm/cross_prod.hpp"
#include "viennagrid/algorithm/norm.hpp"
#include "viennadata/api.hpp"


namespace viennadata { namespace config {
   template <>
   struct key_dispatch<viennagrid::seg_cell_normal_tag> {
      typedef type_key_dispatch_tag    tag;
   };    
}}

namespace viennagrid {
// -----------------------------------------------------------------------------
// 
// a domain specific cell normal vector algorithm
//
template<int DIMG, typename CellTagT> 
struct assignCellNormals_impl
{
   template <typename DomainT>
   static void eval(DomainT& domain)
   {
      std::cerr << "ViennaGrid - Error: Cell Normal Algorithm not implemented for this domain type" << std::endl;
   }
};

// for triangular hull meshes
//
template<> 
struct assignCellNormals_impl <3, viennagrid::triangle_tag>
{
   template <typename DomainT>
   static void eval(DomainT& domain)
   {
      typedef typename DomainT::config_type                    DomainConfiguration;
      typedef typename DomainConfiguration::cell_tag           CellTag;   
      typedef typename DomainConfiguration::numeric_type       NumericType;

      static const int DIMT = DomainConfiguration::cell_tag::dim;   
      static const int CELLSIZE = viennagrid::topology::bndcells<CellTag, 0>::num;
      
      typedef typename DomainT::segment_type                                                                      SegmentType; 
      typedef typename viennagrid::result_of::ncell<DomainConfiguration, DIMT>::type      CellType;
      typedef typename viennagrid::result_of::ncell_range<SegmentType, DIMT>::type         CellRange;          
      typedef typename viennagrid::result_of::iterator<CellRange>::type                                       CellIterator;         
      typedef typename viennagrid::result_of::ncell_range<CellType, 0>::type                                  VertexOnCellRange;
      typedef typename viennagrid::result_of::iterator<VertexOnCellRange>::type                               VertexOnCellIterator;            
      
      typedef typename viennagrid::result_of::point<DomainConfiguration>::type                               PointType;
      
      std::vector<PointType>     cell_points(CELLSIZE);
      
      // traverse the segments
      //
      for (std::size_t si = 0; si < domain.segments().size(); ++si)
      {
         SegmentType & seg = domain.segments()[si];

         // traverse the cells of this segment
         //
         CellRange cells = viennagrid::ncells<DIMT>(seg);
         
         for (CellIterator cit = cells.begin(); cit != cells.end(); ++cit)
         {
            // get the cell points
            //
            std::size_t vi = 0;       
            VertexOnCellRange vertices_for_cell = viennagrid::ncells<0>(*cit);
            for (VertexOnCellIterator vocit = vertices_for_cell.begin();
                vocit != vertices_for_cell.end();
                ++vocit)
            {
               cell_points[vi++] = vocit->point();
            }
            
            // setup two vectors which represent the cell plane
            //  based on one common vertex
            //
            PointType vec1 = cell_points[1]-cell_points[0];
            PointType vec2 = cell_points[2]-cell_points[0];
            
            // compute the normal vector on this plane
            //
            PointType normal_vector = viennagrid::cross_prod(vec1,vec2);            
            
            // compute the magnitude of the normal vector
            //
            NumericType magnitude = viennagrid::norm(normal_vector);

            // normalize the normal-vector
            //
            normal_vector /= magnitude;
            
            // store it via viennadata on the cell in respect to the 
            // current cell
            // 
            viennadata::access<
               viennagrid::seg_cell_normal_tag,         // key-type
               viennagrid::seg_cell_normal_data::type   // data-type
            >()(*cit)[si]= normal_vector;
         }
      }
   }
};

// for quadrilateral hull meshes
//
template<> 
struct assignCellNormals_impl <3, viennagrid::quadrilateral_tag>
{
   template <typename DomainT>
   static void eval(DomainT& domain)
   {
      // the 3d_triangular specialization holds for 3d_quadrilateral
      // configuration too ..
      //
      assignCellNormals_impl<3, viennagrid::triangle_tag>::eval(domain);
   }
};

// -----------------------------------------------------------------------------
// 
// the generic access function
// note: extracts meta-data from the domain and forwards it to the 
//       tag-dispatched algorithm specialization hierarchy
//
template <typename DomainT>
void assign_cell_normals(DomainT & domain)
{
   typedef typename DomainT::config_type                          DomainConfiguration;

   typedef typename DomainConfiguration::cell_tag                 CellTag;
   
   assignCellNormals_impl<DomainConfiguration::coordinate_system_tag::dim, CellTag>::eval(domain);
}

} // end namespace viennagrid


#endif 
