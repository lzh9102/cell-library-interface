#include "library.h"

using namespace std;

Library::Library()
{
}

Library::~Library()
{
}

bool Library::loadLibrary(const string& filename, const set<string>& cells)
{
	// example: whether information of a cell should be saved
	bool and3_used = cells.find("and3") != cells.end();
	if (and3_used) {
		// save and3 information
	}
	return false;
}

bool Library::gateExists(const string& gate) const
{
	return false;
}

bool Library::pinExists(const string& gate, const string& pin) const
{
	return false;
}

string Library::getFunction(const string& gate,
			const string& pin) const
{
	return "";
}

double Library::computeRiseTrans(const string& gate,
			const string& pin, double itrans, double ocap) const
{
	return 0;
}

double Library::computeFallTrans(const string& gate,
			const string& pin, double itrans, double ocap) const
{
	return 0;
}

double Library::computeRiseDelay(const string& gate,
			const string& pin, double itrans, double ocap) const
{
	return 0;
}

double Library::computeFallDelay(const string& gate,
			const string& pin, double itrans, double ocap) const
{
	return 0;
}
