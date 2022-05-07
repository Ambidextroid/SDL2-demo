#include "../include/map.hpp"


Map::Map(Media *media, int *offsetX, int *offsetY, int *targetOffsetX, int *targetOffsetY)
:map{
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,3,0,0,2,2,2,2,2,2,3,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,1},
    {1,2,0,0,0,2,2,2,2,2,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,1},
    {1,2,0,0,0,2,2,2,2,2,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,1},
    {1,2,0,0,0,2,2,2,2,2,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,2,2,2,1,1,2,2,2,1},
    {1,2,0,0,0,2,2,2,2,2,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,2,2,2,2,2,2,1,1,1,2,2,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,3,0,0,2,2,2,2,2,2,2,2,2,3,0,0,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,2,2,2,2,2,1,1,1,2,0,0,0,2,1},
    {1,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,2,2,2,2,2,2,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,2,1},
    {1,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,1,1,1,1,2,2,2,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,2,1},
    {1,2,2,1,1,2,2,2,2,2,1,2,1,1,1,2,1,1,1,1,2,2,2,0,0,0,2,2,1,1,2,2,2,2,2,0,0,0,2,1},
    {1,2,2,1,1,2,2,2,2,2,1,2,1,2,2,2,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,1,2,1,2,1,2,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,1,1,2,2,2,1,1,1,2,1,2,1,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,1},
    {1,1,1,2,2,2,2,2,2,2,1,2,1,1,1,1,1,1,2,2,2,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,1,2,1,1,1,1,1,1,2,2,2,2,2,1,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,1},
    {1,2,3,0,0,2,2,2,2,1,1,2,1,2,2,2,2,2,2,2,1,1,1,1,2,0,2,0,2,1,1,1,2,2,2,2,2,2,2,1},
    {1,2,0,0,0,2,2,2,2,1,1,2,1,2,1,1,2,1,1,1,1,2,2,1,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,1},
    {1,2,0,0,0,2,2,2,2,2,1,2,1,2,2,1,2,2,2,2,2,2,2,1,0,2,2,2,0,1,2,2,2,2,2,2,2,2,2,1},
    {1,2,0,0,0,2,1,1,2,2,1,2,1,1,1,1,1,1,1,1,1,1,2,1,2,0,0,0,2,1,2,2,2,2,2,2,1,1,2,1},
    {1,2,0,0,0,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,2,2,2,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,2,2,2,2,2,1,1,1,1,2,2,2,2,2,2,1,2,1},
    {1,2,3,0,0,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,2,2,3,0,0,2,2,1,1,2,2,2,2,2,1,1,1,2,1},
    {1,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,2,2,0,0,0,2,2,1,1,2,2,2,2,1,1,1,1,2,1},
    {1,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,0,0,0,2,2,1,1,2,2,2,2,2,2,2,2,2,1},
    {1,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,0,0,0,2,2,1,1,2,2,2,2,2,2,2,2,2,1},
    {1,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,0,0,0,2,2,1,1,1,1,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,2,2,2,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,1,1,1,2,2,2,2,2,2,2,1,1,2,3,0,0,2,2,2,2,2,1},
    {1,2,3,0,0,2,2,2,1,1,2,2,2,2,3,0,0,2,2,1,1,2,2,2,2,2,2,2,1,1,2,0,0,0,2,1,1,1,2,1},
    {1,2,0,0,0,2,2,2,2,1,1,2,2,2,0,0,0,2,2,1,1,2,2,1,1,1,1,1,1,1,2,0,0,0,2,2,2,2,2,1},
    {1,2,0,0,0,2,2,2,2,1,1,1,2,2,0,0,0,2,2,1,1,2,2,2,2,2,2,2,2,2,2,0,0,0,2,2,2,2,1,1},
    {1,2,0,0,0,2,2,2,2,1,1,1,1,2,0,0,0,2,1,1,1,2,2,2,2,2,2,2,2,2,2,0,0,0,2,2,2,2,2,1},
    {1,2,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,1,2,2,2,2,2,2,2,1,1,1,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,3,0,0,2,2,2,2,2,2,2,2,3,0,0,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,0,0,0,2,2,1,1,1,1,2,2,0,0,0,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,1,1,2,2,0,0,0,2,2,2,2,2,2,2,2,0,0,0,2,2,1},
    {1,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0,0,2,2,2,2,2,2,2,2,0,0,0,2,2,1},
    {1,2,2,1,1,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,2,0,0,0,2,2,2,2,2,2,2,2,0,0,0,2,2,1},
    {1,2,2,1,1,2,2,2,2,2,1,1,1,2,2,2,2,2,1,1,1,2,2,1,1,1,2,2,2,2,2,2,2,2,1,1,1,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,2,2,2,2,1,1,2,2,3,0,0,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,1,1,2,2,2,2,2,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,1},
    {1,2,2,2,2,2,1,1,2,2,2,2,2,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,1},
    {1,1,1,2,2,2,1,1,2,2,2,2,2,2,0,0,0,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,2,2,1,1,2,2,1},
    {1,1,1,2,2,2,2,2,2,2,2,2,2,2,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
}, cameraZones{
    { {0, 0, 2000, 1200}, {1500, 400, 500, 400} },          //first rect is camera zone, all following rects are camera triggers for that zone
    { {1000, 400, 1000, 1600}, {1500, 800, 500, 1200} },
    { {1000, 1200, 1000, 800}, {1000, 1200, 500, 800} },
    { {0, 1200, 1000, 1600}, {0, 1200, 1000, 1200} },
    { {0, 2000, 2000, 800}, {500, 2400, 500, 400}, {1500, 2000, 500, 400} }
}, mapTextures{
    media->starTex, media->brickFloorTex, media->brickBackgroundTex, media->windowTex
}, cameraShake(0), offsetX(offsetX), offsetY(offsetY), targetOffsetX(targetOffsetX), targetOffsetY(targetOffsetY)
{
    currentZone = cameraZones.at(0).at(0);
    mapH = map.size();
    mapW = map.at(0).size();
    *offsetX = 0;
    *offsetY = 1200;
}

int Map::getTile(int x, int y)
{
    if (x >= 0 && x < mapW && y >= 0 && y < mapH) return map.at(y).at(x);
    else return -1;
}
void Map::setTile(int x, int y, int tile)
{
    if (x >= 0 && x < mapW && y >= 0 && y < mapH) map.at(y).at(x) = tile;
}

void Map::updateCamera(Player *player)
{
    //check if player is in camera trigger and update camera zone
    int centrePlayerX = player->getX() + cnst::TILE_SIZE/2.0f;          //coordinates of centre of player
    int centrePlayerY = player->getY() + cnst::TILE_SIZE/2.0f;
    for (int zone = 0; zone < cameraZones.size(); zone++)               //for each camera zone
    {
        for (int trig = 1; trig < cameraZones.at(zone).size(); trig++)  //for each trigger for that zone
        {
            if (centrePlayerX >= cameraZones.at(zone).at(trig).x && centrePlayerX <= cameraZones.at(zone).at(trig).x + cameraZones.at(zone).at(trig).w
            && centrePlayerY >= cameraZones.at(zone).at(trig).y && centrePlayerY <= cameraZones.at(zone).at(trig).y + cameraZones.at(zone).at(trig).h)
            //if player position is inside zone
            {
                currentZone = cameraZones.at(zone).at(0);   //set corresponding room
            }
        }
    }

    //update camera position
    *targetOffsetX = player->getX() - cnst::WIN_W/2 + cnst::TILE_SIZE/2; //camera target is set so that player is centred on screen
    *targetOffsetY = player->getY() - cnst::WIN_H/2 + cnst::TILE_SIZE/2;
    
    if (*targetOffsetX < currentZone.x) *targetOffsetX = currentZone.x;   //clamp offset at borders so camera stops at the side of a room
    if (*targetOffsetY < currentZone.y) *targetOffsetY = currentZone.y;
    if (*targetOffsetX > currentZone.x + currentZone.w - cnst::WIN_W) *targetOffsetX = currentZone.x + currentZone.w - cnst::WIN_W;
    if (*targetOffsetY > currentZone.y + currentZone.h - cnst::WIN_H) *targetOffsetY = currentZone.y + currentZone.h - cnst::WIN_H;

    //smooth camera movement by approaching target with geometric series
    const float ratio = 0.08;                               //ratio used to determine the speed of the smooth camera movement
    int distanceX = *targetOffsetX - *offsetX;              //distance camera is from target
    int distanceY = *targetOffsetY - *offsetY;
    if (distanceX > 1) *offsetX += distanceX * ratio + 1;      //move camera by distance*ratio, plus at least one pixel
    else if (distanceX < -1) *offsetX += distanceX * ratio - 1;
    else *offsetX = *targetOffsetX;                               //truncate small distances

    if (distanceY > 1) *offsetY += distanceY * ratio + 1;
    else if (distanceY < -1) *offsetY += distanceY * ratio - 1;
    else *offsetY = *targetOffsetY;

    if (cameraShake)
    {
        cameraShake *= 0.95;                                //decrease shake amplitude
        *offsetX += sin(SDL_GetTicks()/10) * cameraShake;    //sin(time) * amplitude
        if (abs(cameraShake) < 1.0f) cameraShake = 0;       //truncate small values
    }
}

void Map::shakeCamera(float cameraShake) { this-> cameraShake = cameraShake; }

void Map::drawMap(SDL_Renderer *renderer)
{
    int tileOffsetX = *offsetX % cnst::TILE_SIZE;   //offset the position of the tiles so they scroll smoothly
    int tileOffsetY = *offsetY % cnst::TILE_SIZE;

    for (int y = -5; y <= cnst::WIN_H/cnst::TILE_SIZE + 10; y++) //loop through visible tiles, plus an off-screen margin of 5 tiles
    {
        for (int x = -5; x <= cnst::WIN_W/cnst::TILE_SIZE + 10; x++)
        {
            int tileID = getTile(x + *offsetX/cnst::TILE_SIZE, y + *offsetY/cnst::TILE_SIZE);
            //brick tiles repeat in a pattern of 6, plus a squared Y value to introduce pseudorandomness to the pattern
            int brickPattern = (x + *offsetX/cnst::TILE_SIZE + (y +*offsetY/cnst::TILE_SIZE)*(y + *offsetY/cnst::TILE_SIZE)) % 6;
            switch(tileID)
            {
            case 1: //solid bricks
            case 2: //background bricks
                {
                    Entity tile(x * cnst::TILE_SIZE, y * cnst::TILE_SIZE, mapTextures.at(tileID), cnst::TILE_SIZE, cnst::TILE_SIZE, brickPattern);
                    tile.render(-tileOffsetX, -tileOffsetY, renderer);
                }   //scope closed to clear tile entities after they have been rendered
                break;
            case 3: //window
                {
                    Entity windowframe(x * cnst::TILE_SIZE, y * cnst::TILE_SIZE, mapTextures.at(tileID), 150, 250, 0);
                    windowframe.render(-tileOffsetX, -tileOffsetY, renderer);  //render tile with offset applied
                }   
                break;
            }
        }
    }
}

void Map::drawParralax(SDL_Renderer *renderer)
{

    int factor = 5;     //stars move a proportion of offset to produce parralax effect
    Entity star(0, 0, mapTextures.at(0), cnst::WIN_W, cnst::WIN_H, 0);
    //screen is covered with a 2 by 2 tiling of stars which wrap around once they are off screen
    star.render(-*offsetX/factor % cnst::WIN_W + cnst::WIN_W , -*offsetY/factor % cnst::WIN_H + cnst::WIN_H, renderer);
    star.render(-*offsetX/factor % cnst::WIN_W               , -*offsetY/factor % cnst::WIN_H + cnst::WIN_H, renderer);
    star.render(-*offsetX/factor % cnst::WIN_W + cnst::WIN_W , -*offsetY/factor % cnst::WIN_H, renderer);
    star.render(-*offsetX/factor % cnst::WIN_W               , -*offsetY/factor % cnst::WIN_H, renderer);
}

//check if the player will collide with a given tile in a given direction
bool Map::checkCollision(char dir, ActiveEntity *entity, int tile)   
{
    if (dir == 'r' && (getTile( (entity->getX() + entity->getXVel())/cnst::TILE_SIZE + 1, entity->getY()/cnst::TILE_SIZE ) == tile || getTile( (entity->getX() + entity->getXVel())/cnst::TILE_SIZE + 1, entity->getY()/cnst::TILE_SIZE + 0.99 ) == tile))
        return true;
    else if (dir == 'l' && (getTile( (entity->getX() + entity->getXVel())/cnst::TILE_SIZE, entity->getY()/cnst::TILE_SIZE ) == tile || getTile( (entity->getX() + entity->getXVel())/cnst::TILE_SIZE, entity->getY()/cnst::TILE_SIZE + 0.99 ) == tile))
        return true;
    else if (dir == 'd' && (getTile( (entity->getX() + entity->getXVel())/cnst::TILE_SIZE, (entity->getY() + entity->getYVel())/cnst::TILE_SIZE + 1 ) == tile || getTile( (entity->getX() + entity->getXVel())/cnst::TILE_SIZE + 0.99, (entity->getY() + entity->getYVel())/cnst::TILE_SIZE + 1 ) == tile))
        return true;
    else if (dir == 'u' && (getTile( (entity->getX() + entity->getXVel())/cnst::TILE_SIZE, (entity->getY() + entity->getYVel())/cnst::TILE_SIZE ) == tile || getTile( (entity->getX() + entity->getXVel())/cnst::TILE_SIZE + 0.99, (entity->getY() + entity->getYVel())/cnst::TILE_SIZE ) == tile))
        return true;
    else return false;
}