//
// Created by egor0 on 4/30/2020.
//

#ifndef CPP_LIFE_RLE_CODER_H
#define CPP_LIFE_RLE_CODER_H
#include "Grid.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include <regex>


struct RLEReadResult{
    int x;
    int y;
    std::vector< std::vector<CellBehavior> > pattern;
};

std::vector< std::vector<CellBehavior> > FromRLE(std::string rle){
    std::remove(rle.begin(),rle.end(),'\n');
//    std::vector< std::vector<CellState> > result(std::count(rle.begin(),rle.end(),'$')+1);
    std::vector< std::vector<CellBehavior> > result(1);
    int vec_y=0;

    std::string number;

    for(int i=0;i<rle.size();i++){
        if(std::isdigit(rle[i])){
            number+=rle[i];
        }
        else{

            if(rle[i]=='!'){
                continue;
            }
            else{

                if(number.length()==0) number="1";

                for(int n=0;n<std::stoi(number);n++){
                    if(rle[i]=='$'){
                        vec_y+=1;
                        result.push_back(std::vector<CellBehavior>());
                    }
                    if(rle[i]=='o'){
                        result[vec_y].push_back(CellBehavior::Alive);
                    }
                    if(rle[i]=='b'){
                        result[vec_y].push_back(CellBehavior::Empty);
                    }
                }
                number = "";
            }

        }
    }

    return result;
}
RLEReadResult OpenRLE_File(std::string filePath){

//    std::regex key_value_regex("([^=,]*)=(\"[^\"]*\"|[^,\"]*)");
    std::regex key_value_regex("(\\w+)\\s*=\\s*([^\\s]+)");

    std::string line;
    std::ifstream file (filePath);

    bool parametersFound = false;
    std::string rle="";
    std::vector<std::string> strParameters;
    if (file.is_open())
    {
        while ( getline (file,line) )
        {

            if (line.rfind("#C",0)!=0){
                if(!parametersFound) {
                    auto words_begin =
                            std::sregex_iterator(line.begin(), line.end(), key_value_regex);
                    auto words_end = std::sregex_iterator();


                    if (std::distance(words_begin, words_end) > 0) {
                        parametersFound = true;

                    }
                    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
                        std::smatch match = *i;
//                    std::string match_str = match.str(2);
                        strParameters.push_back(match.str(2));
                    }
                }
                else{
                    rle+=line;
//                    std::cout<<rle<<std::endl;
                }
            }

        }
        file.close();
        RLEReadResult result;
        result.x = std::stoi(strParameters[0]);
        result.y = std::stoi(strParameters[1]);
        result.pattern = FromRLE(rle);
        return result;
    }
    else{
        std::cout<<"Error"<<std::endl;
    }
}



#endif //CPP_LIFE_RLE_CODER_H
