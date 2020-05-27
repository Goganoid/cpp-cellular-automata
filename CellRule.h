//
// Created by egor0 on 4/26/2020.
//

#ifndef CPP_LIFE_CELLRULE_H
#define CPP_LIFE_CELLRULE_H
#include "Grid.h"
#include <string>
#include <algorithm>
#include <sstream>
#include <map>

class LookupRule{
private:
    std::unordered_map  <int,int> lookupTable;
    std::map<int,bool> neighsToBornTable;
    std::map<int,bool> neighsToSurviveTable;
public:
    [[nodiscard]] int Lookup(int i)  { return lookupTable[i];}
    explicit LookupRule(std::string stringRule);
};


#endif //CPP_LIFE_CELLRULE_H