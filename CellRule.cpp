//
// Created by egor0 on 5/6/2020.
//
#include "CellRule.h"

inline std::vector<std::string> DivideLine(std::string string,char delim){
    std::stringstream stringstream(string);
    std::string segment;
    std::vector<std::string> dividedLine;
    while(std::getline(stringstream, segment, '/'))
    {
        dividedLine.push_back(segment);
    }
    return dividedLine;
}

Rule::Rule(std::string stringRule,Grid& grid){
    // remove literals
    stringRule.erase(std::remove_if(stringRule.begin(), stringRule.end(), ::isalpha), stringRule.end());
    _grid = &grid;
    std::stringstream stringstream(stringRule);
    std::string segment;
    std::vector<std::string> params = DivideLine(stringRule,'/');
    // "c-48" - converts char to int. '10'-48 = 10
    for(char& c : params[0]) {
    neighsToBorn.push_back(c-48);
    }
    for(char& c : params[1]) {
    neighsToSurvive.push_back(c-48);
    }
}
void Rule::Execute(Cell * cell,int  x, int  y){
    int _neighbours =
            _grid->GetCell(x+1,y).GetState() +
            _grid->GetCell(x-1,y).GetState() +
            _grid->GetCell(x,y+1).GetState() +
            _grid->GetCell(x,y-1).GetState() +
            _grid->GetCell(x+1,y+1).GetState() +
            _grid->GetCell(x-1,y-1).GetState() +
            _grid->GetCell(x+1,y-1).GetState() +
            _grid->GetCell(x-1,y+1).GetState();
    bool willBorn = std::find(neighsToBorn.begin(), neighsToBorn.end(), _neighbours) != neighsToBorn.end();
    bool willSurvive = std::find(neighsToSurvive.begin(), neighsToSurvive.end(), _neighbours) != neighsToSurvive.end();

    // if neighbours count is in vector
    if (willBorn || (willSurvive && _grid->GetCell(x,y).IsAlive())){
        cell->SetNextState(CellBehavior::Alive);
//        return CellState::Alive;
    }
    else{
        cell->SetNextState(CellBehavior::Empty);
    }
//    if( willSurvive && _grid->GetCell(x,y).IsAlive()){
//        cell->SetNextState(CellState::Alive);
//        return CellState::Alive;
//    }
//    return CellState::Empty;
}
