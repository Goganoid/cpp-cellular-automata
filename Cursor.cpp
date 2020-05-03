//
// Created by egor0 on 4/23/2020.
//
#include "Cursor.h"
Cursor::Cursor(int x, int y) {
    _x=x;
    _y=y;
    CreateObjectTexture();
}
void Cursor::CreateObjectTexture() {
    objectRect = sf::RectangleShape(sf::Vector2f(1.0f,1.0f));
    objectRect.setFillColor(sf::Color::Yellow);
    objectRect.setPosition(_x,_y);
}
