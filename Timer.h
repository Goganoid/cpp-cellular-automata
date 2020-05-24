//
// Created by egor0 on 5/24/2020.
//

#ifndef CPP_LIFE_TIMER_H
#define CPP_LIFE_TIMER_H

#include <ctime>
#include <iostream>
#include <iomanip>
#include <string>
class Timer{
private:
        clock_t start, end;
        double time_taken;
public:
    void Start(){
        start = clock();
    }
    void End(){
        end = clock();
    }
    void PrintTime(std::string objectName="job"){
        time_taken = double(end - start)/ double (CLOCKS_PER_SEC);
        std::cout << "Done "<<objectName<<" in : " << std::fixed
                  << time_taken << std::setprecision(5);
        std::cout << " sec " << std::endl;
    }
};
#endif //CPP_LIFE_TIMER_H
