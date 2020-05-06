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

Rule::Rule(std::string stringRule){
    // remove literals
    stringRule.erase(std::remove_if(stringRule.begin(), stringRule.end(), ::isalpha), stringRule.end());

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
CellState Rule::Execute(Grid& grid, int x, int y){
    int neighbours =
            grid.GetCell(x+1,y).GetState() +
            grid.GetCell(x-1,y).GetState() +
            grid.GetCell(x,y+1).GetState() +
            grid.GetCell(x,y-1).GetState() +
            grid.GetCell(x+1,y+1).GetState() +
            grid.GetCell(x-1,y-1).GetState() +
            grid.GetCell(x+1,y-1).GetState() +
            grid.GetCell(x-1,y+1).GetState();

    bool willBorn = std::find(neighsToBorn.begin(), neighsToBorn.end(), neighbours) != neighsToBorn.end();
    bool willSurvive = std::find(neighsToSurvive.begin(),neighsToSurvive.end(),neighbours)!=neighsToSurvive.end();
    // if neighbours count is in vector
    if (willBorn){
        return CellState::Alive;
    }
    if( willSurvive && grid.GetCell(x,y).IsAlive()){
        return CellState::Alive;
    }
    return CellState::Empty;
}
