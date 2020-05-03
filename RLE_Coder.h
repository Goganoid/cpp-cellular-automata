//
// Created by egor0 on 4/30/2020.
//

#ifndef CPP_LIFE_RLE_CODER_H
#define CPP_LIFE_RLE_CODER_H
#include "Grid.h"
#include <string.h>

//std::string ToRlE(Grid& grid,int x, int y){
//
//}
std::vector< std::vector<CellState> > FromRLE(std::string rle,int x, int y){
    std::vector< std::vector<CellState> > result(std::count(rle.begin(),rle.end(),'$')+1);
    int vec_y=0;

    std::string number;

    for(int i=0;i<rle.size();i++){
        if(std::isdigit(rle[i])){
            number+=rle[i];
        }
        else{
            if(rle[i]=='$'){
                vec_y+=1;
            }
            else if(rle[i]=='!'){
                continue;
            }
            else{

                if(number.length()==0) number="1";

                for(int n=0;n<std::stoi(number);n++){
                    if(rle[i]=='o'){
                        result[vec_y].push_back(CellState::Alive);
                    }
                    if(rle[i]=='b'){
                        result[vec_y].push_back(CellState::Empty);
                    }
                }
                number = "";
            }

        }
    }
    return result;
}

#endif //CPP_LIFE_RLE_CODER_H
