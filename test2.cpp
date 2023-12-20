#include <ios>
#include <iostream>
#include <stdexcept>
#include <vector>


#include <adios2.h>

int main(int argc, char *argv[]){			
	std::vector<float> data;
	std::size_t sizeR = 10;
	int rank = 0;	
  
	adios2::ADIOS adios;
	adios2::IO bpIO = adios.DeclareIO("bpIO");

	for(int x=0;x<sizeR;x++){
		for(int y=0;y<sizeR;y++){
			data.push_back(x+y);		
		}
	}
  
	const std::string extent = "0 " + std::to_string(sizeR) + " 0 " + std::to_string(sizeR) + " 0 " + std::to_string(sizeR);
	
	const std::string imageData = R"(
    		<?xml version="1.0"?>
     		<VTKFile type="ImageData" version="0.1" byte_order="LittleEndian">
      		 <ImageData WholeExtent=")" + extent + R"(" Origin="0 0 0" Spacing="1 1 0">
         	    <Piece Extent=")" + extent + R"(">
           	      <PointData>
               		<DataArray Name="bpTester" />
           	      </PointData>
         	    </Piece>
		 </ImageData>
     		</VTKFile>)";

	bpIO.DefineAttribute<std::string>("vtk.xml", imageData);
	
	adios2::Variable<float> bpTester = bpIO.DefineVariable<float>("bpTester", {sizeR,sizeR,sizeR}, {0,0,0}, {sizeR,sizeR,sizeR});
	
	std::string filename = "fileTest2.bp";

	adios2::Engine bpWriter = bpIO.Open(filename, adios2::Mode::Write);
	
	bpWriter.BeginStep();

	bpWriter.Put(bpTester,data.data());
		
	bpWriter.EndStep();
	bpWriter.Close();	
	 
	std::cout << "Finished\n";		
	return 0;
}
