#include <iostream>

#include <SFML/Graphics.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"
#include "imgui/imfilebrowser.h"

#include "Grid.h"
#include "Cursor.h"
#include "RLE_Coder.h"
#include "Controls.h"
#include "Timer.h"






void setViewSize(sf::RenderWindow &window,sf::Vector2f center,sf::Vector2f size){
    sf::View view(center,size);
    window.setView(view);
}

int main()
{



    auto res = FromRLE("4b2o6b2o4b$3bobo6bobo3b$3bo10bo3b$2obo10bob2o$2obobo2b2o2bobob2o$3bobo\n"
                       "bo2bobobo3b$3bobobo2bobobo3b$2obobo2b2o2bobob2o$2obo10bob2o$3bo10bo3b$\n"
                       "3bobo6bobo3b$4b2o6b2o!");


    auto r = OpenRLE_File("C:\\Users\\egor0\\ClionProjects\\cpp_life\\test.rl");

    int threadCount = 8;
    // create window
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Game of life");
    window.setFramerateLimit(30);
    ImGui::SFML::Init(window);
    // change window size to upscale pixels
    sf::Vector2f newScreenSize{1000,1000};
//    sf::Vector2f newScreenSize{160,160};
    sf::Vector2f screenCenter{30,30};
    setViewSize(window,screenCenter, newScreenSize);
    sf::RenderTexture buffer;
    buffer.create(newScreenSize.x,newScreenSize.y);
    sf::Sprite bufferSprite(buffer.getTexture());

    // create grid and get access to grid 2d array
    Grid world = Grid(newScreenSize.x, newScreenSize.y, threadCount, buffer);
    Cursor cursor = Cursor(50,50);

    // Create clock to count framerate
    sf::Clock Clock;
    sf::Clock deltaClock;
    float framerate;

    bool isPaused = false;

    MouseControls controls(world, cursor, window);

    // all for imgui
    float gridColor[3]{0.215,0.215,0.215};
    ImGui::FileBrowser fileBrowser;
    fileBrowser.SetTitle("Choose file");


    world.logger.AddLog("Test",LogType::Info);
//    fileBrowser.SetTypeFilters({".rle",".txt"});


    Timer timer;

    while (window.isOpen())
    {
        framerate = 1.f / Clock.getElapsedTime().asSeconds();
        Clock.restart();
        window.setTitle(std::to_string(framerate)+" " +"X: " +std::to_string(cursor.GetX()) + " Y:" + std::to_string(cursor.GetY()));
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;


        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if(!fileBrowser.IsOpened() && !ImGui::IsAnyWindowFocused()) controls.SwitchMouse(event);
            
            if(event.type == sf::Event::KeyPressed) {

                // Start/Pause simulation
                if(event.key.code == sf::Keyboard::Space){
                    world.TogglePause();
                }

                // place smth
                if(event.key.code==sf::Keyboard::Z){
                    for(Point point:r.pattern){
                        world.GetCell(point.x,point.y).SetNextState(CellBehavior::Alive);
                    }
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        world.logger.Draw("TEstTitle");

        ImGui::Begin("Sample window");
        ImGui::ColorEdit3("Grid Color",gridColor);

        if(ImGui::Button("Choose pattern")){
            fileBrowser.Open();
        }

        fileBrowser.Display();

        if(fileBrowser.HasSelected())
        {
            std::cout << "Selected filename " << fileBrowser.GetSelected().string() << std::endl;
            r = OpenRLE_File(fileBrowser.GetSelected().string());
            fileBrowser.ClearSelected();
        }
        ImGui::End();

            window.clear(sf::Color::Black);
            buffer.clear(sf::Color(gridColor[0]*255,gridColor[1]*255,gridColor[2]*255));
            // calculate cells new state



            world.CalculateCells();


            world.DisplayCells();

            cursor.DrawTo(&buffer);
            buffer.display();
            window.draw(bufferSprite);

            // end the current frame
            ImGui::SFML::Render(window);
            window.display();

    }
    ImGui::SFML::Shutdown();
    return 0;
}