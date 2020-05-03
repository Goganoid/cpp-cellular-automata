//
// Created by egor0 on 4/26/2020.
//

#ifndef CPP_LIFE_CELLRULE_H
#define CPP_LIFE_CELLRULE_H
#include "Grid.h"
enum class Rule{
    GameOfLife
};
inline CellState GameOfLife(Grid& grid, int x, int y){
    int neighbours =
        grid.GetCell(x+1,y).GetState() +
        grid.GetCell(x-1,y).GetState() +
        grid.GetCell(x,y+1).GetState() +
        grid.GetCell(x,y-1).GetState() +
        grid.GetCell(x+1,y+1).GetState() +
        grid.GetCell(x-1,y-1).GetState() +
        grid.GetCell(x+1,y-1).GetState() +
        grid.GetCell(x-1,y+1).GetState();

    if(neighbours==2 && grid.GetCell(x,y).IsAlive()){
        return CellState::Alive;
    }
    if(neighbours==3){
        return CellState::Alive;
    }
    return CellState::Empty;
}
#endif //CPP_LIFE_CELLRULE_H
