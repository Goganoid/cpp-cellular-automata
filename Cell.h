//
// Created by egor0 on 4/21/2020.
//
#include <vector>
#include <iostream>
#include "WorldMember.h"

#ifndef CPP_LIFE_CELL_H
#define CPP_LIFE_CELL_H
enum CellBehavior: uint8_t{
    Empty,
    Alive,

};
class CellRect: public WorldMember{
public:
    CellRect();
    CellRect(int x, int y);
};
struct Cell{
    CellBehavior	current_behavior : 4;
    CellBehavior	next_behavior : 4;
    CellRect * _rect;
    void SetRect(CellRect & rect) {
        _rect = &rect;
    }
    [[nodiscard]] bool IsAlive() const{ return current_behavior == CellBehavior::Alive;}
    [[nodiscard]] CellBehavior  GetNextState() const  { return next_behavior;}
    [[nodiscard]] CellBehavior  GetState() const  { return current_behavior;}
    void SetState(CellBehavior state){
        current_behavior = state;
    };
    void SetNextState(CellBehavior state){
        next_behavior = state;
    };
};




#endif //CPP_LIFE_CELL_H
