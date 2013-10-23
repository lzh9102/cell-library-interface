#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include <string>
#include <set>

class Library
{
public:
	Library();
	virtual ~Library();

	/**
	 * Load cell library from a file.
	 * Call setCellList() to set the cells before calling this function
	 *
	 * @param filename file name of the cell library
	 * @return true if success, false if failed
	 */
	bool loadLibrary(const std::string& filename,
			const std::set<std::string>& cells);

	/**
	 * Determine whether the library contains the gate
	 *
	 * @param gate name of the gate to lookup
	 * @return true if the gate exists, false otherwise
	 */
	bool gateExists(const std::string& gate) const;

	/**
	 * Determine whether the gate contains the pin
	 *
	 * @param gate name of the gate to lookup
	 * @param pin name of the pin to lookup
	 * @return true if the gate exists and the pin belongs to it; false
	 * otherwise
	 */
	bool pinExists(const std::string& gate, const std::string& pin) const;

	/**
	 * NOTE: The function definition is unclear
	 *
	 * @param gate name of the gate to lookup
	 * @param pin name of the pin
	 * @return string representation of the function. empty if the gate
	 */
	std::string getFunction(const std::string& gate,
			const std::string& pin) const;

	/**
	 * Compute the output transition time (rising)
	 *
	 * @note if gate or pin doesn't exist, the return value is undefined
	 *
	 * @param gate name of the gate
	 * @param pin name of the pin
	 * @param itrans input transition time into the pin
	 * @param ocap total output capacitance
	 */
	double computeRiseTrans(const std::string& gate,
			const std::string& pin, double itrans, double ocap) const;

	/**
	 * Compute the output rise transition time (falling)
	 *
	 * @param gate name of the gate
	 * @param pin name of the pin
	 * @param itrans input transition time into the pin
	 * @param ocap total output capacitance
	 * @see computeRiseTrans
	 */
	double computeFallTrans(const std::string& gate,
			const std::string& pin, double itrans, double ocap) const;

	/**
	 * Compute the delay from the input pin to output (rising)
	 *
	 * @param gate name of the gate
	 * @param pin name of the pin
	 * @param itrans input transition time into the pin
	 * @param ocap total output capacitance
	 * @see computeRiseTrans
	 */
	double computeRiseDelay(const std::string& gate,
			const std::string& pin, double itrans, double ocap) const;

	/**
	 * Compute the delay from the input pin to output (falling)
	 *
	 * @param gate name of the gate
	 * @param pin name of the pin
	 * @param itrans input transition time into the pin
	 * @param ocap total output capacitance
	 * @see computeRiseTrans
	 */
	double computeFallDelay(const std::string& gate,
			const std::string& pin, double itrans, double ocap) const;
private:
};

#endif /* _LIBRARY_H_ */
