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

#include "eu_clean_logic.h"
#include "CleanConfig.h"

void print_instructions(const CleanConfig &clean_config)
{
    cout << "\nverbose mode is currently: ";
    if (clean_config.verbose)
    { cout << "on"; }
    else
    { cout << "off"; }
    if (clean_config.clean_invalid_tags || clean_config.clean_wars || clean_config.clean_province_history ||
        clean_config.clean_decisions || clean_config.clean_advisors ||
        clean_config.clean_buildings || clean_config.clean_occupations ||
        clean_config.clean_province_history2 || clean_config.clean_rulers || clean_config.clean_heirs ||
        clean_config.clean_leaders)
    {
        cout << "\n\nwill be cleaned:";
    }
    if (clean_config.clean_invalid_tags) cout << "\n\t1 - Invalid Tags";
    if (clean_config.clean_wars) cout << "\n\t2 - Concluded Wars";
    if (clean_config.clean_province_history) cout << "\n\t3 - Province History";
    if (clean_config.clean_decisions) cout << "\n\t4 - Decision History";
    if (clean_config.clean_advisors) cout << "\n\t5 - Advisor History";
    if (clean_config.clean_buildings) cout << "\n\t6 - Building History";
    if (clean_config.clean_occupations) cout << "\n\t7 - Occupation History";
    if (clean_config.clean_province_history2) cout << "\n\t8 - Province History #2";
    if (clean_config.clean_rulers) cout << "\n\t9 - Ruler History";
    if (clean_config.clean_heirs) cout << "\n\t10 - Heir/Consort History";
    if (clean_config.clean_leaders) cout << "\n\t11 - Leader History";
    if (!clean_config.clean_invalid_tags || !clean_config.clean_wars || !clean_config.clean_province_history ||
        !clean_config.clean_decisions || !clean_config.clean_advisors ||
        !clean_config.clean_buildings || !clean_config.clean_occupations ||
        !clean_config.clean_province_history2 || !clean_config.clean_rulers || !clean_config.clean_heirs ||
        !clean_config.clean_leaders)
    {
        cout << "\nwill NOT be cleaned:";
    }
    if (!clean_config.clean_invalid_tags) cout << "\n\t1 - Invalid Tags";
    if (!clean_config.clean_wars) cout << "\n\t2 - Concluded Wars";
    if (!clean_config.clean_province_history) cout << "\n\t3 - Province History";
    if (!clean_config.clean_decisions) cout << "\n\t4 - Decision History";
    if (!clean_config.clean_advisors) cout << "\n\t5 - Advisor History";
    if (!clean_config.clean_buildings) cout << "\n\t6 - Building History";
    if (!clean_config.clean_occupations) cout << "\n\t7 - Occupation History";
    if (!clean_config.clean_province_history2) cout << "\n\t8 - Province History #2";
    if (!clean_config.clean_rulers) cout << "\n\t9 - Ruler History";
    if (!clean_config.clean_heirs) cout << "\n\t10 - Heir/Consort History";
    if (!clean_config.clean_leaders) cout << "\n\t11 - Leader History";
    cout << "\n\nType a number and press Enter to toggle cleaning on/off.";
    cout << "\nType 'v' to toggle verbose mode on/off.";
    cout << "\nType 'd' to repeat the descriptions.";
    cout << "\nType 'a' and press Enter to clean_ \"autosave.eu4\"";
    cout << "\nType any other uncompressed EU4 savefile name and press Enter to clean_ it.";
    cout << "\n\n";
}

int main()
{
    CleanConfig clean_cfg("EU Clean.cfg");
    string input;
    print_instructions(clean_cfg);
    cin >> input;
    while (!clean(input, clean_cfg)) cin >> input;
    return 0;
}