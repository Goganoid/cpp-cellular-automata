//
// Created by egor0 on 5/6/2020.
//
#include "CellRule.h"
#include <bitset>

// divides "3/23" in ["3","23"]
inline std::vector<std::string> DivideLine(const std::string& string,char delimiter){
    std::stringstream ss(string);
    std::string segment;
    std::vector<std::string> dividedLine;
    while(std::getline(ss, segment, delimiter))
    {
        dividedLine.push_back(segment);
    }
    return dividedLine;
}

LookupRule::LookupRule(std::string stringRule){
    // remove literals
    stringRule.erase(std::remove_if(stringRule.begin(), stringRule.end(), ::isalpha), stringRule.end());
    std::stringstream ss(stringRule);
    std::string segment;
    std::vector<std::string> params = DivideLine(stringRule,'/');
    // fill with default value
    for(int i=0;i<9;i++){
        neighsToSurviveTable[i] = false;
        neighsToBornTable[i] = false;
    }
    // "c-48" - converts char to int. '10'-48 = 10
    for(char& c : params[0]) {
        neighsToBornTable[c - 48] = true;
    }
    for(char& c : params[1]) {
        neighsToSurviveTable[c - 48] = true;
    }
    // fill lookup table
    for(int i =0;i<512;i++){
        // transform integer in something like that
        // 7 => 0111 => 011100000
        //  0 # #
        //  # 0 0
        //  0 0 0
        std::bitset<9> bitArray(i);
        int alive_cells = bitArray.count();
        lookupTable[i] = (neighsToBornTable[alive_cells] || ((neighsToSurviveTable[alive_cells-1])
                         && bitArray[4]==1)) ? CellBehavior::Alive : CellBehavior::Empty;
    }

}
