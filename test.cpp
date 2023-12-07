#include <ios>
#include <iostream>
#include <stdexcept>
#include <vector>


#include <adios2.h>

int main(int argc, char *argv[]){			
	std::vector<float> tester = {0,1,2,3,4,5,6,7,8,9};
	int sizeR = 10;
	int rank = 0;
	std::vector<float> row;	
	const std::size_t Nx = tester.size();

	adios2::ADIOS adios;
	adios2::IO bpIO = adios.DeclareIO("bpIO");

	const std::string imageData = R"(
    		<?xml version="1.0"?>
     		<VTKFile type="ImageData" version="0.1" byte_order="LittleEndian">
      		  <ImageData WholeExtent="x1 x2 y1 y2" Origin="x0 y0" Spacing="1 1">
             	    <Piece Extent="x1 x2 y1 y2">
           	      <PointData>
               		<DataArray Name="X" />
               		<DataArray Name="Y" />
           	      </PointData>
         	    </Piece>
		  </ImageData>
     		</VTKFile>)";

	bpIO.DefineAttribute<std::string>("vtk.xml", imageData);
	
	adios2::Variable<float> bpTester = bpIO.DefineVariable<float>("bpTester", {sizeR * Nx}, {rank * Nx}, {Nx});
	
	std::string filename = "fileTest.bp";

	adios2::Engine bpWriter = bpIO.Open(filename, adios2::Mode::Write);
	
	bpWriter.BeginStep();

	for(int x=0;x<sizeR;x++){
		for(int y=0;y<sizeR;y++){
			row.push_back(x+y);		
		}
		bpTester.SetSelection({{Nx * x}, {Nx}});
		bpWriter.Put(bpTester,row.data());
		row.clear();
	}
	
	bpWriter.EndStep();

	bpWriter.Close();	
	 
	std::cout << "Finished\n";		
	return 0;
}

