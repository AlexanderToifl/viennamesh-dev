/* =============================================================================
   Copyright (c) 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                 ViennaMesh - The Vienna Mesh Generation Framework
                             -----------------

   authors:    Josef Weinbub                         weinbub@iue.tuwien.ac.at


   license:    see file LICENSE in the base directory
============================================================================= */

#include <iostream>
#include <vector>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include "viennautils/config.hpp"
#include "viennautils/convert.hpp"
#include "viennautils/contio.hpp"
#include "viennautils/io.hpp"
#include "viennautils/file.hpp"

#include "viennagrid/domain.hpp"
#include "viennagrid/io/vtk_writer.hpp"
#include "viennagrid/io/vtk_reader.hpp"
#include "viennagrid/io/gau_reader.hpp"



#include "viennamesh/generator.hpp"
#include "viennamesh/wrapper.hpp"
#include "viennamesh/transfer/viennagrid.hpp"
#include "viennamesh/adaptors.hpp"

namespace viennamesh {

namespace key {
std::string geometry = "geometry";
} // end namespace key

namespace query {

struct input
{
   template<typename ConfigT>
   static std::string type(ConfigT const& config)
   {
      return config.query("/input/type/key/text()");
   }
};


} // end namespace query
} // end namespace viennamesh



int main(int argc, char *argv[])
{
   if(argc != 4)
   {
      std::cerr << "## Error::Parameter - usage: " << argv[0] << " inputfile outputfile configfile" << std::endl;
      std::cerr << "## shutting down .." << std::endl;
      return -1;
   }
   
   std::string inputfile(argv[1]);
   std::string outputfile(argv[2]);
   
//   std::string::size_type pos = inputfile.rfind(".")+1;
//   std::string input_extension = inputfile.substr(pos, inputfile.size());
//   pos = outputfile.rfind(".")+1;   
//   std::string output_extension = outputfile.substr(pos, outputfile.size());

   std::string input_extension  = viennautils::file_extension(inputfile);
   std::string output_extension = viennautils::file_extension(outputfile);

   typedef viennautils::config<viennautils::tag::xml>::type    config_type;
   config_type config;
   std::ifstream configstream(argv[3]);
   config.read(configstream);
   configstream.close();

   if( input_extension == "vtu" )
   {
      typedef viennagrid::domain<viennagrid::config::line_2d>                  domain_type;
      domain_type domain;
      
      viennagrid::io::importVTK(domain, inputfile);
   }
   else
   if(input_extension == "bnd")
   {
      if(viennamesh::query::input::type(config) == viennamesh::key::geometry)
      {
         viennautils::io::bnd_reader my_bnd_reader;
         my_bnd_reader(inputfile);
         my_bnd_reader.dump();

         typedef viennamesh::wrapper<viennamesh::tag::bnd, viennautils::io::bnd_reader>     bnd_wrapper_type;
         bnd_wrapper_type data_in(my_bnd_reader);      
         
         typedef viennamesh::result_of::mesh_generator<viennamesh::tag::cervpt, bnd_wrapper_type>::type   bnd_mesh_generator_type;
         bnd_mesh_generator_type mesher(data_in);      
         mesher();         

//         typedef viennagrid::domain<viennagrid::config::triangular_3d>  domain_out_type;
//         domain_out_type domain_out;      
//         
//         typedef viennamesh::transfer<viennamesh::tag::viennagrid>      transfer_type;
//         transfer_type  transfer;
//         transfer(mesher, domain_out);      
//         
//         viennagrid::io::Vtk_writer<domain_out_type> my_vtk_writer;
//         my_vtk_writer.writeDomain(domain_out, outputfile);      
      }
   }
   else
   if(input_extension == "hin")
   {
      if(viennamesh::query::input::type(config) == viennamesh::key::geometry)
      {
         viennautils::io::hin_reader my_hin_reader;
         my_hin_reader(inputfile);
         //my_hin_reader.dump();

         typedef viennamesh::wrapper<viennamesh::tag::hin, viennautils::io::hin_reader>     hin_wrapper_type;
         hin_wrapper_type data_in(my_hin_reader);      

         typedef viennamesh::result_of::mesh_generator<viennamesh::tag::tetgen, hin_wrapper_type>::type   hin_mesh_generator_type;
         hin_mesh_generator_type mesher(data_in);      
         mesher();         

         typedef viennagrid::domain<viennagrid::config::triangular_3d> domain_out_type;
         domain_out_type domain_out;      

         typedef viennamesh::transfer<viennamesh::tag::viennagrid>      transfer_type;
         transfer_type  transfer;
         transfer(mesher, domain_out);      

         viennagrid::io::Vtk_writer<domain_out_type> my_vtk_writer;
         my_vtk_writer.writeDomain(domain_out, outputfile);            
      }
   }
   else
   if(input_extension == "gau32")
   {
      typedef viennagrid::domain<viennagrid::config::triangular_3d>        domain_type;
      domain_type domain;
      
      viennagrid::io::importGAU(domain, inputfile);      
      
      typedef viennamesh::wrapper<viennamesh::tag::viennagrid, domain_type>     gau_wrapper_type;
      gau_wrapper_type data_in(domain);      
      
//      typedef viennamesh::result_of::mesh_adaptor<viennamesh::tag::consistancy>::type mesh_adaptor_type;
//      mesh_adaptor_type mesh_adaptor;
//      mesh_adaptor(data_in);
      
      
      typedef viennamesh::result_of::mesh_generator<viennamesh::tag::tetgen, gau_wrapper_type>::type   gau_mesh_generator_type;
      gau_mesh_generator_type mesher(data_in);      

      mesher( boost::fusion::make_map<viennamesh::tag::criteria, viennamesh::tag::size>(viennamesh::tag::conforming_delaunay(), 1.0) );         

      typedef viennagrid::domain<viennagrid::config::tetrahedral_3d> domain_out_type;
      domain_out_type domain_out;      
      
      typedef viennamesh::transfer<viennamesh::tag::viennagrid>      transfer_type;
      transfer_type  transfer;
      transfer(mesher, domain_out);      
      
      viennagrid::io::Vtk_writer<domain_out_type> my_vtk_writer;
      my_vtk_writer.writeDomain(domain_out, outputfile);      
   }
   else
   if(input_extension == "gts")
   {
      typedef viennagrid::domain<viennagrid::config::line_2d>                  domain_type;
      domain_type domain;
      
      viennautils::io::gts_reader my_gts_reader;
      my_gts_reader(domain, inputfile);      
      
      typedef viennamesh::wrapper<viennamesh::tag::viennagrid, domain_type>     vgrid_wrapper_type;
      vgrid_wrapper_type data_in(domain);      
      
      typedef viennamesh::result_of::mesh_generator<viennamesh::tag::triangle, vgrid_wrapper_type>::type   mesh_generator_type;
      mesh_generator_type mesher(data_in);      

      mesher( boost::fusion::make_map<viennamesh::tag::criteria, viennamesh::tag::size>(viennamesh::tag::conforming_delaunay(), 1.0) );         

      typedef viennagrid::domain<viennagrid::config::triangular_2d> domain_out_type;
      domain_out_type domain_out;      
      
      typedef viennamesh::transfer<viennamesh::tag::viennagrid>      transfer_type;
      transfer_type  transfer;
      transfer(mesher, domain_out);      
      
      viennagrid::io::Vtk_writer<domain_out_type> my_vtk_writer;
      my_vtk_writer.writeDomain(domain_out, outputfile);
   }
   else
   {
      std::cerr << "## input file format not supported: " << input_extension << std::endl;
      std::cerr << "## shutting down .. " << std::endl;
      return -1;   
   }

   return 0;
}


