#include <plate.h>
int main(int argc, char ** argv)
{
	PLATE::Plate * p;
	p = new PLATE::Plate();
	return p->run(argc,argv);
}
