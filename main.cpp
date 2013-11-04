#include <set>
#include "library.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
	// cells of interest
	set<string> cells;
	cells.insert("INVM0N");     //inv
	cells.insert("CKND2M12N");  //nand2
	cells.insert("ND3M0N");     //nand3
	cells.insert("NR2M0N");     //nor2

	// load library
	Library library;
	library.loadLibrary("l90sprvt_typ.lib", cells);

    cout<<library.computeRiseTrans("INVM0N", "Z", 0.45, 0.02)<<endl;
    
	return 0;
}
