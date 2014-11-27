#include <plate.h>
#include <iostream>
#include <exception>

int main(int argc, char ** argv)
{
	try
	{
		PLATE::Plate * p;
		p = new PLATE::Plate();
		return p->run(argc,argv);
	}
	catch (std::exception const& ex)
	{
		std::cerr << ex.what() << std::endl;
		return 1;
	}
}
