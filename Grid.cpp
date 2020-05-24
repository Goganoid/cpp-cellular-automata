//
// Created by egor0 on 4/21/2020.
//
#include "Grid.h"
#include <thread>
#include "CellRule.h"
#include <iostream>
#include "Timer.h"
Grid::Grid(int width, int height, int threadsAmount,sf::RenderTarget& screen) {
    rule = new LifeRule("B3/S23");
    _isPaused = true;
    _screen = &screen;


    _width = width;
    _height = height;
    pool = new ThreadPool(threadsAmount);
    storage = new std::vector<Cell*>[threadsAmount];
    _ranges = DivideGridIntoZones(threadsAmount,_width);

    Timer timer;
    // fill grid with empty cells using threads
    std::cout<<"Creating array"<<std::endl;
    timer.Start();
    // create empty grid
    _grid.Init(nullptr, _height);
    _rect_grid.Init(nullptr,_height*_width);
    for(int y=0;y<_height;y++){
        _grid[y].Init(nullptr,_width);
        _rect_grid[y].Init(nullptr,_width);
        pool->AddJob([this,y]() mutable {
            for(int x=0;x<_width;x++){
                _rect_grid[y][x] = CellRect(x,y);
                _grid[y][x] = {CellBehavior::Empty,CellBehavior::Empty};
                _grid[y][x].SetRect(_rect_grid[y][x]);
            }
        });
    }
    pool->WaitAll();
    timer.End();
    timer.PrintTime("creating array");

}

Grid::~Grid() {
    delete rule;
    delete pool;
    delete []storage;
}

Cell& Grid::GetCell(int x, int y) {
    return _grid[y][x];
}
Cell& Grid::GetCell(int coords[2]) {
    return GetCell(coords[0],coords[1]);
}
Cell& Grid::GetCell(std::vector<int> & coords) {
    return GetCell(coords[0],coords[1]);
}



int ** Grid::DivideGridIntoZones(int zones, int length){
    int** zonesRanges;
    zonesRanges = new int*[zones];
    if((length%zones)!=0){
        throw std::runtime_error("The length should be divided completely!");
    }
    int zoneLength = length/zones;
    int prevValue=0;
    for(int i=0;i<zones;i+=1){
        zonesRanges[i] = new int[2]{prevValue,(i+1)*zoneLength};
        prevValue = (i+1)*zoneLength;
    }
    return zonesRanges;
}


void Grid::CalculateZone(int id,const int range[2]){
    std::vector<Cell*> temp;
    Cell * cell;
    for(int y=1;y<=_height;y++){
        int environment=   (GetCell(range[0],y-1).GetState() ? 32 : 0) + (GetCell(range[0]+1,y-1).GetState() ?  4 : 0)
                         + (GetCell(range[0],   y  ).GetState() ? 16 : 0) + (GetCell(range[0]+1   ,y  ).GetState() ?  2 : 0)
                         + (GetCell(range[0],y+1).GetState() ?  8 : 0) + (GetCell(range[0]+1,y+1).GetState() ?  1 : 0);

        for(int x=range[0]+1;x<=range[1];x++){
            environment = ((environment % 64) * 8)
                          + (GetCell(x+1,y-1).GetState() ? 4 : 0)
                          + (GetCell(x+1,y  ).GetState() ? 2 : 0)
                          + (GetCell(x+1,y+1).GetState() ? 1 : 0);
            cell = &GetCell(x,y);
            if(cell->GetState() == CellBehavior::Alive){
                temp.emplace_back(cell);
            }
            if(!_isPaused) {
                cell->SetNextState(static_cast<CellBehavior>(rule->Lookup(environment)));
                }
            }
        }
    storage[id] = temp;
}

void Grid::UpdateCellsStates(const int * range){
    Cell * cell;
    for(int y=0;y<_height;y++){
        for(int x=range[0];x<range[1];x++) {
            cell = &GetCell(x,y);
            cell->SetState(cell->GetNextState());

        }
    }
}

void Grid::CalculateCells(const int threadsAmount) {

    for(int i=0;i<threadsAmount;i++){
       pool->AddJob( [this,i](){this->UpdateCellsStates(_ranges[i]);});
    }
    pool->WaitAll();



    for(int i=0;i<threadsAmount;i++){
        pool->AddJob( [this,i]() {this->CalculateZone(i,_ranges[i]);});
    }
    pool->WaitAll();

    unsigned int newSize=0;
    for(int i=0;i<threadsAmount;i++){
        newSize+=storage[i].size();
    }
    _cells_to_draw.reserve(newSize);
    for(int i=0;i<threadsAmount;i++){
        _cells_to_draw.insert(_cells_to_draw.end(), storage[i].begin(), storage[i].end());
    }

}



void Grid::DisplayCells() {
    for (size_t i=0;i<_cells_to_draw.size();i++) {
        _cells_to_draw[i]->_rect->DrawTo(_screen);
    }
    _cells_to_draw.clear();
}

void Grid::SetPause(bool state) {
    _isPaused = state;
}
bool Grid::IsPaused() {
    return _isPaused;
}
void Grid::TogglePause() {
    _isPaused = !_isPaused;
}