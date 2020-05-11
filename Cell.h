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
class CellRect: public WorldMember{
public:
    CellRect();
    CellRect(int x, int y);
    void CreateObjectTexture() override;
};
class Cell{
private:
    CellState cellState;
    CellRect * _rect;
public:
    void SetRect(CellRect & rect){
        _rect = &rect;
}
    CellRect & GetRect(){
    return *_rect;
}
    Cell(CellBehavior state=CellBehavior::Empty);
    bool IsAlive(){ return cellState.current_behavior == CellBehavior::Alive;}
    const CellBehavior & GetNextState() const  { return cellState.next_behavior;}
    const CellBehavior & GetState() const  { return cellState.current_behavior;}
    void SetState(CellBehavior state){
        cellState.current_behavior = state;
    };
    void SetNextState(CellBehavior state){
        cellState.next_behavior = state;
    };
};




#endif //CPP_LIFE_CELL_H
