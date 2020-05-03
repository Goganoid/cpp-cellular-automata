//
// Created by egor0 on 4/21/2020.
//
#include "Grid.h"
#include <thread>
#include "CellRule.h"
#include <iostream>
#include <iomanip>

Grid::Grid(int width, int height, int threadsAmount,sf::RenderTarget& screen) {
    _isPaused = true;
    _screen = &screen;
    // create empty grid

    _width = width;
    _height = height;
    pool = new ThreadPool(threadsAmount);
    storage = new std::vector<std::vector<int>>[threadsAmount];


    clock_t start, end;
    start = clock();
    // fill grid with empty cells using threads

    std::cout<<"Creating array"<<std::endl;
    _grid.Init(nullptr, height);
    for(int y=0;y<height;y++){
        _grid[y].Init(nullptr, width);

        pool->AddJob([this,width,y]() mutable {
            for(int x=0;x<width;x++){
                _grid[y][x] = Cell(x, y);
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
    delete pool;
    delete []storage;
}

Cell& Grid::GetCell(int x, int y) {
    return _grid[y][x];
}
Cell& Grid::GetCell(int coords[2]) {
    return GetCell(coords[0],coords[1]);
}
Cell& Grid::GetCell(std::vector<int> coords) {
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
    std::vector<std::vector<int>> temp;
    for(int y=0;y<_height;y++){

        for(int x=range[0];x<range[1];x++){
            Cell & cell = GetCell(x,y);

            if(cell.GetState()==CellState::Alive){
                temp.emplace_back(std::vector<int>{x,y});
            }
            if(!_isPaused){
                cell.SetNextState(GameOfLife(*this,x,y));
            }
        }
    }
    storage[id] = temp;
}

void Grid::UpdateCellsStates(const int range[2]){
    for(int y=0;y<_height;y++){
        for(int x=range[0];x<range[1];x++) {
            Cell & cell = GetCell(x,y);
            cell.SetState(cell.GetNextState());
        }
    }
}

void Grid::CalculateCells(const int threadsAmount) {
    int ** ranges = DivideGridIntoZones(threadsAmount,_width);

    for(int i=0;i< threadsAmount;i++){
       pool->AddJob( [this,ranges,i](){this->UpdateCellsStates(ranges[i]);});
    }
    pool->WaitAll();

    for(int i=0;i< threadsAmount;i++){
        pool->AddJob( [this,ranges,i]() {this->CalculateZone(i,ranges[i]);});
    }
    pool->WaitAll();

    // change _cells_to_draw_coords size
    unsigned int newSize=0;
    for(int i=0;i<threadsAmount;i++){
        newSize+=storage[i].size();
    }
    _cells_to_draw_coords.reserve(newSize);

    for(int i=0;i<threadsAmount;i++){
        _cells_to_draw_coords.insert(_cells_to_draw_coords.end(),storage[i].begin(),storage[i].end());
    }



}



void Grid::DisplayCells() {
    for (const auto &cell_coord:_cells_to_draw_coords) {
        GetCell(cell_coord).DrawTo(_screen);
    }
    _cells_to_draw_coords.clear();
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