//
// Created by egor0 on 4/21/2020.
//
#include "Cell.h"


Cell::Cell(CellBehavior state){
    cellState.current_behavior = state;
    cellState.next_behavior = state;
}


CellRect::CellRect() {
    CellRect(1,1);
}
CellRect::CellRect(int x, int y) {
    _x = x;
    _y = y;
    CreateObjectTexture();
}
void CellRect::CreateObjectTexture() {
    objectRect = sf::RectangleShape(sf::Vector2f(1.0f,1.0f));
    objectRect.setFillColor(sf::Color::White);
    objectRect.setPosition(_x,_y);
}