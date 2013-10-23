#include <set>
#include "library.h"

using namespace std;

int main(int argc, char *argv[])
{
	// cells of interest
	set<string> cells;
	cells.insert("not");
	cells.insert("and2");
	cells.insert("and3");
	cells.insert("or2");

	// load library
	Library library;
	library.loadLibrary("test.lib", cells);

	return 0;
}
