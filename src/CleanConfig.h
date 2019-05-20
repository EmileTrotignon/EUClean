//
// Created by Emile on 20/05/2019.
//

#ifndef EUCLEAN_CLEANCONFIG_H
#define EUCLEAN_CLEANCONFIG_H


class CleanConfig {
public:
    CleanConfig();

    bool use_defaults = false;    //for ultra-fast testing purposes
    bool verbose = false;
    bool clean_invalid_tags = true;
    bool clean_wars = false;
    bool clean_province_history = true;
    bool clean_decisions = true;
    bool clean_advisors = true;
    bool clean_buildings = true;
    bool clean_occupations = true;
    bool clean_province_history2 = true;
    bool clean_rulers = false;
    bool clean_heirs = true;
    bool clean_leaders = true;
};


#endif //EUCLEAN_CLEANCONFIG_H
