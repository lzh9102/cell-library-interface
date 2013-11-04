#include "library.h"
#include <fstream>
#include <iostream>
using namespace std;

Library::Library()
{
}

Library::~Library()
{
    map<string, CELL*>::iterator it;
    for (it=nameToCell.begin(); it!=nameToCell.end(); ++it) {
        delete it->second;
        it->second=NULL;
    }
}

void CheckBracesLevel(string str,unsigned &level) {
    if (str.find('{')!=string::npos) {
        level++;
    }
    if (str.find('}')!=string::npos) {
        level--;
    }
    
}
void ReplaceWhiteSpace(string &str) {
    for (unsigned i=0; i<str.size();) {
        if (str[i]==' ') {
            str.erase(str.begin()+i);
            continue;
        }
        i++;
    }
}
void GetNextLine(fstream &infile,string &next_line,unsigned &BracesLevel) {
    next_line.clear();
    
    string str;
    string::size_type pos;
    while (!infile.eof()) {
        getline(infile, str);
        if ((pos=str.rfind('\\'))!=string::npos) {
           str=str.substr(0,pos);
        }
        next_line+=str;
        if (str.find(';')!=string::npos) {
            break;
        }
        if (str.find('{')!=string::npos) {
            break;
        }
        if (str.find('}')!=string::npos) {
            break;
        }
    }
    ReplaceWhiteSpace(next_line);
    CheckBracesLevel(str, BracesLevel);
}

void ExtractNumbersFromString(string str,unsigned &length,double* &array) {
    if (str[0]=='"') {str=str.substr(1);}
    if (str[str.size()-1]=='"') {str=str.substr(0,str.size()-1);}
    
    str+=',';
    for (unsigned i=0; i<str.size(); i++) {
        if (str[i]==',') {length++;}
    }
    array=new double[length];
    
    unsigned index=0;
    string::size_type pos;
    string number;
    while ((pos=str.find(','))!=string::npos) {
        number=str.substr(0,pos);
        str=str.substr(pos+1);
        
        array[index++]=atof(number.c_str());
    }
}

bool Library::loadLibrary(const string& filename, const set<string>& cells)
{
    if (fopen(filename.c_str(), "r")==NULL) {
        cout << "Can't open library file: " << filename << endl;
        exit(-1);
    }
    fstream infile(filename.c_str(),ios::in);
    
    string str;
    string::size_type pos;
    unsigned BracesLevel=1;;
    while (!infile.eof()) {
        GetNextLine(infile, str, BracesLevel);
        
        
        if (BracesLevel==3 && (pos=str.find("cell("))!=string::npos) { //"cell" define detected
            str=str.substr(pos+5);
            pos=str.find(')');
            str=str.substr(0,pos);
            
            bool cell_used= cells.find(str)!=cells.end();
            if (cell_used) {
                //save cell information
                
                CELL* cell=new CELL(str);
                nameToCell[str]=cell;
                
                while (!infile.eof()) {
                    GetNextLine(infile, str,BracesLevel);
                    
                    if (BracesLevel==4 && (pos=str.find("pin("))!=string::npos) { //"pin" define detected
                        
                        str=str.substr(pos+4);
                        pos=str.find(')');
                        str=str.substr(0,pos);
                        
                        PIN* pin=new PIN(str);
                        cell->pinList.push_back(pin);
                        while (!infile.eof()) {
                            
                            GetNextLine(infile, str, BracesLevel);
                            
                            if ((pos=str.find("function"))!=string::npos) {
                                pos=str.find(';');
                                str=str.substr(0,pos);
                                pos=str.find(':');
                                str=str.substr(pos+1);
                                //cout<<str<<endl;
                                
                                pos=str.find('"');
                                str=str.substr(pos+1);
                                pos=str.find('"');
                                str=str.substr(0,pos);
                                pin->function=str;
                            }
                            else if ((pos=str.find("capacitance"))!=string::npos) {
                                pos=str.find(';');
                                str=str.substr(0,pos);
                                pos=str.find(':');
                                str=str.substr(pos+1);
                                
                                pin->capacitance=atof(str.c_str());
                            }
                            else if ((pos=str.find("max_transition"))!=string::npos) {
                                pos=str.find(';');
                                str=str.substr(0,pos);
                                pos=str.find(':');
                                str=str.substr(pos+1);
                                
                                pin->maxTransition=atof(str.c_str());
                            }
                            else if ((pos=str.find("max_capacitance"))!=string::npos) {
                                pos=str.find(';');
                                str=str.substr(0,pos);
                                pos=str.find(':');
                                str=str.substr(pos+1);
                                
                                pin->maxCapacitance=atof(str.c_str());
                            }
                            else if ((pos=str.find("timing()"))!=string::npos) {
                                //enter the timing braces in a PIN definition
                                
                                while (!infile.eof()) {
                                    GetNextLine(infile, str, BracesLevel);
                                    
                                    TABLE* table;
                                    
                                    if ((pos=str.find("cell_fall"))!=string::npos) {
                                        table=new TABLE;
                                        pin->FallTimingTable.push_back(table);
                                    }
                                    else if ((pos=str.find("cell_rise"))!=string::npos) {
                                        table=new TABLE;
                                        pin->RiseTimingTable.push_back(table);
                                    }
                                    else if ((pos=str.find("fall_transition"))!=string::npos) {
                                        table=new TABLE;
                                        pin->FallTransitionTable.push_back(table);
                                    }
                                    else if ((pos=str.find("rise_transition"))!=string::npos) {
                                        table=new TABLE;
                                        pin->RiseTransitionTable.push_back(table);
                                    }
                                    
                                    if (pos!=string::npos) {
                                        
                                        while (!infile.eof()) {
                                            GetNextLine(infile, str, BracesLevel);
                                            
                                            if ((pos=str.find("index_1"))!=string::npos) {
                                                pos=str.find('(');
                                                str=str.substr(pos+1);
                                                pos=str.rfind(')');
                                                str=str.substr(0,pos);
                                                
                                                ExtractNumbersFromString(str,table->no_Index1,table->index1Array);
                                            }
                                            else if ((pos=str.find("index_2"))!=string::npos) {
                                                pos=str.find('(');
                                                str=str.substr(pos+1);
                                                pos=str.rfind(')');
                                                str=str.substr(0,pos);
                                                
                                                ExtractNumbersFromString(str,table->no_Index2,table->index2Array);
                                            }
                                            else if ((pos=str.find("values"))!=string::npos) {
                                                pos=str.find('(');
                                                str=str.substr(pos+1);
                                                pos=str.rfind(')');
                                                str=str.substr(0,pos);
                                                
                                                unsigned CheckLength=0;
                                                ExtractNumbersFromString(str,CheckLength,table->dataTable);
                                                
                                                if (CheckLength!=table->no_Index1*table->no_Index2) {
                                                    cout<<"Error at timing table generation phase."<<endl;
                                                    exit(-1);
                                                }
                                            }
                                            
                                            if (BracesLevel<6) break;
                                        }
                                        
                                    }
                                    
                                    if (BracesLevel<5) break;
                                }
                            }
                            else if (BracesLevel<4) break;
                            
                        }
                    }
                    else if (BracesLevel<3) break;
                }
            }
        }
        else if (BracesLevel==0) {
                cout<<"Mismatch on Braces. {}"<<endl;
                exit(-1);
        }
    }
    infile.close();
	return true;
}

bool Library::gateExists(const string& gate) const
{
    if (nameToCell.find(gate)==nameToCell.end()) {
        return false;
    }
	return true;
}

bool Library::pinExists(const string& gate, const string& pin) const
{
    if (nameToCell.find(gate)==nameToCell.end()) {
        return false;
    }
    CELL* cell=nameToCell.find(gate)->second;
    for (unsigned i=0; i<cell->pinList.size(); i++) {
        if (cell->pinList[i]->name.compare(pin)==0) {
            return true;
        }
    }
	return false;
}

bool Library::pinExists(const string& gate, const string& pin, PIN* &ptr) const
{
    if (nameToCell.find(gate)==nameToCell.end()) {
        ptr=NULL;
        return false;
    }
    CELL* cell=nameToCell.find(gate)->second;
    for (unsigned i=0; i<cell->pinList.size(); i++) {
        if (cell->pinList[i]->name.compare(pin)==0) {
            ptr=cell->pinList[i];
            return true;
        }
    }
    ptr=NULL;
	return false;
}

bool Library::getFunction(const string& gate,
			const string& pin, std::string& function) const
{
    if (nameToCell.find(gate)==nameToCell.end()) {
        return false;
    }
    CELL* cell=nameToCell.find(gate)->second;
    for (unsigned i=0; i<cell->pinList.size(); i++) {
        if (cell->pinList[i]->name.compare(pin)==0) {
            function=cell->pinList[i]->function;
            return true;
        }
    }
	return false;
}

double Library::computeTable(vector<TABLE*> &tableList, double index1, double index2) const {
    
    return 0;
}

double Library::computeRiseTrans(const string& gate,
			const string& pin, double itrans, double ocap) const
{
    PIN* ptr;
    if (pinExists(gate, pin, ptr)) {
        return computeTable(ptr->RiseTransitionTable, itrans, ocap);
    }
	return 0;
}

double Library::computeFallTrans(const string& gate,
			const string& pin, double itrans, double ocap) const
{
    PIN* ptr;
    if (pinExists(gate, pin, ptr)) {
        return computeTable(ptr->FallTransitionTable, itrans, ocap);
    }
	return 0;
}

double Library::computeRiseDelay(const string& gate,
			const string& pin, double itrans, double ocap) const
{
    PIN* ptr;
    if (pinExists(gate, pin, ptr)) {
        return computeTable(ptr->RiseTimingTable, itrans, ocap);
    }
	return 0;
}

double Library::computeFallDelay(const string& gate,
			const string& pin, double itrans, double ocap) const
{
    PIN* ptr;
    if (pinExists(gate, pin, ptr)) {
        return computeTable(ptr->FallTimingTable, itrans, ocap);
    }
	return 0;
}
