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


int main(int argc, char** argv) {

	string outfileloc;
	vector<string> savelines;

	bool use_defaults				= false;	//for ultra-fast testing purposes
	bool verbose					= false;

	bool clean_invalid_tags			= true;
	bool clean_wars					= false;
	bool clean_province_history		= true;
	bool clean_decisions			= true;
	bool clean_advisors				= true;
	bool clean_buildings			= true;
	bool clean_occupations			= true;
	bool clean_province_history2	= true;
	bool clean_rulers				= false;
	bool clean_heirs				= true;
	bool clean_leaders				= true;

	cout << "EU Clean (version 9)";
	cout << "\nby Gakumerasara\n\n";

	if(use_defaults){
		verbose					= true;
		clean_invalid_tags		= false;
		clean_wars				= false;
		clean_province_history	= false;
		clean_decisions			= false;
		clean_advisors			= false;
		clean_buildings			= true;
		clean_occupations		= true;
		clean_province_history2	= true;
		clean_rulers			= true;
		clean_heirs				= true;
		clean_leaders			= true;

		ifstream infile("input.eu4");
		if(infile) {
			cout << "Importing savefile: input.eu4\n";
			string s;
			while(getline(infile,s)) {savelines.push_back(s);}
			infile.close();
		}
		else{cout << "\nError: unable to open file: input.eu4";}
	}
	else{
		cout << "\nThis program strips EU4 save files of the following:\n";
		string d = "";
		d += "\n1 - Invalid Tags";
		d += "\n\tdeletes nations that have had all cores removed and have no means of";
		d += "\n\treemerging.  also removes these tags from trade demands, province";
		d += "\n\tdiscovery, and certain related country switches";
		d += "\n2 - Concluded Wars (disabled by default)";
		d += "\n\tremoves the records of all wars that are no longer active";
		d += "\n3 - Province History";
		d += "\n\tremoves records of changes to province tax, production,";
		d += "\n\tand manpower over time (a great deal of spam in MEIOU)";
		d += "\n4 - Decision History";
		d += "\n\tremoves records of spam decisions (e.g. converting heretic minority)";
		d += "\n\tas well as estate interactions from over 10 years prior";
		d += "\n5 - Advisor History";
		d += "\n\tremoves records of deceased advisors";
		d += "\n6 - Building History";
		d += "\n\tremoves province records of building history";
		d += "\n7 - Occupation History";
		d += "\n\tremoves province records of changing occupations";
		d += "\n8 - Province History #2";
		d += "\n\tremoves province records of changes to religion, culture,";
		d += "\n\ttrade goods, native interactions, unrest, and province flags";
		d += "\n9 - Ruler History";
		d += "\n\tremoves records of deceased rulers";
		d += "\n10 - Heir/Consort History";
		d += "\n\tremoves records of deceased heirs & consorts";
		d += "\n11 - Leader History";
		d += "\n\tremoves records of deceased generals & admirals";
		d += "\n\n";
		cout << d;
		ifstream cfg("EU Clean.cfg");
		if(cfg) {
			string s;
			vector<string> cfglines;
			while(getline(cfg,s)) {cfglines.push_back(s);}
			cfg.close();
			if(cfglines.size() == 12){
				if(cfglines[0] != "0") { verbose					= true;  }
				if(cfglines[1] == "0") { clean_invalid_tags			= false; }
				if(cfglines[2] != "0") { clean_wars					= true;  }
				if(cfglines[3] == "0") { clean_province_history		= false; }
				if(cfglines[4] == "0") { clean_decisions			= false; }
				if(cfglines[5] == "0") { clean_advisors				= false; }
				if(cfglines[6] == "0") { clean_buildings			= false; }
				if(cfglines[7] == "0") { clean_occupations			= false; }
				if(cfglines[8] == "0") { clean_province_history2	= false; }
				if(cfglines[9] != "0") { clean_rulers				= true;  }
				if(cfglines[10] == "0"){ clean_heirs				= false; }
				if(cfglines[11] == "0"){ clean_leaders				= false; }
			}
			else {cout << "\nwarning: ignoring corrupted configuration file \"EU Clean.cfg\"";}
		}

		string input;
		string msg = "";
		while(1){
			cout << "\nverbose mode is currently: ";
			if(verbose){	cout << "on";}
			else{			cout << "off";}
			if(clean_invalid_tags||clean_wars||clean_province_history||clean_decisions||clean_advisors||clean_buildings||clean_occupations||clean_province_history2||clean_rulers||clean_heirs||clean_leaders){
											cout << "\n\nwill be cleaned:";
			}
			if(clean_invalid_tags)			{cout << "\n\t1 - Invalid Tags";}
			if(clean_wars)					{cout << "\n\t2 - Concluded Wars";}
			if(clean_province_history)		{cout << "\n\t3 - Province History";}
			if(clean_decisions)				{cout << "\n\t4 - Decision History";}
			if(clean_advisors)				{cout << "\n\t5 - Advisor History";}
			if(clean_buildings)				{cout << "\n\t6 - Building History";}
			if(clean_occupations)			{cout << "\n\t7 - Occupation History";}
			if(clean_province_history2)		{cout << "\n\t8 - Province History #2";}
			if(clean_rulers)				{cout << "\n\t9 - Ruler History";}
			if(clean_heirs)					{cout << "\n\t10 - Heir/Consort History";}
			if(clean_leaders)				{cout << "\n\t11 - Leader History";}
			if(!clean_invalid_tags||!clean_wars||!clean_province_history||!clean_decisions||!clean_advisors||!clean_buildings||!clean_occupations||!clean_province_history2||!clean_rulers||!clean_heirs||!clean_leaders){
											cout << "\nwill NOT be cleaned:";
			}
			if(!clean_invalid_tags)			{cout << "\n\t1 - Invalid Tags";}
			if(!clean_wars)					{cout << "\n\t2 - Concluded Wars";}
			if(!clean_province_history)		{cout << "\n\t3 - Province History";}
			if(!clean_decisions)			{cout << "\n\t4 - Decision History";}
			if(!clean_advisors)				{cout << "\n\t5 - Advisor History";}
			if(!clean_buildings)			{cout << "\n\t6 - Building History";}
			if(!clean_occupations)			{cout << "\n\t7 - Occupation History";}
			if(!clean_province_history2)	{cout << "\n\t8 - Province History #2";}
			if(!clean_rulers)				{cout << "\n\t9 - Ruler History";}
			if(!clean_heirs)				{cout << "\n\t10 - Heir/Consort History";}
			if(!clean_leaders)				{cout << "\n\t11 - Leader History";}
			cout << "\n\nType a number and press Enter to toggle cleaning on/off.";
			cout << "\nType 'v' to toggle verbose mode on/off.";
			cout << "\nType 'd' to repeat the descriptions.";
			cout << "\nType 'a' and press Enter to clean \"autosave.eu4\"";
			cout << "\nType any other uncompressed EU4 savefile name and press Enter to clean it.";
			cout << "\n\n";
			if(msg != ""){cout << msg << "\n\n";}
			cout << ">";
			cin >> input;
			msg = "";
			if(input == "1"){
				if(clean_invalid_tags){		clean_invalid_tags = false;			msg = "Disabling";}
				else{						clean_invalid_tags = true;			msg = "Enabling";}
				msg += " cleanup of invalid tags";
			}
			else if(input == "2"){
				if(clean_wars){				clean_wars = false;					msg = "Disabling";}
				else{						clean_wars = true;					msg = "Enabling";}
				msg += " cleanup of concluded wars";
			}
			else if(input == "3"){
				if(clean_province_history){	clean_province_history = false;		msg = "Disabling";}
				else{						clean_province_history = true;		msg = "Enabling";}
				msg += " cleanup of province history";
			}
			else if(input == "4"){
				if(clean_decisions){		clean_decisions = false;			msg = "Disabling";}
				else{						clean_decisions = true;				msg = "Enabling";}
				msg += " cleanup of decision history";
			}
			else if(input == "5"){
				if(clean_advisors){			clean_advisors = false;				msg = "Disabling";}
				else{						clean_advisors = true;				msg = "Enabling";}
				msg += " cleanup of advisor history";
			}
			else if(input == "6"){
				if(clean_buildings){		clean_buildings = false;			msg = "Disabling";}
				else{						clean_buildings = true;				msg = "Enabling";}
				msg += " cleanup of building history";
			}
			else if(input == "7"){
				if(clean_occupations){		clean_occupations = false;			msg = "Disabling";}
				else{						clean_occupations = true;			msg = "Enabling";}
				msg += " cleanup of occupation history";
			}
			else if(input == "8"){
				if(clean_province_history2){clean_province_history2 = false;	msg = "Disabling";}
				else{						clean_province_history2 = true;		msg = "Enabling";}
				msg += " cleanup of province history #2";
			}
			else if(input == "9"){
				if(clean_rulers){			clean_rulers = false;				msg = "Disabling";}
				else{						clean_rulers = true;				msg = "Enabling";}
				msg += " cleanup of ruler history";
			}
			else if(input == "10"){
				if(clean_heirs){			clean_heirs = false;				msg = "Disabling";}
				else{						clean_heirs = true;					msg = "Enabling";}
				msg += " cleanup of heir/consort history";
			}
			else if(input == "11"){
				if(clean_leaders){			clean_leaders = false;				msg = "Disabling";}
				else{						clean_leaders = true;				msg = "Enabling";}
				msg += " cleanup of leader history";
			}
			else if(input == "v"){
				if(verbose){				verbose = false;					msg = "Disabling";}
				else{						verbose = true;						msg = "Enabling";}
				msg += " verbose output";
			}
			else if(input == "d"){
				msg = d;
			}
			else if(!(clean_invalid_tags||clean_wars||clean_province_history||clean_decisions||clean_advisors||clean_buildings||clean_occupations||clean_province_history2||clean_rulers||clean_heirs||clean_leaders)){
				msg = "Error: no cleanup options selected.";
			}
			else{
				string base_file;
				string infileloc;
				if(input == "a"){ infileloc = "autosave.eu4"; }
				else			{ infileloc = input; }
				if(infileloc.substr(infileloc.length()-4) == ".eu4"){	base_file = infileloc.substr(0,infileloc.length()-4);}
				else{													base_file = infileloc; infileloc += ".eu4";}
				outfileloc = base_file + "_cleaned.eu4";
				ifstream infile(infileloc.c_str());
				if(infile) {
					cout << "Importing savefile: " << infileloc << "\n";
					string s;
					while(getline(infile,s)) {savelines.push_back(s);}
					infile.close();
					break;
				}
				else{msg = "Error: unable to open file: " + infileloc;}
			}
		}

		ofstream _c("EU Clean.cfg");
		if(verbose)					{_c << "1";} else{_c << "0";}; _c << "\n";
		if(clean_invalid_tags)		{_c << "1";} else{_c << "0";}; _c << "\n";
		if(clean_wars)				{_c << "1";} else{_c << "0";}; _c << "\n";
		if(clean_province_history)	{_c << "1";} else{_c << "0";}; _c << "\n";
		if(clean_decisions)			{_c << "1";} else{_c << "0";}; _c << "\n";
		if(clean_advisors)			{_c << "1";} else{_c << "0";}; _c << "\n";
		if(clean_buildings)			{_c << "1";} else{_c << "0";}; _c << "\n";
		if(clean_occupations)		{_c << "1";} else{_c << "0";}; _c << "\n";
		if(clean_province_history2)	{_c << "1";} else{_c << "0";}; _c << "\n";
		if(clean_rulers)			{_c << "1";} else{_c << "0";}; _c << "\n";
		if(clean_heirs)				{_c << "1";} else{_c << "0";}; _c << "\n";
		if(clean_leaders)			{_c << "1";} else{_c << "0";}; _c << "\n";
		_c.close();
	}

	int lines_removed_1 = 0;
	int lines_removed_2 = 0;
	int lines_removed_3 = 0;
	int lines_removed_4 = 0;
	int lines_removed_5 = 0;
	int lines_removed_6 = 0;
	int lines_removed_7 = 0;
	int lines_removed_8 = 0;
	int lines_removed_9 = 0;
	int lines_removed_10 = 0;
	int lines_removed_11 = 0;
	int lines_removed_12 = 0;
	int lines_removed_13 = 0;
	int references_removed_1 = 0;
	int savelength = savelines.size();

	if(clean_invalid_tags){
		cout << "Checking provinces for valid tags.\n";
		bool get_cores = false;
		set<string> valid_tags;
		for(vector<string>::const_iterator i = savelines.begin(); i != savelines.end(); ++i) {
			string line = *i;
			if(get_cores){
				string cores = line.substr(3);
				while (cores.length() > 2) {
					string core = cores.substr(0,3);
					if(core.substr(0,1) == "\""){core = cores.substr(1,3);	cores = cores.substr(5);}
					else{													cores = cores.substr(4);}
					if(valid_tags.find(core) == valid_tags.end()) {valid_tags.insert(core);}
				}
				get_cores = false;
			}
			else if(line == "\t\tcores={") {get_cores = true;}
		}

		cout << "Checking rebel factions for valid tags.\n";
		for(vector<string>::const_iterator i = savelines.begin(); i != savelines.end(); ++i) {
			string line = *i;
			if(line.substr(0,14) == "\tindependence="){
				string tag = line.substr(15,3);
				if(valid_tags.find(tag) == valid_tags.end()) {valid_tags.insert(tag);}
			}
		}

		cout << "Identified " << valid_tags.size() << " valid tags.\n";

		if(verbose){
			cout << "Valid tags:\n";
			for(set<string>::const_iterator i = valid_tags.begin(); i != valid_tags.end(); ++i) {cout << *i << "\t";}
			cout << "\n";
		}

		cout << "Searching for removed cores.\n";
		set<string> removed_cores;
		for(vector<string>::const_iterator i = savelines.begin(); i != savelines.end(); ++i){
			string line = *i;
			if(line.substr(0,17) == "\t\t\t\tremove_core=\""){
				string tag = line.substr(17,3);
				if(removed_cores.find(tag) != removed_cores.end())	{continue;}
				removed_cores.insert(tag);
			}
		}

		if(verbose){
			cout << "Removed cores:\n";
			for(set<string>::const_iterator i = removed_cores.begin(); i != removed_cores.end(); ++i) {cout << *i << "\t";}
			cout << "\n";
		}

		string tmp[] = {"AKK","AST","BSH","CEN","CLI","CMN","CNG","CRI","CSE","CTA","CYN","DAA","DSH","JIN","KAZ","KHI","KZH","LNG","MNG"
						"MOL","NOG","QAR","QII","QIN","QNG","SHU","SHY","SIB","SNG","TIA","TNG","WUU","XIA","XNG","YUA","YUE","ZHE","ZOU"};
		set<string> exceptions(tmp, tmp+sizeof(tmp)/sizeof(tmp[0]));

		cout << "Searching for invalid tags.\n";
		set<string> invalid_tags;
		for(vector<string>::const_iterator i = savelines.begin(); i != savelines.end(); ++i) {
			string line = *i;
			if(line.length() > 4 && line.substr(0,1) == "\t" && line.substr(1,1) != "\t" && line.substr(4) == "={"){
				string tag = line.substr(1,3);
				if(valid_tags.find(tag) != valid_tags.end())		{continue;}
				if(exceptions.find(tag) != exceptions.end())		{continue;}
				if(removed_cores.find(tag) == removed_cores.end())	{continue;}
				if(isdigit(tag.at(1)))								{continue;}
				invalid_tags.insert(tag);
			}
		}
		cout << "Identified " << invalid_tags.size() << " invalid tags.\n";

		if(verbose){
			cout << "Invalid tags:\n";
			for(set<string>::const_iterator i = invalid_tags.begin(); i != invalid_tags.end(); ++i) {cout << *i << "\t";}
			cout << "\n";
		}

		cout << "Purging invalid tags.\n";
		vector<string> purged_save;
		bool purge_nation = false;
		bool purge_demand = false;
		for(vector<string>::const_iterator i = savelines.begin(); i != savelines.end(); ++i) {
			string line = *i;
			if(purge_nation){
				if(line.substr(0,2) == "\t}"){
					purge_nation = false;
				}
				lines_removed_1++;	
				continue;
			}
			else if(line.length() > 4 && line.substr(0,1) == "\t" && line.substr(1,1) != "\t" && line.substr(4) == "={"){
				string tag = line.substr(1,3);
				if(invalid_tags.find(tag) != invalid_tags.end()){
					purge_nation = true;
					lines_removed_1++;
					continue;
				}
			}	
			if(purge_demand){
				if(line.substr(0,3) == "\t\t}"){
					purge_demand = false;
				}
				lines_removed_2++;	
				continue;
			}
			else if(line.length() > 5 && line.substr(0,2) == "\t\t" && line.substr(2,1) != "\t" && line.substr(5) == "={"){
				string tag = line.substr(2,3);
				if(invalid_tags.find(tag) != invalid_tags.end()){
					purge_demand = true;
					lines_removed_2++;
					continue;
				}
			}
			if(line.substr(0,19) == "\t\t\twinning_against_"){
				string tag = line.substr(19,3);
				if(invalid_tags.find(tag) != invalid_tags.end()){
					lines_removed_3++;
					continue;
				}
			}
			else if(line.substr(0,17) == "\t\tdiscovered_by={"){
				for(set<string>::const_iterator j = invalid_tags.begin(); j != invalid_tags.end(); ++j) {
					int k = line.find(*j);
					if(k != std::string::npos) {
						line.erase(k,4);
						references_removed_1++;
					}
				}
			}
			purged_save.push_back(line);
		}
		savelines = purged_save;
	}

	if(clean_wars){
		cout << "Purging concluded wars: ";
		int removed = 0;
		bool purge_war = false;
		vector<string> purged_save;
		for(vector<string>::const_iterator i = savelines.begin(); i != savelines.end(); ++i) {
			string line = *i;
			if(purge_war){
				if(line.substr(0,1) == "}"){purge_war = false;}
				lines_removed_4++;	
				continue;
			}
			else if(line.length() == 14 && line.substr(0,14) == "previous_war={"){
				purge_war = true;
				removed++;
				lines_removed_4++;
				continue;
			}	
			purged_save.push_back(line);
		}
		savelines = purged_save;
		cout << removed << " deleted\n";
	}

	if(clean_province_history){
		cout << "Consolidating province histories: ";
		int removed = 0;
		vector<string> purged_save;
		for(vector<string>::const_iterator i = savelines.begin(); i != savelines.end(); ++i) {
			string line = *i;
			bool strip_line = false;
			if(line.substr(0,4) == "\t\t\t\t"){
				if(line.substr(0,13) == "\t\t\t\tbase_tax=")				{strip_line = true;}
				else if(line.substr(0,18) == "\t\t\t\tbase_manpower=")		{strip_line = true;}
				else if(line.substr(0,18) == "\t\t\t\tdiscovered_by=")		{strip_line = true;}
				else if(line.substr(0,20) == "\t\t\t\tbase_production=")	{strip_line = true;}
				if(strip_line){
					removed++;
					string prev_line = purged_save.back();
					i++;
					string next_line = *i;
					i--;
					if(prev_line.substr(0,3) == "\t\t\t" && isdigit(prev_line[3]) && next_line == "\t\t\t}"){
						purged_save.pop_back();
						lines_removed_5 += 3;
						i++;
						continue;
					}
					else{
						lines_removed_5++;
						continue;
					}
				}
			}
			purged_save.push_back(line);
		}
		savelines = purged_save;
		cout << removed << " deleted\n";
	}

	if(clean_province_history2){
		cout << "Consolidating province histories #2: ";
		int removed = 0;
		vector<string> purged_save;
		for(vector<string>::const_iterator i = savelines.begin(); i != savelines.end(); ++i) {
			string line = *i;
			if(line.substr(0,4) == "\t\t\t\t"){
				bool strip_line = false;
				if(line == "\t\t\t\tis_city==yes")							{strip_line = true;}
				else if(line.substr(0,11) == "\t\t\t\tunrest=")				{strip_line = true;}
				else if(line.substr(0,12) == "\t\t\t\tculture=")			{strip_line = true;}
				else if(line.substr(0,13) == "\t\t\t\treligion=")			{strip_line = true;}
				else if(line.substr(0,16) == "\t\t\t\ttrade_goods=")		{strip_line = true;}
				else if(line.substr(0,16) == "\t\t\t\tnative_size=")		{strip_line = true;}
				else if(line.substr(0,20) == "\t\t\t\tnative_ferocity=")	{strip_line = true;}
				else if(line.substr(0,22) == "\t\t\t\tset_province_flag=")	{strip_line = true;}
				else if(line.substr(0,23) == "\t\t\t\tnative_hostileness=")	{strip_line = true;}
				if(strip_line){
					removed++;
					string prev_line = purged_save.back();
					i++;
					string next_line = *i;
					i--;
					if(prev_line.substr(0,3) == "\t\t\t" && isdigit(prev_line[3]) && next_line == "\t\t\t}"){
						purged_save.pop_back();
						lines_removed_10 += 3;
						i++;
						continue;
					}
					else{
						lines_removed_10++;
						continue;
					}
				}
			}
			purged_save.push_back(line);
		}
		savelines = purged_save;
		cout << removed << " deleted\n";
	}

	if(clean_decisions){
		cout << "Removing decision spam records: ";
		int removed = 0;
		int current_year = atoi(((savelines.at(1)).substr(5,9)).c_str());
		vector<string> purged_save;
		vector<string> to_strip;
		to_strip.push_back("\t\t\t\tdecision=\"attract_persian_talent\"");
		to_strip.push_back("\t\t\t\tdecision=\"buy_curia_influence\"");
		to_strip.push_back("\t\t\t\tdecision=\"caliph_investiture\"");
		to_strip.push_back("\t\t\t\tdecision=\"cardinal_policy\"");
		to_strip.push_back("\t\t\t\tdecision=\"change_pronvince_name_to_german\"");
		to_strip.push_back("\t\t\t\tdecision=\"commission_of_cardinals\"");
		to_strip.push_back("\t\t\t\tdecision=\"consult_great_seer\"");
		to_strip.push_back("\t\t\t\tdecision=\"convert_heretic_minority\"");
		to_strip.push_back("\t\t\t\tdecision=\"endow_churches\"");
		to_strip.push_back("\t\t\t\tdecision=\"expand_royal_elephant_stables_oriental_mod\"");
		to_strip.push_back("\t\t\t\tdecision=\"foreign_missionary_neighbor\"");
		to_strip.push_back("\t\t\t\tdecision=\"foreign_missionary_primitive\"");
		to_strip.push_back("\t\t\t\tdecision=\"foreign_missionary_vassal\"");
		to_strip.push_back("\t\t\t\tdecision=\"genoa_sicilian_trade_privileges\"");
		to_strip.push_back("\t\t\t\tdecision=\"grant_peerage\"");
		to_strip.push_back("\t\t\t\tdecision=\"haram_oriental_mod_ai\"");
		to_strip.push_back("\t\t\t\tdecision=\"heretic_expulsion\"");
		to_strip.push_back("\t\t\t\tdecision=\"hold_naadam\"");
		to_strip.push_back("\t\t\t\tdecision=\"introduce_heir_oriental_mod\"");
		to_strip.push_back("\t\t\t\tdecision=\"irregular_imperial_examinations\"");
		to_strip.push_back("\t\t\t\tdecision=\"last_pagans_in_europe_orthodox\"");
		to_strip.push_back("\t\t\t\tdecision=\"policy_of_intolerance\"");
		to_strip.push_back("\t\t\t\tdecision=\"policy_of_tolerance\"");
		to_strip.push_back("\t\t\t\tdecision=\"reaffirm_emperorship\"");
		to_strip.push_back("\t\t\t\tdecision=\"request_clerical_advisor\"");
		to_strip.push_back("\t\t\t\tdecision=\"request_dispensation\"");
		to_strip.push_back("\t\t\t\tdecision=\"request_kanrei_appointement\"");
		to_strip.push_back("\t\t\t\tdecision=\"request_kyushu_tandai_appointement\"");
		to_strip.push_back("\t\t\t\tdecision=\"request_shikoku_tandai_appointement\"");
		to_strip.push_back("\t\t\t\tdecision=\"request_shugo_appointement\"");
		to_strip.push_back("\t\t\t\tdecision=\"seek_elders_blessing\"");
		to_strip.push_back("\t\t\t\tdecision=\"select_policy_partners\"");
		to_strip.push_back("\t\t\t\tdecision=\"select_policy_vassals\"");
		to_strip.push_back("\t\t\t\tdecision=\"shinto_pilgrimage\"");
		to_strip.push_back("\t\t\t\tdecision=\"show_deigratia_help\"");
		to_strip.push_back("\t\t\t\tdecision=\"subject_integration\"");
		to_strip.push_back("\t\t\t\tdecision=\"subject_subjugation\"");
		to_strip.push_back("\t\t\t\tdecision=\"weighing_ceremony_oriental_mod\"");
		for(vector<string>::const_iterator i = savelines.begin(); i != savelines.end(); ++i) {
			string line = *i;
			if(line.substr(0,4) == "\t\t\t\t"){
				if(find(to_strip.begin(),to_strip.end(),line) != to_strip.end()){
					removed++;
					string prev_line = purged_save.back();
					i++;
					string next_line = *i;
					i--;
					if(prev_line.substr(0,3) == "\t\t\t" && isdigit(prev_line[3]) && next_line == "\t\t\t}"){
						purged_save.pop_back();
						lines_removed_6 += 3;
						i++;
						continue;
					}
					else{
						lines_removed_6++;
						continue;
					}
				}
			}
			else if(line == "\t\t\tinteraction_use={"){
				i += 2;
				int test_year = atoi(((*i).substr(9,13)).c_str());
				i -= 2;
				if(current_year > test_year + 10){
					lines_removed_6 += 4;
					removed++;
					i += 3;
					continue;
				}
			}
			purged_save.push_back(line);
		}
		savelines = purged_save;
		cout << removed << " deleted\n";
	}

	if(clean_buildings){
		cout << "Removing building history records: ";

		int removed = 0;
		vector<string> purged_save;
		vector<string> to_strip;

		to_strip.push_back("\t\t\t\tnative_ceremonial_fire_pit=yes");
		to_strip.push_back("\t\t\t\tnative_earthwork=yes");
		to_strip.push_back("\t\t\t\tnative_fortified_house=yes");
		to_strip.push_back("\t\t\t\tnative_great_trail=yes");
		to_strip.push_back("\t\t\t\tnative_irrigation=yes");
		to_strip.push_back("\t\t\t\tnative_longhouse=yes");
		to_strip.push_back("\t\t\t\tnative_storehouse=yes");
		to_strip.push_back("\t\t\t\tnative_sweat_lodge=yes");
		to_strip.push_back("\t\t\t\tnative_three_sisters_field=yes");

		to_strip.push_back("\t\t\t\tbureaucracy_1=yes");
		to_strip.push_back("\t\t\t\tbureaucracy_1=no");
		to_strip.push_back("\t\t\t\tbureaucracy_2=yes");
		to_strip.push_back("\t\t\t\tbureaucracy_2=no");
		to_strip.push_back("\t\t\t\tbureaucracy_3=yes");
		to_strip.push_back("\t\t\t\tbureaucracy_3=no");
		to_strip.push_back("\t\t\t\tbureaucracy_4=yes");
		to_strip.push_back("\t\t\t\tbureaucracy_4=no");
		to_strip.push_back("\t\t\t\tbureaucracy_5=yes");
		to_strip.push_back("\t\t\t\tbureaucracy_5=no");

		to_strip.push_back("\t\t\t\ttemple=yes");
		to_strip.push_back("\t\t\t\ttemple=no");
		to_strip.push_back("\t\t\t\ttemple_heathen=yes");
		to_strip.push_back("\t\t\t\ttemple_heathen=no");
		to_strip.push_back("\t\t\t\tgreat_temple=yes");
		to_strip.push_back("\t\t\t\tgreat_temple=no");

		to_strip.push_back("\t\t\t\tsmall_university=yes");
		to_strip.push_back("\t\t\t\tsmall_university=no");
		to_strip.push_back("\t\t\t\tmedium_university=yes");
		to_strip.push_back("\t\t\t\tmedium_university=no");
		to_strip.push_back("\t\t\t\tbig_university=yes");
		to_strip.push_back("\t\t\t\tbig_university=no");

		to_strip.push_back("\t\t\t\tmarketplace=yes");
		to_strip.push_back("\t\t\t\tmarketplace=no");
		to_strip.push_back("\t\t\t\tmerchant_guild=yes");
		to_strip.push_back("\t\t\t\tmerchant_guild=no");
		to_strip.push_back("\t\t\t\ttrade_depot=yes");
		to_strip.push_back("\t\t\t\ttrade_depot=no");
		to_strip.push_back("\t\t\t\tcustoms_house=yes");
		to_strip.push_back("\t\t\t\tcustoms_house=no");

		to_strip.push_back("\t\t\t\tbank=yes");
		to_strip.push_back("\t\t\t\tbank=no");
		to_strip.push_back("\t\t\t\tstock_exchange=yes");
		to_strip.push_back("\t\t\t\tstock_exchange=no");

		to_strip.push_back("\t\t\t\twarehouse=yes");
		to_strip.push_back("\t\t\t\twarehouse=no");
		to_strip.push_back("\t\t\t\twarehouse_district=yes");
		to_strip.push_back("\t\t\t\twarehouse_district=no");

		to_strip.push_back("\t\t\t\tworkshop=yes");
		to_strip.push_back("\t\t\t\tworkshop=no");
		to_strip.push_back("\t\t\t\tcorporation_guild=yes");
		to_strip.push_back("\t\t\t\tcorporation_guild=no");
		to_strip.push_back("\t\t\t\turban_manufactory=yes");
		to_strip.push_back("\t\t\t\turban_manufactory=no");
		to_strip.push_back("\t\t\t\tfactory=yes");
		to_strip.push_back("\t\t\t\tfactory=no");
		to_strip.push_back("\t\t\t\tsteam_powered_factory=yes");
		to_strip.push_back("\t\t\t\tsteam_powered_factory=no");

		to_strip.push_back("\t\t\t\tart_corporation=yes");
		to_strip.push_back("\t\t\t\tart_corporation=no");
		to_strip.push_back("\t\t\t\tfine_arts_academy=yes");
		to_strip.push_back("\t\t\t\tfine_arts_academy=no");

		to_strip.push_back("\t\t\t\tfort_14th=yes");
		to_strip.push_back("\t\t\t\tfort_14th=no");
		to_strip.push_back("\t\t\t\tfort_15th=yes");
		to_strip.push_back("\t\t\t\tfort_15th=no");
		to_strip.push_back("\t\t\t\tfort_16th=yes");
		to_strip.push_back("\t\t\t\tfort_16th=no");
		to_strip.push_back("\t\t\t\tfort_17th=yes");
		to_strip.push_back("\t\t\t\tfort_17th=no");
		to_strip.push_back("\t\t\t\tfort_18th=yes");
		to_strip.push_back("\t\t\t\tfort_18th=no");

		to_strip.push_back("\t\t\t\tlocal_fortification_1=yes");
		to_strip.push_back("\t\t\t\tlocal_fortification_1=no");
		to_strip.push_back("\t\t\t\tlocal_fortification_1_off=yes");
		to_strip.push_back("\t\t\t\tlocal_fortification_1_off=no");
		to_strip.push_back("\t\t\t\tlocal_fortification_2=yes");
		to_strip.push_back("\t\t\t\tlocal_fortification_2=no");
		to_strip.push_back("\t\t\t\tlocal_fortification_2_off=yes");
		to_strip.push_back("\t\t\t\tlocal_fortification_2_off=no");
		to_strip.push_back("\t\t\t\tlocal_fortification_3=yes");
		to_strip.push_back("\t\t\t\tlocal_fortification_3=no");
		to_strip.push_back("\t\t\t\tlocal_fortification_3_off=yes");
		to_strip.push_back("\t\t\t\tlocal_fortification_3_off=no");

		to_strip.push_back("\t\t\t\ttrading_post=yes");
		to_strip.push_back("\t\t\t\ttrading_company=yes");

		to_strip.push_back("\t\t\t\tmines_1=yes");
		to_strip.push_back("\t\t\t\tmines_1=no");
		to_strip.push_back("\t\t\t\tmines_2=yes");
		to_strip.push_back("\t\t\t\tmines_2=no");
		to_strip.push_back("\t\t\t\tmines_3=yes");
		to_strip.push_back("\t\t\t\tmines_3=no");
		to_strip.push_back("\t\t\t\tmines_4=yes");
		to_strip.push_back("\t\t\t\tmines_4=no");
		to_strip.push_back("\t\t\t\tmines_5=yes");
		to_strip.push_back("\t\t\t\tmines_5=no");

		to_strip.push_back("\t\t\t\trural_infrastructure_1=yes");
		to_strip.push_back("\t\t\t\trural_infrastructure_1=no");
		to_strip.push_back("\t\t\t\trural_infrastructure_2=yes");
		to_strip.push_back("\t\t\t\trural_infrastructure_2=no");
		to_strip.push_back("\t\t\t\trural_infrastructure_3=yes");
		to_strip.push_back("\t\t\t\trural_infrastructure_3=no");
		to_strip.push_back("\t\t\t\trural_infrastructure_4=yes");
		to_strip.push_back("\t\t\t\trural_infrastructure_4=no");
		to_strip.push_back("\t\t\t\trural_infrastructure_5=yes");
		to_strip.push_back("\t\t\t\trural_infrastructure_5=no");
		to_strip.push_back("\t\t\t\trural_infrastructure_6=yes");
		to_strip.push_back("\t\t\t\trural_infrastructure_6=no");
		to_strip.push_back("\t\t\t\trural_infrastructure_7=yes");
		to_strip.push_back("\t\t\t\trural_infrastructure_7=no");

		to_strip.push_back("\t\t\t\ttown_hall=yes");
		to_strip.push_back("\t\t\t\ttown_hall=no");
		to_strip.push_back("\t\t\t\turban_infrastructure_1=yes");
		to_strip.push_back("\t\t\t\turban_infrastructure_2=yes");
		to_strip.push_back("\t\t\t\turban_infrastructure_3=yes");
		to_strip.push_back("\t\t\t\turban_infrastructure_4=yes");
		to_strip.push_back("\t\t\t\turban_infrastructure_5=yes");

		to_strip.push_back("\t\t\t\tharbour_infrastructure_1=yes");
		to_strip.push_back("\t\t\t\tharbour_infrastructure_1=no");
		to_strip.push_back("\t\t\t\tharbour_infrastructure_2=yes");
		to_strip.push_back("\t\t\t\tharbour_infrastructure_2=no");
		to_strip.push_back("\t\t\t\tharbour_infrastructure_3=yes");
		to_strip.push_back("\t\t\t\tharbour_infrastructure_3=no");
		to_strip.push_back("\t\t\t\tharbour_infrastructure_4=yes");
		to_strip.push_back("\t\t\t\tharbour_infrastructure_4=no");
		to_strip.push_back("\t\t\t\tharbour_infrastructure_5=yes");
		to_strip.push_back("\t\t\t\tharbour_infrastructure_5=no");

		to_strip.push_back("\t\t\t\tmilitary_harbour_1=yes");
		to_strip.push_back("\t\t\t\tmilitary_harbour_1=no");
		to_strip.push_back("\t\t\t\tmilitary_harbour_2=yes");
		to_strip.push_back("\t\t\t\tmilitary_harbour_2=no");
		to_strip.push_back("\t\t\t\tmilitary_harbour_3=yes");
		to_strip.push_back("\t\t\t\tmilitary_harbour_3=no");
		to_strip.push_back("\t\t\t\tmilitary_harbour_4=yes");
		to_strip.push_back("\t\t\t\tmilitary_harbour_4=no");

		to_strip.push_back("\t\t\t\troad_network=yes");
		to_strip.push_back("\t\t\t\troad_network=no");
		to_strip.push_back("\t\t\t\tpaved_road_network=yes");
		to_strip.push_back("\t\t\t\tpaved_road_network=no");
		to_strip.push_back("\t\t\t\thighway_network=yes");
		to_strip.push_back("\t\t\t\thighway_network=no");
		to_strip.push_back("\t\t\t\troad_and_rail_network=yes");
		to_strip.push_back("\t\t\t\troad_and_rail_network=no");

		to_strip.push_back("\t\t\t\tnational_capital_1=yes");
		to_strip.push_back("\t\t\t\tnational_capital_1=no");
		to_strip.push_back("\t\t\t\tnational_capital_2=yes");
		to_strip.push_back("\t\t\t\tnational_capital_2=no");
		to_strip.push_back("\t\t\t\tnational_capital_3=yes");
		to_strip.push_back("\t\t\t\tnational_capital_3=no");
		to_strip.push_back("\t\t\t\tnational_capital_4=yes");
		to_strip.push_back("\t\t\t\tnational_capital_4=no");
		to_strip.push_back("\t\t\t\tnational_capital_5=yes");
		to_strip.push_back("\t\t\t\tnational_capital_5=no");

		for(vector<string>::const_iterator i = savelines.begin(); i != savelines.end(); ++i) {
			string line = *i;
			if(line.substr(0,4) == "\t\t\t\t"){
				if(find(to_strip.begin(),to_strip.end(),line) != to_strip.end()){
					string prev_line = purged_save.back();
					i++;
					string next_line = *i;
					i--;
					removed++;
					if(prev_line.substr(0,3) == "\t\t\t" && isdigit(prev_line[3]) && next_line == "\t\t\t}"){
						purged_save.pop_back();
						lines_removed_8 += 3;
						i++;
						continue;
					}
					else{
						lines_removed_8++;
						continue;
					}
				}
			}
			purged_save.push_back(line);
		}
		savelines = purged_save;
		cout << removed << " deleted\n";
	}

	if(clean_occupations){
		cout << "Purging occupation history: ";
		int removed = 0;
		vector<string> purged_save;
		for(vector<string>::const_iterator i = savelines.begin(); i != savelines.end(); ++i) {
			string line = *i;
			if(line == "\t\t\t\tcontroller={"){
				string prev_line = purged_save.back();
				i += 2;
				string test1 = *i;
				i++;
				string test2 = *i;
				i++;
				string test3 = *i;
				i -= 4;
				if(prev_line.substr(0,3) == "\t\t\t" && isdigit(prev_line[3])){
					if(test1 == "\t\t\t\t}" && test2 == "\t\t\t}"){
						purged_save.pop_back();
						lines_removed_9 += 5;
						removed++;
						i += 3;
						continue;
					}
					if(test2 == "\t\t\t\t}" && test3 == "\t\t\t}"){
						purged_save.pop_back();
						lines_removed_9 += 6;
						removed++;
						i += 4;
						continue;
					}
				} 
				if(test1  == "\t\t\t\t}"){
					lines_removed_9 += 3;
					removed++;
					i += 2;
					continue;
				}
				if(test2  == "\t\t\t\t}"){
					lines_removed_9 += 4;
					removed++;
					i += 3;
					continue;
				}
			}
			else{
				bool strip_line = false;				
				if(line.substr(0,15) == "\t\t\t\tfake_owner=")	{strip_line = true;}
				if(strip_line){
					removed++;
					string prev_line = purged_save.back();
					i++;
					string next_line = *i;
					i--;
					if(prev_line.substr(0,3) == "\t\t\t" && isdigit(prev_line[3]) && next_line == "\t\t\t}"){
						purged_save.pop_back();
						lines_removed_9 += 3;
						i++;
						continue;
					}
					else{
						lines_removed_9++;
						continue;
					}
				}
			}
			purged_save.push_back(line);
		}
		savelines = purged_save;
		cout << removed << " deleted\n";
	}

	if(clean_rulers){
		cout << "Scanning for active rulers: ";
		set<string> valid_rulers;
		Scan_IDs(savelines,valid_rulers,"\t\tmonarch={");
		cout << valid_rulers.size() << " found\n";

		cout << "Purging deceased rulers: ";
		int removed = 0;
		Strip_Deceased(savelines, valid_rulers, "\t\t\t\tmonarch={", removed, lines_removed_11);
		Strip_Deceased(savelines, valid_rulers, "\t\t\t\tmonarch_heir={", removed, lines_removed_11);
		Strip_Deceased(savelines, valid_rulers, "\t\t\t\tmonarch_consort={", removed, lines_removed_11);
		if(removed){
			vector<string> purged_save;
			for(vector<string>::const_iterator i = savelines.begin(); i != savelines.end(); ++i) {
				string line = *i;
				if(line == "\t\tprevious_monarch={"){
					lines_removed_11 += 4;
					i += 3;
					continue;
				}
				purged_save.push_back(line);
			}
			savelines = purged_save;
		}
		cout << removed << " deleted\n";
	}

	if(clean_heirs){
		cout << "Scanning for active heirs: ";
		set<string> valid_heirs;
		Scan_IDs(savelines,valid_heirs,"\t\their={");
		cout << valid_heirs.size() << " found\n";

		cout << "Purging deceased heirs: ";
		int removed = 0;
		Strip_Deceased(savelines, valid_heirs, "\t\t\t\their={", removed, lines_removed_12);
		cout << removed << " deleted\n";

		cout << "Scanning for active consorts: ";
		set<string> valid_consorts;
		Scan_IDs(savelines,valid_consorts,"\t\tqueen={");
		cout << valid_consorts.size() << " found\n";

		cout << "Purging deceased consorts: ";
		removed = 0;
		Strip_Deceased(savelines, valid_consorts, "\t\t\t\tqueen={", removed, lines_removed_12);
		cout << removed << " deleted\n";
	}

	if(clean_advisors){
		cout << "Scanning for active advisors: ";
		set<string> valid_advisors;
		Scan_IDs(savelines,valid_advisors,"\t\tadvisor={");
		cout << valid_advisors.size() << " found\n";

		cout << "Purging deceased advisors: ";
		int removed = 0;
		Strip_Deceased(savelines, valid_advisors, "\t\t\t\tadvisor={", removed, lines_removed_7);
		cout << removed << " deleted\n";
	}

	if(clean_leaders){
		cout << "Scanning for active leaders: ";
		set<string> valid_leaders;
		Scan_IDs(savelines,valid_leaders,"\t\tleader={");
		cout << valid_leaders.size() << " found\n";

		cout << "Purging deceased leaders: ";
		int removed = 0;
		Strip_Deceased(savelines, valid_leaders, "\t\t\t\tleader={", removed, lines_removed_13);
		cout << removed << " deleted\n";
	}

	cout << "Removed:\n";
	if(clean_invalid_tags){
		cout << "\t" << references_removed_1 << "\treferences within province discovery\n";
	}
	if(clean_invalid_tags){
		cout << "\t" << lines_removed_1 << "\t(" << Percent_Removed(lines_removed_1, savelength) << "%) lines of country definition/history\n";
		cout << "\t" << lines_removed_2 << "\t(" << Percent_Removed(lines_removed_2, savelength) << "%) lines of unnecessary trade demands\n";
		cout << "\t" << lines_removed_3 << "\t(" << Percent_Removed(lines_removed_3, savelength) << "%) lines of irrelevant country switches\n";
	}
	if(clean_wars){				cout << "\t" << lines_removed_4  << "\t(" << Percent_Removed(lines_removed_4, savelength)	<< "%) lines of concluded war history\n";}
	if(clean_province_history){	cout << "\t" << lines_removed_5  << "\t(" << Percent_Removed(lines_removed_5, savelength)	<< "%) lines of redundant province history\n";}
	if(clean_province_history2){cout << "\t" << lines_removed_10 << "\t(" << Percent_Removed(lines_removed_10, savelength)	<< "%) lines of additional province history\n";}
	if(clean_occupations){		cout << "\t" << lines_removed_9  << "\t(" << Percent_Removed(lines_removed_9, savelength)	<< "%) lines of occupation history\n";}
	if(clean_buildings){		cout << "\t" << lines_removed_8  << "\t(" << Percent_Removed(lines_removed_8, savelength)	<< "%) lines of building history\n";}
	if(clean_decisions){		cout << "\t" << lines_removed_6  << "\t(" << Percent_Removed(lines_removed_6, savelength)	<< "%) lines of decision spam history\n";}
	if(clean_rulers){			cout << "\t" << lines_removed_11 << "\t(" << Percent_Removed(lines_removed_11, savelength)	<< "%) lines of dead ruler records\n";}
	if(clean_heirs){			cout << "\t" << lines_removed_12 << "\t(" << Percent_Removed(lines_removed_12, savelength)	<< "%) lines of dead heir/consort records\n";}
	if(clean_advisors){			cout << "\t" << lines_removed_7  << "\t(" << Percent_Removed(lines_removed_7, savelength)	<< "%) lines of dead advisor records\n";}
	if(clean_leaders){			cout << "\t" << lines_removed_13 << "\t(" << Percent_Removed(lines_removed_13, savelength)	<< "%) lines of dead leader records\n";}

	int total = 0;
	total += lines_removed_1;
	total += lines_removed_2;
	total += lines_removed_3;
	total += lines_removed_4;
	total += lines_removed_5;
	total += lines_removed_6;
	total += lines_removed_7;
	total += lines_removed_8;
	total += lines_removed_9;
	total += lines_removed_10;
	total += lines_removed_11;
	total += lines_removed_12;
	total += lines_removed_13;
	cout << "\t--------------------------------------------\n";
	cout << "\t" << total << "\t(" << Percent_Removed(total, savelength) << "%) total lines removed\n";
	
	cout << "Writing updated savefile: " << outfileloc;
	ofstream outfile(outfileloc.c_str());
	ostream_iterator<string> output_iterator(outfile, "\n");
	copy(savelines.begin(), savelines.end(), output_iterator);
	outfile.close();

	cout << "\n\nPress Enter to continue.";
	cin.sync(); cin.get();

	return 0;
}
