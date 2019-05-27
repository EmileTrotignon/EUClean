//
// Created by Emile on 20/05/2019.
//

#include <vector>
#include <fstream>
#include "CleanConfig.h"

CleanConfig::CleanConfig()
{
    use_defaults = false;    //for ultra-fast testing purposes
    verbose = false;
    clean_invalid_tags = true;
    clean_wars = false;
    clean_province_history = true;
    clean_decisions = true;
    clean_advisors = true;
    clean_buildings = true;
    clean_occupations = true;
    clean_province_history2 = true;
    clean_rulers = false;
    clean_heirs = true;
    clean_leaders = true;
}

CleanConfig::CleanConfig(const string &cfg_loc)
{
    ifstream cfg(cfg_loc);
    if (cfg)
    {
        string s;
        vector<string> cfglines;
        while (getline(cfg, s))
        { cfglines.push_back(s); }
        cfg.close();
        if (cfglines.size() == 12)
        {
            if (cfglines[0] != "0") verbose = true;
            if (cfglines[1] == "0") clean_invalid_tags = false;
            if (cfglines[2] != "0") clean_wars = true;
            if (cfglines[3] == "0") clean_province_history = false;
            if (cfglines[4] == "0") clean_decisions = false;
            if (cfglines[5] == "0") clean_advisors = false;
            if (cfglines[6] == "0") clean_buildings = false;
            if (cfglines[7] == "0") clean_occupations = false;
            if (cfglines[8] == "0") clean_province_history2 = false;
            if (cfglines[9] != "0") clean_rulers = true;
            if (cfglines[10] == "0") clean_heirs = false;
            if (cfglines[11] == "0") clean_leaders = false;
        } else
        { cout << "\nwarning: ignoring corrupted configuration file \"EU Clean.cfg\""; }
    }

}

void CleanConfig::save_config(const string &cfg_loc)
{
    ofstream _c(cfg_loc);
    if (verbose) _c << "1";
    else _c << "0";
    _c << "\n";
    if (clean_invalid_tags) _c << "1";
    else _c << "0";
    _c << "\n";
    if (clean_wars) _c << "1";
    else _c << "0";
    _c << "\n";
    if (clean_province_history) _c << "1";
    else _c << "0";
    _c << "\n";
    if (clean_decisions) _c << "1";
    else _c << "0";
    _c << "\n";
    if (clean_advisors) _c << "1";
    else _c << "0";
    _c << "\n";
    if (clean_buildings) _c << "1";
    else _c << "0";
    _c << "\n";
    if (clean_occupations) _c << "1";
    else _c << "0";
    _c << "\n";
    if (clean_province_history2) _c << "1";
    else _c << "0";
    _c << "\n";
    if (clean_rulers) _c << "1";
    else _c << "0";
    _c << "\n";
    if (clean_heirs) _c << "1";
    else _c << "0";
    _c << "\n";
    if (clean_leaders) _c << "1";
    else _c << "0";
    _c << "\n";
    _c.close();
}
