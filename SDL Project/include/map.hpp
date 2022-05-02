#pragma once

#include "../include/cnst.hpp"
#include "../include/entity.hpp"
#include "../include/renderwindow.hpp"

//struct used for storing and drawing the map and its textures
struct Map
{
    int mapH, mapW; //map dimensions
    SDL_Texture *floorTex, *bgTex, *windowTex, *doorTex;  //textures
    Entity *key, *door;    //key and door entities
    std::vector<std::vector<int>> map;  //the map
    std::vector<std::vector<SDL_Rect>> rooms;   //rects determining zones used to set the camera
    SDL_Rect currentRoom;   //whichever room the camera is currently bound to


    Map()
    :floorTex(NULL), bgTex(NULL), windowTex(NULL), doorTex(NULL), key(NULL), door(NULL), map{
            {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
            {3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3},
            {3,1,1,1,1,1,1,1,1,1,1,1,2,0,0,1,1,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,1,3,1,3},
            {3,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,6,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3},
            {3,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,1,3,1,3},
            {3,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,1,1,3},
            {3,1,1,1,1,1,1,1,1,1,1,1,0,0,0,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3},
            {3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3},
            {3,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,3,3,1,1,1,1,1,1,3},
            {3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,1,1,1,1,1,3,3,1,1,1,2,0,0,1,3},
            {3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,1,1,1,1,1,1,1,1,1,0,0,0,1,3},
            {3,1,1,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,1,1,1,3,3,3,1,1,1,1,1,1,1,1,0,0,0,1,3},
            {3,1,3,3,3,3,3,1,1,1,3,3,3,1,1,1,1,1,1,3,3,1,3,1,1,3,3,3,1,1,1,1,1,1,1,0,0,0,1,3},
            {3,1,3,3,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,1,3,3,1,1,3,3,3,1,1,1,1,1,1,0,0,0,1,3},
            {3,1,3,3,3,3,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,1,1,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3},
            {3,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
            {3,1,3,1,2,0,0,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
            {3,1,3,1,0,0,0,1,1,1,1,3,3,3,1,1,1,1,1,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3},
            {3,1,3,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3},
            {3,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,3,3,3,3,3,1,1,2,0,0,1,1,1,1,1,1,1,1,2,0,0,1,1,3},
            {3,1,3,1,0,0,0,1,1,1,1,1,1,1,1,1,3,3,3,3,3,1,1,0,0,0,1,1,1,1,1,5,1,1,0,0,0,1,1,3},
            {3,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,1,1,0,0,0,1,1,1,1,3,3,3,1,0,0,0,1,3,3},
            {3,1,3,3,3,3,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,3},
            {3,1,1,1,1,1,1,1,1,1,3,3,3,3,3,3,3,3,3,3,3,1,1,0,0,0,1,1,1,1,1,1,1,1,0,0,0,1,1,3},
            {3,1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,3,3,1,1,3,3,3,1,1,1,1,1,1,1,1,3,3,3,1,1,3},
            {3,1,3,1,1,1,1,1,3,3,3,1,2,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,1,1,1,1,1,1,1,1,3},
            {3,1,3,3,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,3,1,1,1,1,1,1,1,1,3},
            {3,1,3,3,3,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,1,1,1,1,3,3,3,3},
            {3,1,1,1,3,3,1,1,1,1,1,1,0,0,0,1,3,3,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,3},
            {3,1,1,1,3,3,3,1,1,1,1,1,0,0,0,1,3,3,1,1,1,1,1,1,1,1,3,3,1,1,1,1,1,1,1,1,1,1,1,3},
            {3,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,3,3,1,1,1,1,1,1,1,1,3,3,1,1,1,1,1,1,3,3,1,1,1,3},
            {3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3}
        }, rooms{   
               //first rect represents the whole room used by the camera, all following rects represent camera trigger zones for that room
            { {0, 0, 1000, 1600}, {0, 0, 1000, 1600} },
            { {1000, 0, 1000, 800}, {1000, 0, 1000, 800} },
            { {0, 800, 2000, 800}, {500, 1200, 1500, 400} }
        }
    {
        mapH = map.size();
        mapW = map.at(0).size();
        currentRoom = rooms.at(0).at(0);
    }

    
    //draw the map and player to screen
    void drawMap(RenderWindow* window, int offsetX, int offsetY)
    {
        int visTilesX = cnst::WIN_W/cnst::TILE_SIZE;  //number of tiles that can fit on the screen
        int visTilesY = cnst::WIN_H/cnst::TILE_SIZE;

        int tileOffsetX = offsetX % cnst::TILE_SIZE; //offset the position of the tiles so they scroll smoothly
        int tileOffsetY = offsetY % cnst::TILE_SIZE;

        for (int y = -5; y <= visTilesY+10; y++) //loop through a grid of tile-sized squares which covers the screen, plus margins for larger tiles
        {
            for (int x = -5; x <= visTilesX+10; x++)
            {
                int tile = getTile(x + offsetX/cnst::TILE_SIZE, y + offsetY/cnst::TILE_SIZE); //retrieve each tile from the array corresponding to the visible portion of the map
                //brick tiles repeat in a pattern of 6, plus a squared Y value to introduce pseudorandomness to the pattern
                int image = (x + offsetX/cnst::TILE_SIZE + (y + offsetY/cnst::TILE_SIZE)*(y + offsetY/cnst::TILE_SIZE)) % 6;
                switch(tile)
                {
                case 1: //if the tile is a 1, render background tiles
                    {
                        Entity background(x * cnst::TILE_SIZE, y * cnst::TILE_SIZE, bgTex, NULL, image);    //create background entitiy
                        window->render(background, -tileOffsetX, -tileOffsetY);  //render tile with offset applied
                    }   //scope to clear floor entities after they have been rendered
                    break;
                case 2: //window
                    {
                        std::vector<std::vector<SDL_Rect>> windowClips {window->makeClips(0, 1, 150, 250)};
                        Entity windowframe(x * cnst::TILE_SIZE, y * cnst::TILE_SIZE, windowTex, &windowClips, 0);    //create floor entitiy
                        window->render(windowframe, -tileOffsetX, -tileOffsetY);  //render tile with offset applied
                    }   
                    break;
                case 3: //floor tiles
                    {
                        Entity floor(x * cnst::TILE_SIZE, y * cnst::TILE_SIZE, floorTex, NULL, image);    //create floor entitiy
                        window->render(floor, -tileOffsetX, -tileOffsetY);  //render tile with offset applied
                    }   
                    break;
                case 5: //key (draw background and move key to location)
                    {
                        Entity background(x * cnst::TILE_SIZE, y * cnst::TILE_SIZE, bgTex, NULL, image);    //create background entitiy
                        window->render(background, -tileOffsetX, -tileOffsetY);  //render tile with offset applied
                    }   
                    key->x = x*cnst::TILE_SIZE + offsetX -tileOffsetX;  //update key position
                    key->y = y*cnst::TILE_SIZE + offsetY -tileOffsetY + 5.0f*sin(SDL_GetTicks()/300.0f);
                    break;
                case 6: //sliding door (draw background and move door to location)
                    {
                        Entity floor(x * cnst::TILE_SIZE, y * cnst::TILE_SIZE, floorTex, NULL, image);    //create floor entitiy
                        window->render(floor, -tileOffsetX, -tileOffsetY);  //render tile with offset applied
                    }
                    door->x = x*cnst::TILE_SIZE + offsetX -tileOffsetX;  //update door position
                    door->y = y*cnst::TILE_SIZE + offsetY -tileOffsetY;
                    break;
                }
            }
        }
    }

    int getTile(int x, int y)   //get tile ID from map
    {
        if (x >= 0 && x < mapW && y >= 0 && y < mapH) return map.at(y).at(x);
        else return -1;
    }

    bool checkCollision(char dir, Player* player, int tile)   //check if the player is colliding with a given tile in a given direction
    {
        //add player's velocity to their position and check if they will overlap a given tile
        //as the player's position is given by the top left coordinate, both the tile next to the player and the tile above that tile have to be ckecked
        if (dir == 'r' && (getTile((player->x+player->velX)/cnst::TILE_SIZE + 1, player->y/cnst::TILE_SIZE) == tile || getTile((player->x+player->velX)/cnst::TILE_SIZE + 1, player->y/cnst::TILE_SIZE + 0.99) == tile))
            return true;
        else if (dir == 'l' && (getTile((player->x+player->velX)/cnst::TILE_SIZE, player->y/cnst::TILE_SIZE) == tile || getTile((player->x+player->velX)/cnst::TILE_SIZE, player->y/cnst::TILE_SIZE + 0.99) == tile))
            return true;
        else if (dir == 'd' && (getTile((player->x+player->velX)/cnst::TILE_SIZE, (player->y+player->velY)/cnst::TILE_SIZE + 1) == tile || getTile((player->x+player->velX)/cnst::TILE_SIZE + 0.99, (player->y+player->velY)/cnst::TILE_SIZE + 1) == tile))
            return true;
        else if (dir == 'u' && (getTile((player->x+player->velX)/cnst::TILE_SIZE, (player->y+player->velY)/cnst::TILE_SIZE) == tile || getTile((player->x+player->velX)/cnst::TILE_SIZE + 0.99, (player->y+player->velY)/cnst::TILE_SIZE) == tile))
            return true;
        else return false;
    }

    SDL_Rect updateCamZone(Player* player)  //check if the player has entered a camera trigger zone and update the current room
    {
        int centrePlayerX = player->x + cnst::TILE_SIZE/2.0f;   //coordinates of centre of player
        int centrePlayerY = player->y + cnst::TILE_SIZE/2.0f;
        for (int r = 0; r < rooms.size(); r++)  //for each room
        {
            for (int z = 1; z < rooms.at(r).size(); z++)    //for each cam zone in that room
            {
                if (centrePlayerX >= rooms.at(r).at(z).x && centrePlayerX <= rooms.at(r).at(z).x + rooms.at(r).at(z).w && centrePlayerY >= rooms.at(r).at(z).y && centrePlayerY <= rooms.at(r).at(z).y + rooms.at(r).at(z).h)
                //if player position is inside zone
                {
                    currentRoom = rooms.at(r).at(0);    //set corresponding room
                }
            }
        }
    }
};