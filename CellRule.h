//
// Created by egor0 on 4/26/2020.
//

#ifndef CPP_LIFE_CELLRULE_H
#define CPP_LIFE_CELLRULE_H
#include "Grid.h"
#include <string>
#include <algorithm>
#include <sstream>


class LifeRule{
private:

//  int _neighbours;
  Grid * _grid;
public:
    std::vector<int> neighsToBorn;
    std::vector<int> neighsToSurvive;
    explicit LifeRule(std::string stringRule, Grid& grid);
    void Execute(Cell * cell,int  x, int  y);
};


#endif //CPP_LIFE_CELLRULE_H