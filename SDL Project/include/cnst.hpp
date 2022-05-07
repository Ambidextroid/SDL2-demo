#pragma once

//constants used throughout the game
namespace cnst
{
    constexpr int WIN_W = 1000;     //program window dimenstions
    constexpr int WIN_H = 800;
    constexpr int TILE_SIZE = 50;   //size of tiles in pixels
    constexpr int TIME_STEP = 10;   //game simulation time step in milliseconds
    //animation modes
    constexpr int ANIM_REPEAT = 0;  //loop
    constexpr int ANIM_END = 1;     //play once and stop
    constexpr int ANIM_DEFAULT = 2; //play once, set animNo to 0 and loop

    constexpr int TERMINAL_VEL = 23;    //max downward velocity
}