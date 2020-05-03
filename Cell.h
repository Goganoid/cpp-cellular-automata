//
// Created by egor0 on 4/21/2020.
//
#include <vector>
#include <iostream>
#include "WorldMember.h"

#ifndef CPP_LIFE_CELL_H
#define CPP_LIFE_CELL_H
enum CellState{
    Empty,
    Alive,

};
class Cell: public WorldMember{
private:
    CellState _state;
    CellState _nextState;
public:
    Cell();
    Cell(int x,int y,CellState state=CellState::Empty);

    void CreateObjectTexture() override;
    bool IsAlive(){ return _state==CellState::Alive;}
    CellState GetState()  { return _state;}
    CellState GetNextState()  { return _nextState;}
    void SetState(CellState state);
    void SetNextState(CellState state);
//    void Move(int x, int y) override {};
};
#endif //CPP_LIFE_CELL_H
