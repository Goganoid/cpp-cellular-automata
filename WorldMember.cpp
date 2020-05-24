//
// Created by egor0 on 4/23/2020.
//
#include "WorldMember.h"

WorldMember::WorldMember(int x, int y, sf::Color color) {
    objectRect = sf::VertexArray(sf::Quads,4);
    objectRect[0].color = color;
    objectRect[1].color = color;
    objectRect[2].color = color;
    objectRect[3].color = color;
    SetPos(x,y);
}

int WorldMember::GetX() const { return _x;}
int WorldMember::GetY() const { return _y;}
void WorldMember::Move(int x, int y) {
    SetPos(_x+x,_y+y);
}
void WorldMember::DrawTo(sf::RenderTarget *renderTarget) {
    renderTarget->draw(objectRect);
}
void WorldMember::SetPos(int x, int y) {
    _x = x;
    _y = y;
    objectRect[0].position = sf::Vector2f(x, y);
    objectRect[1].position = sf::Vector2f(x+1, y);
    objectRect[2].position = sf::Vector2f(x+1, y+1);
    objectRect[3].position = sf::Vector2f(x, y+1);
}