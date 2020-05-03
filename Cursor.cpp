//
// Created by egor0 on 4/23/2020.
//
#include "Cursor.h"
Cursor::Cursor() {
    _x=50;
    _y=50;
    CreateObjectTexture();
}
void Cursor::CreateObjectTexture() {
    objectRect = sf::RectangleShape(sf::Vector2f(1.0f,1.0f));
    objectRect.setFillColor(sf::Color::Yellow);
    objectRect.setPosition(_x,_y);
}
