//
// Created by egor0 on 4/23/2020.
//

#ifndef CPP_LIFE_CURSOR_H
#define CPP_LIFE_CURSOR_H

#include "WorldMember.h"
#include "SFML/Graphics.hpp"
class Cursor: public WorldMember{
public:
    Cursor();
    void CreateObjectTexture() override;
};

#endif //CPP_LIFE_CURSOR_H
