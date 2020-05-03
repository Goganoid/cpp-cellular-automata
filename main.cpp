#include <iostream>
#include <SFML/Graphics.hpp>
#include "Grid.h"
#include "Cursor.h"
#include "RLE_Coder.h"
#include "Controls.h"

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
    mousePos.x += (viewCenter.x - screenSize.x/2);
    mousePos.y += (viewCenter.y - screenSize.y/2);

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


    bool worldPause = true;


    Controls controls(grid,cursor,window);

    while (window.isOpen())
    {
        framerate = 1.f / Clock.getElapsedTime().asSeconds();
        Clock.restart();
        window.setTitle(std::to_string(framerate)+" " +"X: " +std::to_string(cursor.GetX()) + " Y:" + std::to_string(cursor.GetY()));
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;


        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();

            controls.SwitchMouse(event);
            
            if(event.type == sf::Event::KeyPressed) {

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

//            setViewSize(window,screenCenter, newScreenSize);
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