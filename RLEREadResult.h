//
// Created by egor0 on 5/31/2020.
//

#ifndef CPP_LIFE_RLEREADRESULT_H
#define CPP_LIFE_RLEREADRESULT_H
#include "Point.h"
struct RLEReadResult{
    std::string rule;
    Point point;
    std::vector<Point> pattern;
};
#endif //CPP_LIFE_RLEREADRESULT_H
