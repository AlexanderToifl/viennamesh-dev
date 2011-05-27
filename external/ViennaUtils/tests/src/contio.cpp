/* =============================================================================
   Copyright (c) 2010, 2011, Institute for Microelectronics, TU Wien
   http://www.iue.tuwien.ac.at
                             -----------------
                ViennaUtils - The Vienna Utilities Library
                             -----------------

   authors:    Josef Weinbub                      weinbub@iue.tuwien.ac.at
               Markus Bina                        bina@iue.tuwien.ac.at
               Karl Rupp                          rupp@iue.tuwien.ac.at

   license:    see file LICENSE in the base directory
============================================================================= */

#include<vector>
#include<cmath>
#include<iostream>
#include<fstream>

#include"viennautils/contio.hpp"


int main(int argc, char * argv[])
{
   std::vector<double>  cont;
   cont.push_back(1.0);
   cont.push_back(2.0);

   // the container can be printed directly
   // additionally the size of the container is printed
   std::cout << cont;

   // or the container can be printed line by line to std::cout ..
   viennautils::print()(cont);

   // or to a filestream ..
   std::ofstream stream("containerfile.txt");
   viennautils::print()(cont, stream);
   stream.close();
   return 0;
}