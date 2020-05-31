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


int MakeDivisibleBy(int value, int div){
    while(value%div!=0){
        value+=1;
    }
    return value;
}


void setViewSize(sf::RenderWindow &window,sf::Vector2f center,sf::Vector2f size){
    sf::View view(center,size);
    window.setView(view);
}


int main() {



    int threadCount = 8;
    // create window
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Game of life");
    ImGui::SFML::Init(window);

    // change window size to upscale pixels
    sf::RenderTexture buffer;
    int newScreenSize[2] = {160, 160};
    sf::Vector2f screenCenter{newScreenSize[0] / 2.f, newScreenSize[1] / 2.f};
    setViewSize(window, screenCenter, sf::Vector2f(newScreenSize[0], newScreenSize[1]));
    buffer.create(newScreenSize[0], newScreenSize[1]);
    sf::Sprite bufferSprite(buffer.getTexture());

    // create grid and get access to grid 2d array
    Grid world = Grid(newScreenSize[0], newScreenSize[1], "3/23", threadCount, buffer);
    Cursor cursor = Cursor(50, 50);

    // Create clock to count framerate
    sf::Clock Clock;
    sf::Clock deltaClock;
    float framerate;


    Controls controls(world, cursor, window);
    window.setFramerateLimit(controls.framerateLimit);
    // all for imgui
    float gridColor[3]{0.215, 0.215, 0.215};
    ImGui::FileBrowser fileBrowser;
    fileBrowser.SetTitle("Choose file");
    fileBrowser.Open();


    bool creatingNewGrid = false;


    while (window.isOpen()) {
        framerate = 1.f / Clock.getElapsedTime().asSeconds();
        Clock.restart();
        window.setTitle(std::to_string(framerate) + " " + "X: " + std::to_string(cursor.GetX()) + " Y:" +
                        std::to_string(cursor.GetY()));
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;


        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (!fileBrowser.IsOpened() && !ImGui::IsAnyWindowFocused()) controls.SwitchMouse(event);
            if (event.type == sf::Event::KeyPressed) controls.SwitchKeyboard(event);
        }
            ImGui::SFML::Update(window, deltaClock.restart());
            ImGui::SetNextWindowPos(ImVec2(276, 0), ImGuiCond_Always);
            world.logger->Draw("Logger");
            ImGui::SetNextWindowSize(ImVec2(275, 150), ImGuiCond_Always);
            ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
            ImGui::Begin("Game Of Life");
            ImGui::ColorEdit3("Grid Color", gridColor);

            if (ImGui::Button("Choose pattern")) {
                fileBrowser.Open();
            }

            fileBrowser.Display();

            if (fileBrowser.HasSelected()) {
                controls.SetPattern(OpenRLE_File(fileBrowser.GetSelected().string()));
                creatingNewGrid = true;
                newScreenSize[0] = MakeDivisibleBy(controls.GetPattern().point.x, threadCount);
                newScreenSize[1] = MakeDivisibleBy(controls.GetPattern().point.y, threadCount);
                int biggest = std::max(newScreenSize[0], newScreenSize[1]);
                newScreenSize[0] = biggest;
                newScreenSize[1] = biggest;
                screenCenter.x = newScreenSize[0] / 2.f;
                screenCenter.y = newScreenSize[1] / 2.f;
                fileBrowser.ClearSelected();

            }
            ImGui::End();
            if (creatingNewGrid) {
                world.SetPause(true);


                ImGui::Begin("Select grid size");
                ImGui::Text("Grid size is ");
                ImGui::SameLine();
                ImGui::TextColored(ImVec4(1, 1, 0, 1), "%i, %i", (int) newScreenSize[0], (int) newScreenSize[1]);

                bool sizeDividesByThreadCount = true;
                ImGui::Text("Change to:");

                ImGui::InputInt2("New Size", newScreenSize);
                if (newScreenSize[0] % threadCount != 0 || newScreenSize[1] % threadCount != 0) {
                    sizeDividesByThreadCount = false;
                    ImGui::TextColored(ImVec4(1, 0, 0, 1),
                                       "X/Y must be divided by the number of threads(%i) completely!", threadCount);
                } else {
                    sizeDividesByThreadCount = true;
                }

                if (ImGui::Button("OK") && sizeDividesByThreadCount) {

                    buffer.create(newScreenSize[0], newScreenSize[1]);
                    bufferSprite = sf::Sprite(buffer.getTexture());
                    setViewSize(window, screenCenter, sf::Vector2f(newScreenSize[0], newScreenSize[1]));
                    controls.UpdateMouseConfiguration();

                    world.ChangeRule(controls.GetPattern().rule);
                    world.ChangeSize(newScreenSize[0], newScreenSize[1]);
                    creatingNewGrid = false;
                }
                ImGui::End();

            }

            window.clear(sf::Color::Black);
            buffer.clear(sf::Color(gridColor[0] * 255, gridColor[1] * 255, gridColor[2] * 255));
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
