//
// Created by egor0 on 5/3/2020.
//
#include "Controls.h"

#include <utility>
Controls::Controls(Grid& grid, Cursor& cursor, sf::RenderWindow& window){
_grid = &grid;
_cursor = &cursor;
_window = &window;
    UpdateMouseConfiguration();
_mouseViewPortPos = mouseToViewPortPos(*_window, _viewportSize);
_prevMouseViewPortPos = _mouseViewPortPos;
}

void Controls::setViewSize(sf::RenderWindow &window, sf::Vector2f center, sf::Vector2f size){
    sf::View view(center,size);
    window.setView(view);
}

void Controls::UpdateMouseConfiguration() {
    _center = _window->getView().getCenter();
    _viewportSize = _window->getView().getSize();
}

sf::Vector2i Controls::mouseToCursorPos(sf::RenderWindow& window, sf::Vector2f screenSize){
    sf::Vector2i mousePos{sf::Mouse::getPosition(window)};
    sf::Vector2u size = window.getSize();
    sf::Vector2f viewCenter = window.getView().getCenter();
    mousePos.x /= (size.x/screenSize.x);
    mousePos.y /= (size.y/screenSize.y);
    mousePos.x += (viewCenter.x - screenSize.x/2);
    mousePos.y += (viewCenter.y - screenSize.y/2);

    return mousePos;
}

sf::Vector2i Controls::mouseToViewPortPos(sf::RenderWindow& window, sf::Vector2f screenSize){
    sf::Vector2i mousePos{sf::Mouse::getPosition(window)};
    sf::Vector2u size = window.getSize();
    mousePos.x /= (size.x/screenSize.x);
    mousePos.y /= (size.y/screenSize.y);

    return mousePos;
}




void Controls::SwitchMouse(sf::Event event){
    _mouseViewPortPos = mouseToViewPortPos(*_window, _viewportSize);
    // move cursor to mouse
    if (event.type == sf::Event::MouseMoved) {
        sf::Vector2i mousePos = mouseToCursorPos(*_window, _viewportSize);
        _cursor->SetPos(mousePos.x, mousePos.y);
    }
    // move viewport
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        _center.x -= (float)_mouseViewPortPos.x - _prevMouseViewPortPos.x;
        _center.y -= (float)_mouseViewPortPos.y - _prevMouseViewPortPos.y;
    }
    // zoom
    if (event.type == sf::Event::MouseWheelMoved) {
        _viewportSize.x -= (float)event.mouseWheel.delta * zoomModifier;
        _viewportSize.y -= (float)event.mouseWheel.delta * zoomModifier;
    }

    // Cell placing
    if(_grid->InBounds(_cursor->GetX(),_cursor->GetY())) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {

            _grid
                    ->GetCell(_cursor->GetX(), _cursor->GetY())
                    .SetNextState(CellBehavior::Alive);
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            _grid
                    ->GetCell(_cursor->GetX(), _cursor->GetY())
                    .SetNextState(CellBehavior::Empty);
        }
    }
    _prevMouseViewPortPos = _mouseViewPortPos;
    setViewSize(*_window,_center,_viewportSize);
}

void Controls::SwitchKeyboard(sf::Event event) {
    // Start/Pause simulation
    if(event.key.code == sf::Keyboard::Space){
        _grid->TogglePause();
    }

    // spawn pattern
    if(event.key.code==sf::Keyboard::Z){
        _grid->Erase();
        for(Point point:_pattern.pattern){
            _grid->GetCell(point.x,point.y).SetNextState(CellBehavior::Alive);
        }
    }
    if(event.key.code==sf::Keyboard::X){
        _grid->Erase();
    }

    if(event.key.code==sf::Keyboard::Add || event.key.code==sf::Keyboard::Equal){
        framerateLimit+=5;
        _window->setFramerateLimit(framerateLimit);
    }
    if(event.key.code==sf::Keyboard::Subtract|| event.key.code==sf::Keyboard::Dash){
        if(framerateLimit>=5) framerateLimit-=5;
        else framerateLimit = 1;
        _window->setFramerateLimit(framerateLimit);
    }
}

void Controls::SetPattern(RLEReadResult pattern) {
    _pattern = std::move(pattern);
}
RLEReadResult const&  Controls::GetPattern() const {
    return _pattern;
}
