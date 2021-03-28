#pragma once
#include "Ant.h"

class AntColonyOptimization : public olc::PixelGameEngine
{
public:
    AntColonyOptimization();

public:
    bool OnUserCreate() override;
    bool OnUserUpdate( float fElapsedTime ) override;

private:
    void composeFrame();
    
};