/* =============================================================================
   Copyright (c) 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                 ViennaMesh - The Vienna Mesh Generation Framework
                             -----------------

   authors:    Josef Weinbub                         weinbub@iue.tuwien.ac.at
               Karl Rupp                                rupp@iue.tuwien.ac.at


   license:    see file LICENSE in the base directory
============================================================================= */

#ifndef VIENNAMESH_TAGS_HPP
#define VIENNAMESH_TAGS_HPP

namespace viennamesh {
   
namespace tag {
struct none     {};
struct triangle {};
struct tetgen   {};
struct netgen   {};
}
   
namespace key {
struct algorithm {};
struct criteria  {};
struct dim_topo  {};
struct dim_geom  {};
}

namespace val {
struct incremental_delaunay {};
struct advancing_front      {};
struct constrained_delaunay {};
struct conforming_delaunay  {};
struct one                  {};
struct two                  {};
struct three                {};
}
   
} // end namespace viennamesh

#endif