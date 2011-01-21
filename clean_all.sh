#!/bin/bash

echo ""
echo "-------------------------------------------------------------------" 
echo "-- ViennaMesh 1.0  2010                                          --" 
echo "-------------------------------------------------------------------" 
echo "--  developed by:                                                --" 
echo "--    Franz Stimpfl, Rene Heinzl, Philipp Schwaha                --" 
echo "--  maintained by:                                               --" 
echo "--    Josef Weinbub, Johann Cervenka                             --" 
echo "--  Institute for Microelectronics, TU Wien, Austria             --"
echo "--  http://www.iue.tuwien.ac.at                                  --"  
echo "-------------------------------------------------------------------" 

echo ""
echo "-------------------------------------------------------------------"
echo "## cleaning hull converter .. "
echo "-------------------------------------------------------------------"
cd hull_converter/
./clean.sh
cd ..

echo ""
echo "-------------------------------------------------------------------"
echo "## cleaning hull adaptor .. "
echo "-------------------------------------------------------------------"
cd hull_adaptor/
./clean.sh
cd ..

echo ""
echo "-------------------------------------------------------------------"
echo "## cleaning volume mesher .. "
echo "-------------------------------------------------------------------"
cd volume_mesher/
./clean.sh
cd ..

echo ""
echo "-------------------------------------------------------------------"
echo "## cleaning mesh classifier .. "
echo "-------------------------------------------------------------------"
cd mesh_classifier/
./clean.sh
cd ..

echo ""
echo "-------------------------------------------------------------------"
echo "## cleaning hull orienter .. "
echo "-------------------------------------------------------------------"
cd hull_orienter/
./clean.sh
cd ..

echo ""
echo "-------------------------------------------------------------------"
echo "## done .. "
echo "-------------------------------------------------------------------"
