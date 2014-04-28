#ifndef VIENNAMESH_ALGORITHM_TRIANGLE_MAKE_MESH_HPP
#define VIENNAMESH_ALGORITHM_TRIANGLE_MAKE_MESH_HPP

#include "viennamesh/core/algorithm.hpp"
#include "viennamesh/algorithm/triangle/triangle_mesh.hpp"

namespace viennamesh
{
  namespace triangle
  {
    class make_mesh : public base_algorithm
    {
    public:
      make_mesh();

      string name() const;
      string id() const;

      bool run_impl();

    private:
      typedef viennagrid::segmented_mesh<triangle::input_mesh, triangle::input_segmentation> InputMeshType;
      required_input_parameter_interface<InputMeshType>             input_mesh;
      optional_input_parameter_interface<seed_point_2d_container>   input_seed_points;
      optional_input_parameter_interface<point_2d_container>        input_hole_points;
      dynamic_optional_input_parameter_interface                    sizing_function;
      optional_input_parameter_interface<double>                    cell_size;
      optional_input_parameter_interface<double>                    min_angle;
      default_input_parameter_interface<bool>                       delaunay;
      optional_input_parameter_interface<string>                    algorithm_type;
      default_input_parameter_interface<bool>                       extract_segment_seed_points;
      optional_input_parameter_interface<string>                    option_string;

      output_parameter_interface output_mesh;
    };
  }
}

#endif