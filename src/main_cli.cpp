#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <set>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
using namespace std;

string Percent_Removed(int lines_removed, int savelength){
	float pcnt =  ((float)lines_removed/(float)savelength)*100;
	ostringstream sp;
	sp << pcnt;
	string p = sp.str();
	p = p.substr(0,5);
	return p;
}

void Scan_IDs(vector<string> &savelines, set<string> &valid, string target){
	for(vector<string>::const_iterator i = savelines.begin(); i != savelines.end(); ++i) {
		string line = *i;
		if(line == target){
			i++;
			line = *i;
			string id = line.substr(6);
			if(valid.find(id) == valid.end()) {valid.insert(id);}
		}
	}
}

void Strip_Deceased(vector<string> &savelines, set<string> &valid, string target, int &removed, int &lines_removed){
	vector<string> purged_save;
	for(vector<string>::const_iterator i = savelines.begin(); i != savelines.end(); ++i) {
		string line = *i;
		if(line == target){
			int j = 0;
			bool strip = false;
			while(1){
				i++; j++;
				string _l = *i;
				if(_l.substr(0,9) == "\t\t\t\t\t\tid="){
					string id = _l.substr(9);
					if(valid.find(id) == valid.end()) {strip = true;}
					break;
				}
			}
			i -= j;
			if(strip){
				removed++;
				string prev_line = purged_save.back();
				while(1){
					lines_removed++;
					i++;
					string _l = *i;
					if(_l == "\t\t\t\t}"){
						lines_removed++;
						break;
					}
				}
				i++;
				line = *i;
				if(prev_line.substr(0,3) == "\t\t\t" && isdigit(prev_line[3]) && line == "\t\t\t}") {
					purged_save.pop_back();
					lines_removed += 2;
					continue;
				}
				i--;
				continue;
			}
		}
		purged_save.push_back(line);
	}
	savelines = purged_save;
}
