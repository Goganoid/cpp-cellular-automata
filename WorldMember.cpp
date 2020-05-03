//
// Created by egor0 on 4/23/2020.
//
#include "WorldMember.h"

int WorldMember::GetX() { return _x;}
int WorldMember::GetY() { return _y;}
void WorldMember::Move(int x, int y) {
    _x+=x;
    _y+=y;
    objectRect.setPosition(_x,_y);
}
void WorldMember::DrawTo(sf::RenderTarget *renderTarget) {
    renderTarget->draw(objectRect);
}
void WorldMember::SetPos(int x, int y) {
    _x = x;
    _y = y;
    objectRect.setPosition(_x,_y);
}