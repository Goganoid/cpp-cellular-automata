//
// Created by egor0 on 4/26/2020.
//

#ifndef CPP_LIFE_CELLRULE_H
#define CPP_LIFE_CELLRULE_H
#include "Grid.h"
#include <string>
#include <algorithm>
#include <sstream>

class Rule{
private:
  std::vector<int> neighsToBorn;
  std::vector<int> neighsToSurvive;
public:
    explicit Rule(std::string stringRule);
    CellState Execute(Grid& grid, int x, int y);
};

#endif //CPP_LIFE_CELLRULE_H