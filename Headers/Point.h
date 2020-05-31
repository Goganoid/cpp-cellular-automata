//
// Created by egor0 on 5/22/2020.
//

#ifndef CPP_LIFE_POINT_H
#define CPP_LIFE_POINT_H
struct Point{
    int x;
    int y;
    bool const operator==(const Point &o) const {
        return x == o.x && y == o.y;
    }

    bool const operator<(const Point &o) const {
        return x < o.x || (x == o.x && y < o.y);}
};
#endif //CPP_LIFE_POINT_H
