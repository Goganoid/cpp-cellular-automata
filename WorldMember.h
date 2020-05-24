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
    sf::VertexArray  objectRect;
public:
    WorldMember(int x, int y,sf::Color color);
    int GetX();
    int GetY();
    virtual void SetPos(int x,int y);
    virtual void DrawTo(sf::RenderTarget *renderTarget);
    virtual void Move(int x, int y);
};
#endif //CPP_LIFE_WORLDMEMBER_H
