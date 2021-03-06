//
// Created by egor0 on 4/23/2020.
//

#ifndef CPP_LIFE_WORLDMEMBER_H
#define CPP_LIFE_WORLDMEMBER_H

#include "SFML/Graphics.hpp"

class WorldMember{
protected:
    int _x;
    int _y;

public:
    sf::VertexArray  objectRect;
    WorldMember(int x, int y,sf::Color color);
    int GetX() const;
    int GetY() const;
    void SetPos(int x,int y);
    void DrawTo(sf::RenderTarget *renderTarget);
    void Move(int x, int y);
};
#endif //CPP_LIFE_WORLDMEMBER_H
