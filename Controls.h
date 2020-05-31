//
// Created by egor0 on 5/3/2020.
//

#ifndef CPP_LIFE_CONTROLS_H
#define CPP_LIFE_CONTROLS_H
#include "SFML/Graphics.hpp"
#include "Grid.h"
#include "Cursor.h"
#include "RLEREadResult.h"

class Controls {
private:
    Grid *_grid;
    Cursor *_cursor;
    RLEReadResult _pattern;
    sf::RenderWindow *_window;
    sf::Vector2f _viewportSize;
    sf::Vector2i _mouseViewPortPos;
    sf::Vector2i _prevMouseViewPortPos;
    sf::Vector2f _center;
    bool disabled = false;
public:
    float zoomModifier = 20;
    unsigned int framerateLimit = 50;
    Controls(Grid &grid, Cursor &cursor, sf::RenderWindow &window);

    void UpdateMouseConfiguration();

    void SetPattern(RLEReadResult pattern);
    RLEReadResult const& GetPattern() const;
    static void setViewSize(sf::RenderWindow &window, sf::Vector2f center, sf::Vector2f size);

    static sf::Vector2i mouseToCursorPos(sf::RenderWindow &window, sf::Vector2f screenSize);

    static sf::Vector2i mouseToViewPortPos(sf::RenderWindow &window, sf::Vector2f screenSize);

    void SwitchMouse(sf::Event event);
    void SwitchKeyboard(sf::Event event);
    void DisableControls();
    void EnableControls();


};
#endif //CPP_LIFE_CONTROLS_H
