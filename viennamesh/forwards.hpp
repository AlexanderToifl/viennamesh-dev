#ifndef VIENNAMESH_FORWARDS_HPP
#define VIENNAMESH_FORWARDS_HPP

#include "viennagrid/config/default_configs.hpp"

#if __cplusplus > 199711L
  #include <functional>
  #include <memory>
#else
  #include <boost/shared_ptr.hpp>
  #include <boost/enable_shared_from_this.hpp>

  #include <boost/function.hpp>
  #include <boost/bind.hpp>
#endif

#include <boost/lexical_cast.hpp>

namespace viennamesh
{
#if __cplusplus > 199711L
  using std::shared_ptr;
  using std::enable_shared_from_this;
  using std::dynamic_pointer_cast;
  using std::static_pointer_cast;

  using std::function;
  using std::bind;
#else
  using boost::shared_ptr;
  using boost::enable_shared_from_this;
  using boost::dynamic_pointer_cast;
  using boost::static_pointer_cast;

  using boost::function;
  using boost::bind;
#endif

  using boost::lexical_cast;
  using std::string;


  class type_info_wrapper
  {
  public:
    type_info_wrapper() : info_(0) {}
    type_info_wrapper( std::type_info const & info ) : info_(&info) {}

    bool operator<( type_info_wrapper const & rhs ) const
    {
      if (info_ && rhs.info_)
        return info_->before(*rhs.info_);

      return false;
    }

    bool operator==( type_info_wrapper const & rhs ) const
    {
      return *info_ == *rhs.info_;
    }

    bool operator!=( type_info_wrapper const & rhs ) const
    {
      return !(*this == rhs);
    }

    string name() const
    {
      return info_->name();
    }

    std::type_info const * get() const
    { return info_; }

    template<typename TypeT>
    static type_info_wrapper make()
    {
      return type_info_wrapper( typeid(TypeT) );
    }

  private:
    std::type_info const * info_;
  };



  namespace result_of
  {
    template<unsigned int dim>
    struct point;

    template<>
    struct point<1>
    {
      typedef viennagrid::config::point_type_1d type;
    };

    template<>
    struct point<2>
    {
      typedef viennagrid::config::point_type_2d type;
    };

    template<>
    struct point<3>
    {
      typedef viennagrid::config::point_type_3d type;
    };




    template<typename PointT>
    struct point_container
    {
      typedef std::vector<PointT> type;
    };

    template<typename PointT>
    struct seed_point_container
    {
      typedef std::vector< std::pair<PointT, int> > type;
    };


    template<typename PointT>
    struct sizing_function
    {
      typedef viennamesh::function< double( PointT const & ) > type;
    };

  }


  typedef result_of::point<1>::type point_1d;
  typedef result_of::point<2>::type point_2d;
  typedef result_of::point<3>::type point_3d;

  typedef result_of::point_container<point_1d>::type point_1d_container;
  typedef result_of::point_container<point_2d>::type point_2d_container;
  typedef result_of::point_container<point_3d>::type point_3d_container;

  typedef result_of::seed_point_container<point_1d>::type seed_point_1d_container;
  typedef result_of::seed_point_container<point_2d>::type seed_point_2d_container;
  typedef result_of::seed_point_container<point_3d>::type seed_point_3d_container;

  typedef result_of::sizing_function<point_1d>::type sizing_function_1d;
  typedef result_of::sizing_function<point_2d>::type sizing_function_2d;
  typedef result_of::sizing_function<point_3d>::type sizing_function_3d;




  namespace result_of
  {
    template<typename CellTagT, unsigned int GeometricDimensionV>
    struct full_config;

    // vertex
    template<>
    struct full_config<viennagrid::vertex_tag, 1>
    { typedef viennagrid::config::vertex_1d type; };
    template<>
    struct full_config<viennagrid::vertex_tag, 2>
    { typedef viennagrid::config::vertex_2d type; };
    template<>
    struct full_config<viennagrid::vertex_tag, 3>
    { typedef viennagrid::config::vertex_3d type; };

    // line
    template<>
    struct full_config<viennagrid::line_tag, 1>
    { typedef viennagrid::config::line_1d type; };
    template<>
    struct full_config<viennagrid::line_tag, 2>
    { typedef viennagrid::config::line_2d type; };
    template<>
    struct full_config<viennagrid::line_tag, 3>
    { typedef viennagrid::config::line_3d type; };

    // triangle
    template<>
    struct full_config<viennagrid::triangle_tag, 2>
    { typedef viennagrid::config::triangular_2d type; };
    template<>
    struct full_config<viennagrid::triangle_tag, 3>
    { typedef viennagrid::config::triangular_3d type; };

    // quadrilateral
    template<>
    struct full_config<viennagrid::quadrilateral_tag, 2>
    { typedef viennagrid::config::quadrilateral_2d type; };
    template<>
    struct full_config<viennagrid::quadrilateral_tag, 3>
    { typedef viennagrid::config::quadrilateral_3d type; };

    // polygon
    template<>
    struct full_config<viennagrid::polygon_tag, 2>
    { typedef viennagrid::config::polygonal_2d type; };
    template<>
    struct full_config<viennagrid::polygon_tag, 3>
    { typedef viennagrid::config::polygonal_3d type; };

    // PLC
    template<>
    struct full_config<viennagrid::plc_tag, 3>
    { typedef viennagrid::config::plc_3d type; };

    // Tetrahedron
    template<>
    struct full_config<viennagrid::tetrahedron_tag, 3>
    { typedef viennagrid::config::tetrahedral_3d type; };

    // Hexahedron
    template<>
    struct full_config<viennagrid::hexahedron_tag, 3>
    { typedef viennagrid::config::hexahedral_3d type; };




    template<typename CellTagT, unsigned int GeometricDimensionV>
    struct thin_config;

    // line
    template<>
    struct thin_config<viennagrid::line_tag, 1>
    { typedef viennagrid::config::line_1d type; };
    template<>
    struct thin_config<viennagrid::line_tag, 2>
    { typedef viennagrid::config::line_2d type; };
    template<>
    struct thin_config<viennagrid::line_tag, 3>
    { typedef viennagrid::config::line_3d type; };

    // triangle
    template<>
    struct thin_config<viennagrid::triangle_tag, 2>
    { typedef viennagrid::config::thin_triangular_2d type; };
    template<>
    struct thin_config<viennagrid::triangle_tag, 3>
    { typedef viennagrid::config::thin_triangular_3d type; };

    // quadrilateral
    template<>
    struct thin_config<viennagrid::quadrilateral_tag, 2>
    { typedef viennagrid::config::thin_quadrilateral_2d type; };
    template<>
    struct thin_config<viennagrid::quadrilateral_tag, 3>
    { typedef viennagrid::config::thin_quadrilateral_3d type; };

    // Tetrahedron
    template<>
    struct thin_config<viennagrid::tetrahedron_tag, 3>
    { typedef viennagrid::config::thin_tetrahedral_3d type; };

    // Hexahedron
    template<>
    struct thin_config<viennagrid::hexahedron_tag, 3>
    { typedef viennagrid::config::thin_hexahedral_3d type; };
  }

}

#endif
