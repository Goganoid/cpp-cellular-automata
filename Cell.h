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
struct CellState{
    CellBehavior	current_behavior : 4;
    CellBehavior	next_behavior : 4;
};

class Cell: public WorldMember{
private:
//    CellBehavior _state;
//    CellBehavior _nextState;
    CellState cellState;
public:
    Cell();
    Cell(int x, int y, CellBehavior state=CellBehavior::Empty);
    void CreateObjectTexture() override;
//    bool IsAlive(){ return _state == CellBehavior::Alive;}
    bool IsAlive(){ return cellState.current_behavior == CellBehavior::Alive;}
//    CellBehavior & GetNextState()  { return _nextState;}
    const CellBehavior & GetNextState() const  { return cellState.next_behavior;}
//    CellBehavior & GetState()  { return _state;}
    const CellBehavior & GetState() const  { return cellState.current_behavior;}
    void SetState(CellBehavior state);
    void SetNextState(CellBehavior state);
};
#endif //CPP_LIFE_CELL_H
