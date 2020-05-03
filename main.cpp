#include <iostream>
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Cursor.h"
#include "RLE_Coder.h"

void setViewSize(sf::RenderWindow &window,sf::Vector2f center,sf::Vector2f size){
    sf::View view(center,size);
    window.setView(view);
}

sf::Vector2i mouseToCursorPos(sf::RenderWindow& window,sf::Vector2f screenSize){
    sf::Vector2i mousePos{sf::Mouse::getPosition(window)};
    sf::Vector2u size = window.getSize();
    sf::Vector2f viewCenter = window.getView().getCenter();
    mousePos.x /= (size.x/screenSize.x);
    mousePos.y /= (size.y/screenSize.y);
    mousePos.x += (viewCenter.x- screenSize.x/2);
    mousePos.y += (viewCenter.y- screenSize.y/2);

    return mousePos;
}

sf::Vector2i mouseToViewPortPos(sf::RenderWindow& window,sf::Vector2f screenSize){
    sf::Vector2i mousePos{sf::Mouse::getPosition(window)};
    sf::Vector2u size = window.getSize();
    mousePos.x /= (size.x/screenSize.x);
    mousePos.y /= (size.y/screenSize.y);

    return mousePos;
}

int main()
{
//    auto res = FromRLE("bo$2bo$3o!",0,0);
    auto res = FromRLE("4b2o6b2o4b$3bobo6bobo3b$3bo10bo3b$2obo10bob2o$2obobo2b2o2bobob2o$3bobo\n"
                       "bo2bobobo3b$3bobobo2bobobo3b$2obobo2b2o2bobob2o$2obo10bob2o$3bo10bo3b$\n"
                       "3bobo6bobo3b$4b2o6b2o!",0,0);



    int threadCount = 2;
    // create window
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Game of life");
    window.setFramerateLimit(30);

    // change window size to upscale pixels
    sf::Vector2f newScreenSize{100,100};
    sf::Vector2f screenCenter{30,30};
//    sf::View view(sf::Vector2f(newScreenSize.x/2,newScreenSize.y/2), newScreenSize);
//    window.setView(view);
//    setViewSize(window,sf::Vector2f(newScreenSize.x/2,newScreenSize.y/2), newScreenSize);
    setViewSize(window,screenCenter, newScreenSize);
    sf::RenderTexture buffer;
    buffer.create(newScreenSize.x,newScreenSize.y);
    sf::Sprite bufferSprite(buffer.getTexture());

    // create grid and get access to grid 2d array
    Grid grid = Grid(newScreenSize.x,newScreenSize.y,threadCount,buffer);
    Cursor cursor = Cursor(50,50);

    // Create clock to count framerate
    sf::Clock Clock;
    float framerate;

    float zoomModificator = 2;

    bool worldPause = true;



    sf::Vector2i mouseViewPortPos = mouseToViewPortPos(window,newScreenSize);
    sf::Vector2i prevMouseViewPortPos = mouseViewPortPos;
    while (window.isOpen())
    {
        framerate = 1.f / Clock.getElapsedTime().asSeconds();
        Clock.restart();
        window.setTitle(std::to_string(framerate)+" " +"X: " +std::to_string(cursor.GetX()) + " Y:" + std::to_string(cursor.GetY()));
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;


        while (window.pollEvent(event))
        {
            mouseViewPortPos = mouseToViewPortPos(window, newScreenSize);
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            // move cursor to mouse
            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = mouseToCursorPos(window, newScreenSize);
                cursor.SetPos(mousePos.x, mousePos.y);
            }
            // move viewport
            if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {

                screenCenter.x -= (mouseViewPortPos.x-prevMouseViewPortPos.x);
                screenCenter.y += (mouseViewPortPos.y-prevMouseViewPortPos.y);
            }
            // zoom
            if (event.type == sf::Event::MouseWheelMoved) {
                std::cout << event.mouseWheel.delta << std::endl;
                newScreenSize.x -= event.mouseWheel.delta * zoomModificator;
                newScreenSize.y -= event.mouseWheel.delta * zoomModificator;
            }

            // Cell placing
            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
                grid
                    .GetCell(cursor.GetX(),cursor.GetY())
                    .SetNextState(CellState::Alive);
            }
            if(sf::Mouse::isButtonPressed(sf::Mouse::Right)){
                grid
                        .GetCell(cursor.GetX(),cursor.GetY())
                        .SetNextState(CellState::Empty);
            }


            prevMouseViewPortPos = mouseViewPortPos;
            // deprecated controls
            // TODO remove
            if(event.type == sf::Event::KeyPressed) {
                // Cursor Movement
                if(event.key.code == sf::Keyboard::A){
                    cursor.Move(-1,0);
                }
                if(event.key.code == sf::Keyboard::D){
                    cursor.Move(+1,0);
                }
                if(event.key.code == sf::Keyboard::S){
                    cursor.Move(0,+1);
                }
                if(event.key.code == sf::Keyboard::W){
                    cursor.Move(0,-1);
                }
                // Cell Placing and Destruction
                if(event.key.code == sf::Keyboard::O){
                    grid
                    .GetCell(cursor.GetX(),cursor.GetY())
                    .SetNextState(CellState::Alive);

                }
                if(event.key.code == sf::Keyboard::P){
                    grid
                    .GetCell(cursor.GetX(),cursor.GetY())
                    .SetNextState(CellState::Empty);
                }
                // Start/Pause simulation
                if(event.key.code == sf::Keyboard::Space){
                    grid.TogglePause();
                }
                // Pause everything
                if(event.key.code == sf::Keyboard::M){
                    worldPause= !worldPause;
                }
                // place smth
                if(event.key.code==sf::Keyboard::Z){
                    for(int y=0;y<res.size();y++){
                        for(int x=0;x<res[y].size();x++){
                            grid.GetCell(x+cursor.GetX(),y+cursor.GetY()).SetNextState(res[y][x]);
                        }
                    }
                }
            }
        }
        if(worldPause) {

            setViewSize(window,screenCenter, newScreenSize);
            window.clear(sf::Color::Black);
            buffer.clear(sf::Color::Red);
            // calculate cells new state
            grid.CalculateCells(threadCount);
            grid.DisplayCells();


            cursor.DrawTo(&buffer);

            buffer.display();
            window.draw(bufferSprite);

            // end the current frame
            window.display();
        }
    }

    return 0;
}