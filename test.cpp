#include <ios>
#include <iostream>
#include <stdexcept>
#include <vector>


#include <adios2.h>

int main(int argc, char *argv[]){			
	std::vector<float> tester = {0,1,2,3,4,5,6,7,8,9};
	int size = 10;
	int rank = 0;
	std::vector<float> row;	
	const std::size_t Nx = tester.size();

	adios2::ADIOS adios;
	adios2::IO bpIO = adios.DeclareIO("bpIO");
	
	adios2::Variable<float> bpTester = bpIO.DefineVariable<float>("bpTester", {size * Nx}, {rank * Nx}, {Nx}, adios2::ConstantDims);
	
	std::string filename = "fileTest.bp";

	adios2::Engine bpWriter = bpIO.Open(filename, adios2::Mode::Write);
	
	bpWriter.BeginStep();

	for(int x;x<size;x++){
		for(int y;y<size;y++){
			row.push_back(x+y);		
		}
		bpWriter.Put(bpTester,tester.data());
		row.clear();
	}
	
	bpWriter.EndStep();

	bpWriter.Close();	
	 
	std::cout << "Finished" << /n;		
	return 0;
}

