#ifndef VIENNAMESH_STATISTICS_METRICS_MIN_ANGLE_HPP
#define VIENNAMESH_STATISTICS_METRICS_MIN_ANGLE_HPP

#include "viennagrid/algorithm/angle.hpp"
#include "viennamesh/statistics/forwards.hpp"

namespace viennamesh
{
    namespace metrics
    {

        template<typename PointAccessorT, typename ElementT, typename NumericLimitsT>
        typename viennagrid::result_of::coord<typename PointAccessorT::value_type>::type min_angle_impl(PointAccessorT const point_accessor, ElementT const & element, NumericLimitsT numeric_limits, viennagrid::triangle_tag)
        {
            typedef typename PointAccessorT::value_type            PointType;
            typedef typename viennagrid::result_of::coord<PointType>::type            NumericType;
            typedef typename viennagrid::result_of::const_vertex_range<ElementT>::type   VertexOnCellContainer;

            PointType const & a = point_accessor( viennagrid::vertices(element)[0] );
            PointType const & b = point_accessor( viennagrid::vertices(element)[1] );
            PointType const & c = point_accessor( viennagrid::vertices(element)[2] );

            NumericType alpha = viennagrid::angle( a, b, c );
            NumericType beta = viennagrid::angle( a, b, c );
            NumericType gamma = M_PI - alpha - beta;

            return std::min(std::min( alpha, beta ), gamma);
        }


        template<typename PointAccessorT, typename ElementT, typename NumericLimitsT>
        typename viennagrid::result_of::coord<typename PointAccessorT::value_type>::type min_angle_impl(PointAccessorT const point_accessor, ElementT const & element, NumericLimitsT numeric_limits, viennagrid::tetrahedron_tag)
        {
            typedef typename PointAccessorT::value_type            PointType;
            typedef typename viennagrid::result_of::coord<PointType>::type            NumericType;
            typedef typename viennagrid::result_of::const_vertex_range<ElementT>::type   VertexOnCellContainer;

            PointType const & a = point_accessor( viennagrid::vertices(element)[0] );
            PointType const & b = point_accessor( viennagrid::vertices(element)[1] );
            PointType const & c = point_accessor( viennagrid::vertices(element)[2] );
            PointType const & d = point_accessor( viennagrid::vertices(element)[3] );

            NumericType alpha = viennagrid::solid_angle( a, b, c, d );
            NumericType beta = viennagrid::solid_angle( b, a, c, d );
            NumericType gamma = viennagrid::solid_angle( c, b, a, d );
            NumericType delta = viennagrid::solid_angle( d, b, c, a );

            return std::min( std::min( alpha, beta ), std::min(gamma, delta) );
        }
    }


    struct min_angle_tag {};

    template<typename PointAccessorT, typename ElementT, typename NumericLimitsT>
    typename viennagrid::result_of::coord<typename PointAccessorT::value_type>::type min_angle( PointAccessorT const point_accessor, ElementT const & element, NumericLimitsT numeric_limits )
    {
        return metrics::min_angle_impl(point_accessor, element, numeric_limits, typename ElementT::tag());
    }

    template<typename PointAccessorT, typename ElementT>
    typename viennagrid::result_of::coord<typename PointAccessorT::value_type>::type min_angle( PointAccessorT const point_accessor, ElementT const & element )
    {
        return min_angle(point_accessor, element, std::numeric_limits< typename viennagrid::result_of::coord<typename PointAccessorT::value_type>::type >() );
    }

    template<typename ElementT>
    typename viennagrid::result_of::coord< ElementT >::type min_angle(ElementT const & element)
    {
        return min_angle( viennagrid::default_point_accessor(element), element);
    }


    namespace detail
    {
      template<typename PointAccessorT, typename ElementT, typename NumericLimitsT>
      typename viennagrid::result_of::coord<typename PointAccessorT::value_type>::type metric( PointAccessorT const point_accessor, ElementT const & element, NumericLimitsT numeric_limits, min_angle_tag)
      {
          return min_angle(point_accessor, element, numeric_limits);
      }

      template<typename PointAccessorT, typename ElementT>
      typename viennagrid::result_of::coord<typename PointAccessorT::value_type>::type metric( PointAccessorT const point_accessor, ElementT const & element, min_angle_tag)
      {
          return min_angle(point_accessor, element);
      }

      template<typename ElementT>
      typename viennagrid::result_of::coord< ElementT >::type metric( ElementT const & element, min_angle_tag)
      {
          return min_angle(element);
      }
    }


}


#endif
