#pragma once

//constants used throughout the game
namespace cnst
{
    constexpr int WIN_W = 1000;     //program window dimenstions
    constexpr int WIN_H = 800;
    constexpr int TIME_STEP = 10;   //game simulation time stp in milliseconds, default is 10
    constexpr int TILE_SIZE = 50;   //size of tiles in pixels
    constexpr int ANIM_DEF = 0;     //animation modes: DEF plays the animation once then returns to the default animation
    constexpr int ANIM_END = 1;     //END plays the animation once and then stops
    constexpr int ANIM_REP = 2;     //REP repeats the animation
}