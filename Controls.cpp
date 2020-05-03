//
// Created by egor0 on 5/3/2020.
//
#include "Controls.h"

MouseControls::MouseControls(Grid& grid, Cursor& cursor, sf::RenderWindow& window){
_grid = &grid;
_cursor = &cursor;
_window = &window;
_center = window.getView().getCenter();
_viewportSize = window.getView().getSize();
_mouseViewPortPos = mouseToViewPortPos(*_window, _viewportSize);
_prevMouseViewPortPos = _mouseViewPortPos;
}

void MouseControls::setViewSize(sf::RenderWindow &window,sf::Vector2f center,sf::Vector2f size){
    sf::View view(center,size);
    window.setView(view);
}

sf::Vector2i MouseControls::mouseToCursorPos(sf::RenderWindow& window,sf::Vector2f screenSize){
    sf::Vector2i mousePos{sf::Mouse::getPosition(window)};
    sf::Vector2u size = window.getSize();
    sf::Vector2f viewCenter = window.getView().getCenter();
    mousePos.x /= (size.x/screenSize.x);
    mousePos.y /= (size.y/screenSize.y);
    mousePos.x += (viewCenter.x - screenSize.x/2);
    mousePos.y += (viewCenter.y - screenSize.y/2);

    return mousePos;
}

sf::Vector2i MouseControls::mouseToViewPortPos(sf::RenderWindow& window,sf::Vector2f screenSize){
    sf::Vector2i mousePos{sf::Mouse::getPosition(window)};
    sf::Vector2u size = window.getSize();
    mousePos.x /= (size.x/screenSize.x);
    mousePos.y /= (size.y/screenSize.y);

    return mousePos;
}




void MouseControls::SwitchMouse(sf::Event event){
    _mouseViewPortPos = mouseToViewPortPos(*_window, _viewportSize);
    // move cursor to mouse
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = mouseToCursorPos(*_window, _viewportSize);
        _cursor->SetPos(mousePos.x, mousePos.y);
    }
    // move viewport
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

        _center.x -= _mouseViewPortPos.x - _prevMouseViewPortPos.x;
        _center.y -= _mouseViewPortPos.y - _prevMouseViewPortPos.y;
    }
    // zoom
    if (event.type == sf::Event::MouseWheelMoved) {
        _viewportSize.x -= event.mouseWheel.delta * zoomModifier;
        _viewportSize.y -= event.mouseWheel.delta * zoomModifier;
    }

    // Cell placing
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        _grid
                ->GetCell(_cursor->GetX(),_cursor->GetY())
                .SetNextState(CellState::Alive);
    }
    if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
        _grid
                ->GetCell(_cursor->GetX(),_cursor->GetY())
                .SetNextState(CellState::Empty);
    }

    _prevMouseViewPortPos = _mouseViewPortPos;
    setViewSize(*_window,_center,_viewportSize);
}

