//
// Created by egor0 on 4/21/2020.
//
#include "Grid.h"
#include <thread>
#include "CellRule.h"
#include <iostream>
#include <iomanip>

Grid::Grid(int width, int height, int threadsAmount,sf::RenderTarget& screen) {
    rule = new Rule("B3/S23",*this);
    _isPaused = true;
    _screen = &screen;
    // create empty grid

    _width = width;
    _height = height;
    pool = new ThreadPool(threadsAmount);
    storage = new std::vector<Cell*>[threadsAmount];
    _ranges = DivideGridIntoZones(threadsAmount,_width);

    clock_t start, end;
    start = clock();
    // fill grid with empty cells using threads

    std::cout<<"Creating array"<<std::endl;
    _grid.Init(nullptr, height);
    _rect_grid.Init(nullptr,height);
    for(int y=0;y<height;y++){
        _grid[y].Init(nullptr, width);
        _rect_grid[y].Init(nullptr,width);
        pool->AddJob([this,width,y]() mutable {
            for(int x=0;x<width;x++){
                _rect_grid[y][x] = CellRect(x,y);
                _grid[y][x] = Cell();
                _grid[y][x].SetRect(_rect_grid[y][x]);

            }
        });
    }
    pool->WaitAll();

    end=clock();
    double time_taken = double(end - start)/ double (CLOCKS_PER_SEC);
    std::cout << "Created array in : " << std::fixed
              << time_taken << std::setprecision(5);
    std::cout << " sec " << std::endl;

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
    for(int y=0;y<_height;y++){

        for(int x=range[0];x<range[1];x++){
            cell = &GetCell(x,y);

            if(cell->GetState() == CellBehavior::Alive){
                temp.emplace_back(cell);
            }
            if(!_isPaused) {
                    rule->Execute(cell,x,y);

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
//    clock_t start, end;
//
//    double time_taken;
    for(int i=0;i<threadsAmount;i++){
       pool->AddJob( [this,i](){this->UpdateCellsStates(_ranges[i]);});
    }
    pool->WaitAll();


    for(int i=0;i<threadsAmount;i++){
        pool->AddJob( [this,i]() {this->CalculateZone(i,_ranges[i]);});
    }
    pool->WaitAll();


//    start = clock();
    // change _cells_to_draw_coords size
    unsigned int newSize=0;
    for(int i=0;i<threadsAmount;i++){
        newSize+=storage[i].size();
    }
    _cells_to_draw.reserve(newSize);

    for(int i=0;i<threadsAmount;i++){
        _cells_to_draw.insert(_cells_to_draw.end(), storage[i].begin(), storage[i].end());
    }
//    end=clock();
//    time_taken = double(end - start)/ double (CLOCKS_PER_SEC);
//    std::cout << "Calced in : " << std::fixed
//              << time_taken << std::setprecision(5);
//    std::cout << " sec " << std::endl;

}



void Grid::DisplayCells() {
    for (const auto & cell:_cells_to_draw) {
        cell->GetRect().DrawTo(_screen);
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