#ifndef _LIBRARY_H_
#define _LIBRARY_H_

#include <string>
#include <set>
#include <vector>
#include <map>

class Library
{
private:
    struct TABLE{
        unsigned no_Index1;
        unsigned no_Index2;
        double *index1Array;
        double *index2Array;
        double *dataTable;      //size==no_Index1*no_Index2
        
        TABLE():no_Index1(0),no_Index2(0),index1Array(NULL),index2Array(NULL),dataTable(NULL){}
        ~TABLE(){
            delete [] index1Array;
            delete [] index2Array;
            delete [] dataTable;
        }
    };
    struct PIN{
        std::string name;
        std::string function;
        double capacitance;
        double maxTransition;
        double maxCapacitance;
        std::vector<TABLE*> FallTimingTable;
        std::vector<TABLE*> RiseTimingTable;
        std::vector<TABLE*> FallTransitionTable;
        std::vector<TABLE*> RiseTransitionTable;
        
        PIN(std::string n):name(n),function(""),capacitance(0),maxTransition(0),maxCapacitance(0){}
        ~PIN(){
            for(unsigned i=0;i<FallTimingTable.size();i++) {
                delete FallTimingTable[i];
                FallTimingTable[i]=NULL;
            }
            for(unsigned i=0;i<RiseTimingTable.size();i++) {
                delete RiseTimingTable[i];
                RiseTimingTable[i]=NULL;
            }
            for(unsigned i=0;i<FallTransitionTable.size();i++) {
                delete FallTransitionTable[i];
                FallTransitionTable[i]=NULL;
            }
            for(unsigned i=0;i<RiseTransitionTable.size();i++) {
                delete RiseTransitionTable[i];
                RiseTransitionTable[i]=NULL;
            }
        }
    };
    struct CELL{
        std::string name;
        std::vector<PIN*> pinList;
        
        CELL(std::string n):name(n){}
        ~CELL(){
            for(unsigned i=0;i<pinList.size();i++) {
                delete pinList[i];
                pinList[i]=NULL;
            }
        }
    };
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
     * @param function function of the pin (call by reference as returning type)
	 * @return true if the gate exists. false otherwise
	 */
	bool getFunction(const std::string& gate,
			const std::string& pin, std::string& function) const;

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
    
    /**
     * Compute the value suit the parameter
     *
     * @param tableList table list to compute max value
     * @param index1 the former parameter of 2d table
	 * @param index2 the latter parameter of 2d table
     * @return the max value
     */
    double computeTable(std::vector<TABLE*> &tableList, double index1, double index2) const;
private:
    std::map<std::string,CELL*> nameToCell;
    bool pinExists(const std::string& gate, const std::string& pin, PIN* &ptr) const;
};

#endif /* _LIBRARY_H_ */
