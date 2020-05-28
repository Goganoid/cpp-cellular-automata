//
// Created by egor0 on 4/21/2020.
//

#ifndef CPP_LIFE_GRID_H
#define CPP_LIFE_GRID_H

#include "Cell.h"
#include "iostream"
#include <vector>
#include <thread>
#include "ThreadPool.h"
#include "SFML/Graphics.hpp"
#include "OverflowArray.h"
#include "AppLog.h"
/*-------------------


 ------------------- */

class LookupRule;

class Grid{
private:
    int _threads;
    LookupRule* rule;
    int ** _ranges;
    bool _isPaused;
    int _width;
    int _height;
    sf::RenderTarget *  _screen;
    // saves CellBehaviour(alive or empty)
    OverflowArray<Cell> _grid;
    // saves cell vertices for drawing
    OverflowArray<OverflowArray<CellRect>> _rect_grid;
    std::vector<sf::Vertex> _cells_to_draw;
    ThreadPool * pool;
    std::vector<sf::Vertex> * storage;

    ///  Divides number into ranges
    ///  Example:
    /// @code
    /// int ** a = DivideGridIntoZones(3,18)
    /// //a=[[0,6],[6,12],[12,18]]
    /// @endcode
    ///  @param[in] zones number of ranges
    ///  @param[in] length number to divide
    /// @return 2D array
    static int ** DivideGridIntoZones(int zones, int length);

    /// Function that runs in threads
    /// Iterates through a part of the grid matrix
    /// that has the same height as the grid and a width equal to the length of the range
    /// <br> Updates  _cells_to_draw_coords
    /// @param[in] range [start_index,end_index] array to iterate through

    void CalculateZone(int id, int* range[2]);

    /// Function that runs in threads
    /// Iterates through a part of the grid matrix
    /// that has the same height as the grid and a width equal to the length of the range
    /// <br> Sets Cell state value to Cell nextState value
    ///  @param[in] range [start_index,end_index] array to iterate through
    void UpdateCellsStates(const int * range);

    std::string GetPauseInfo() const;


public:
    AppLog * logger;
    Grid(int width, int height, int threadsAmount,sf::RenderTarget& screen);
    ~Grid();

    Cell& GetCell(int x, int y);

    [[maybe_unused]] Cell& GetCell(int coords[2]);
    [[maybe_unused]] Cell& GetCell(std::vector<int> & coords);
    [[nodiscard]] int GetHeight() const { return _height;}
    [[nodiscard]] int GetWidth() const  { return _width;}
    void CalculateCells();
    void DisplayCells();

    [[maybe_unused]] void SetPause(bool state);

    [[maybe_unused]] bool IsPaused() const;
    void TogglePause();

};
#endif //CPP_LIFE_GRID_H
