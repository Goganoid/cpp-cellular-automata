//
// Created by egor0 on 5/3/2020.
//

#ifndef CPP_LIFE_CONTROLS_H
#define CPP_LIFE_CONTROLS_H
#include "SFML/Graphics.hpp"
#include "Grid.h"
#include "Cursor.h"
class MouseControls {
private:
    Grid *_grid;
    Cursor *_cursor;

    sf::RenderWindow *_window;
    sf::Vector2f _viewportSize;
    sf::Vector2i _mouseViewPortPos;
    sf::Vector2i _prevMouseViewPortPos;
    sf::Vector2f _center;
public:
    float zoomModifier = 20;

    MouseControls(Grid &grid, Cursor &cursor, sf::RenderWindow &window);

    static void setViewSize(sf::RenderWindow &window, sf::Vector2f center, sf::Vector2f size);

    static sf::Vector2i mouseToCursorPos(sf::RenderWindow &window, sf::Vector2f screenSize);

    static sf::Vector2i mouseToViewPortPos(sf::RenderWindow &window, sf::Vector2f screenSize);

    void SwitchMouse(sf::Event event);



};
#endif //CPP_LIFE_CONTROLS_H
