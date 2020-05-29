//
// Created by egor0 on 4/21/2020.
//
#include "Grid.h"
#include <thread>
#include "CellRule.h"
#include <iostream>
#include "Timer.h"


Grid::Grid(int width, int height, int threadsAmount,sf::RenderTarget& screen) {
    logger = new AppLog(400,150);

    logger->AddLog("Creating lookup rule");
    Timer timer;
    timer.Start();
    rule = new LookupRule("B3/S23");
    timer.End();
    logger->AddLog(timer.GetTime("creating lookup rule"));

    _isPaused = true;
    _screen = &screen;
    _threads = threadsAmount;

    _width = width;
    _height = height;
    pool = new ThreadPool(threadsAmount);
    storage = new std::vector<sf::Vertex>[threadsAmount];
    _ranges = DivideGridIntoZones(threadsAmount,_width);


    // fill grid with empty cells using threads
    logger->AddLog("Creating array");
    timer.Start();
    // create empty grid
    _grid = new Cell[_height*_width];
    _rect_grid = new CellRect[_height*_width];
    for(int y=0;y<_height;y++){
        pool->AddJob([this,y]() mutable {
            for(int x=0;x<_width;x++){
                _rect_grid[y * _width + x] = CellRect(x,y);
                _grid[y * _width + x] = {CellBehavior::Empty,CellBehavior::Empty};
                _grid[y * _width +x].SetRect(_rect_grid[y * _width + x]);
            }
        });
    }
    pool->WaitAll();
    timer.End();
    logger->AddLog(timer.GetTime("creating array"));

}

Grid::~Grid() {
    delete rule;
    delete pool;
    delete logger;
    delete []storage;
    delete []_grid;
    delete []_rect_grid;
}

Cell& Grid::GetCell(int x, int y) {
    if(y>=_height) y-=_height;
    if(x>=_width || x<0) y-=1;
    return _grid[_width*y + x];
}

[[maybe_unused]] Cell& Grid::GetCell(int coords[2]) {
    return GetCell(coords[0],coords[1]);
}

[[maybe_unused]] Cell& Grid::GetCell(std::vector<int> & coords) {
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


void Grid::CalculateZone(int id, int range[2]){
    std::vector<sf::Vertex> temp;
    Cell * cell;
    for(int y=1;y<=_height;y++){
        // precalculating part of this value in outer loop to reduce number of memory accesses
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
                // emplace vertices
                temp.emplace_back(cell->_rect->objectRect[0]);
                temp.emplace_back(cell->_rect->objectRect[1]);
                temp.emplace_back(cell->_rect->objectRect[2]);
                temp.emplace_back(cell->_rect->objectRect[3]);
            }
            if(!_isPaused) {
                cell->SetNextState(rule->Lookup(environment));
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

void Grid::CalculateCells() {
    Timer timer;
    timer.Start();
    // update state
    for (int i = 0; i < _threads; i++) {
        pool->AddJob([this, i]() { this->UpdateCellsStates(_ranges[i]); });
    }
    pool->WaitAll();

    // calculate next state
    for (int i = 0; i < _threads; i++) {
        pool->AddJob([this, i]() { this->CalculateZone(i, _ranges[i]); });
    }
    pool->WaitAll();

    timer.End();
    logger->AddLog(timer.GetTime("calculation"));


    unsigned int newSize=0;
    for(int i=0;i<_threads;i++){
        newSize+=storage[i].size();
    }
    _cells_to_draw.reserve(newSize);
    // concatenate storage in one vector
    for(int i=0;i<_threads;i++){
        _cells_to_draw.insert(_cells_to_draw.end(), storage[i].begin(), storage[i].end());
    }

}



void Grid::DisplayCells() {
        if(!_cells_to_draw.empty()) {
            _screen->draw(&_cells_to_draw[0], _cells_to_draw.size(), sf::Quads);
            _cells_to_draw.clear();
        }
}

[[maybe_unused]] void Grid::SetPause(bool state) {
    _isPaused = state;
    logger->AddLog(GetPauseInfo());
}

[[maybe_unused]] bool Grid::IsPaused() const {
    return _isPaused;
}
void Grid::TogglePause() {
    _isPaused = !_isPaused;
    logger->AddLog(GetPauseInfo());
}

std::string Grid::GetPauseInfo() const {
    std::string s = "Pause is ";
    s+= (_isPaused ? "on" : "off");
    return s;
}