//
// Created by egor0 on 4/21/2020.
//
#include "Cell.h"

Cell::Cell(){
    Cell(1,1);
}
void Cell::CreateObjectTexture() {
    objectRect = sf::RectangleShape(sf::Vector2f(1.0f,1.0f));
    objectRect.setFillColor(sf::Color::White);
    objectRect.setPosition(_x,_y);
}
Cell::Cell(int x,int y,CellState state){
    _x = x;
    _y = y;
    _state = state;
    _nextState = state;
    CreateObjectTexture();
}
void Cell::SetState(CellState state) {
    _state = state;
}
void Cell::SetNextState(CellState state) {
    _nextState = state;
}