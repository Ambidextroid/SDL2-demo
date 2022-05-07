#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "../include/cnst.hpp"
#include "../include/media.hpp"
#include "../include/entity.hpp"

class Player;
class ActiveEntity;

class Map
{
public:
    Map(Media *media);

    int getTile(int x, int y);
    void setTile(int x, int y, int tile);

    void updateCamera(Player *player);
    void shakeCamera(float cameraShake);

    void drawMap();
    void drawParralax();

    //check if the player will collide with a given tile in a given direction
    bool checkCollision(char dir, ActiveEntity *entity, int tile);

private:
    std::vector<std::vector<int>> map;
    std::vector<std::vector<SDL_Rect>> cameraZones;
    SDL_Rect currentZone;
    int mapH, mapW;
    std::vector<SDL_Texture *> mapTextures;
    float cameraShake;
};