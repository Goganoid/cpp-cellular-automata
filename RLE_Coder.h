//
// Created by egor0 on 4/30/2020.
//

#ifndef CPP_LIFE_RLE_CODER_H
#define CPP_LIFE_RLE_CODER_H
#include <string.h>
#include <iostream>
#include <fstream>
#include <regex>
#include "Point.h"


struct RLEReadResult{
    Point point;
    std::vector<Point> pattern;
};

std::vector<Point> FromRLE(std::string rle){
    std::remove(rle.begin(),rle.end(),'\n');
    std::vector<Point> result;
    int y=0;
    int x=0;
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
                        y+=1;
                        x=0;
                    }
                    if(rle[i]=='o'){
                        result.push_back({x,y});
                        x+=1;
                    }
                    if(rle[i]=='b'){
                        x+=1;
                    }
                }
                number = "";
            }

        }
    }

    return result;
}
RLEReadResult OpenRLE_File(const std::string& filePath){
    // searches for RLE parameters
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
                        strParameters.push_back(match.str(2));
                    }
                }
                else{
                    rle+=line;
                }
            }

        }
        file.close();
        RLEReadResult result;
        result.point.x = std::stoi(strParameters[0]);
        result.point.y = std::stoi(strParameters[1]);
        result.pattern = FromRLE(rle);
        return result;
    }
    else{
        std::cout<<"Error"<<std::endl;
    }
}



#endif //CPP_LIFE_RLE_CODER_H
