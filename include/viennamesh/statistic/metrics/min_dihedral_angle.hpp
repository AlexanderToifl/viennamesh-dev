#ifndef VIENNAMESH_STATISTICS_METRICS_MIN_DIHEDRAL_ANGLE_HPP
#define VIENNAMESH_STATISTICS_METRICS_MIN_DIHEDRAL_ANGLE_HPP

#include "viennagrid/algorithm/angle.hpp"
#include "viennamesh/statistics/forwards.hpp"

namespace viennamesh
{
  namespace metrics
  {
    template<typename PointAccessorT, typename ElementT, typename NumericLimitsT>
    typename viennagrid::result_of::coord<typename PointAccessorT::value_type>::type min_dihedral_angle_impl(PointAccessorT const point_accessor, ElementT const & element, NumericLimitsT, viennagrid::tetrahedron_tag)
    {
      typedef typename PointAccessorT::value_type            PointType;
      typedef typename viennagrid::result_of::coord<PointType>::type            NumericType;
      typedef typename viennagrid::result_of::const_vertex_range<ElementT>::type   VertexOnCellContainer;

      PointType const & p0 = point_accessor( viennagrid::vertices(element)[0] );
      PointType const & p1 = point_accessor( viennagrid::vertices(element)[1] );
      PointType const & p2 = point_accessor( viennagrid::vertices(element)[2] );
      PointType const & p3 = point_accessor( viennagrid::vertices(element)[3] );

      double da_01 = viennagrid::dihedral_angle( p0, p1, p2, p0, p1, p3 );
      double da_02 = viennagrid::dihedral_angle( p0, p2, p1, p0, p2, p3 );
      double da_03 = viennagrid::dihedral_angle( p0, p3, p1, p0, p3, p2 );
      double da_12 = viennagrid::dihedral_angle( p1, p2, p0, p1, p2, p3 );
      double da_13 = viennagrid::dihedral_angle( p1, p3, p0, p1, p3, p2 );
      double da_23 = viennagrid::dihedral_angle( p2, p3, p0, p2, p3, p1 );

      return std::min( std::min( std::min( da_01, da_02 ), std::min(da_03, da_12) ), std::min(da_13, da_23) );
    }
  }


  struct min_dihedral_angle_tag {};

  template<typename PointAccessorT, typename ElementT, typename NumericLimitsT>
  typename viennagrid::result_of::coord<typename PointAccessorT::value_type>::type min_dihedral_angle( PointAccessorT const point_accessor, ElementT const & element, NumericLimitsT numeric_limits )
  {
    return metrics::min_dihedral_angle_impl(point_accessor, element, numeric_limits, typename ElementT::tag());
  }

  template<typename PointAccessorT, typename ElementT>
  typename viennagrid::result_of::coord<typename PointAccessorT::value_type>::type min_dihedral_angle( PointAccessorT const point_accessor, ElementT const & element )
  {
    return min_dihedral_angle(point_accessor, element, std::numeric_limits< typename viennagrid::result_of::coord<typename PointAccessorT::value_type>::type >() );
  }

  template<typename ElementT>
  typename viennagrid::result_of::coord< ElementT >::type min_dihedral_angle(ElementT const & element)
  {
    return min_dihedral_angle( viennagrid::default_point_accessor(element), element);
  }


  namespace detail
  {
    template<typename PointAccessorT, typename ElementT, typename NumericLimitsT>
    typename viennagrid::result_of::coord<typename PointAccessorT::value_type>::type metric( PointAccessorT const point_accessor, ElementT const & element, NumericLimitsT numeric_limits, min_dihedral_angle_tag)
    {
      return min_dihedral_angle(point_accessor, element, numeric_limits);
    }

    template<typename PointAccessorT, typename ElementT>
    typename viennagrid::result_of::coord<typename PointAccessorT::value_type>::type metric( PointAccessorT const point_accessor, ElementT const & element, min_dihedral_angle_tag)
    {
      return min_dihedral_angle(point_accessor, element);
    }

    template<typename ElementT>
    typename viennagrid::result_of::coord< ElementT >::type metric( ElementT const & element, min_dihedral_angle_tag)
    {
      return min_dihedral_angle(element);
    }
  }

}


#endif