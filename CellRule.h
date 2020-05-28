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
#include "Cell.h"
class LookupRule{
private:
//    std::unordered_map  <int,short int> lookupTable;
    std::unordered_map  <int,CellBehavior> lookupTable;
    std::map<int,bool> neighsToBornTable;
    std::map<int,bool> neighsToSurviveTable;
public:
    [[nodiscard]] CellBehavior Lookup(int i)  { return lookupTable[i];}
    explicit LookupRule(std::string stringRule);
};


#endif //CPP_LIFE_CELLRULE_H