#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <cmath>

#include "../include/cnst.hpp"
#include "../include/map.hpp"

class Map;

//---------------------------------------- Entity ----------------------------------------
//any object with a position and an image
class Entity
{
public:
    Entity(int x, int y, SDL_Texture *tex, int w, int h, int imageNo);
    
    float getX();
    float getY();
    void setX(float x);
    void setY(float y);
    void setPos(float x, float y);

    //render entity tex to screen
    virtual void render(int offsetX, int offsetY);

    //virtual methods used by derived classes, for updating all entities iteratively
    virtual void updateClips();
    virtual void updatePos();
    virtual void updateGravity(float gravity);
    virtual void updateCollisions(Map *map);
    virtual bool hasGravity();
    virtual bool hasCollisions();
    virtual void updateKeyDoor(Map *map);
    
protected:
    float x, y;             //position relative to world
    SDL_Texture *tex;       //pointer to texture
    SDL_Rect currentClip;   //clipping area of texture to be rendered
};

//---------------------------------------- AnimEntity ----------------------------------------
//objects with animations
class AnimEntity: public Entity
{
public:
    AnimEntity(int x, int y, SDL_Texture *tex, int w, int h, std::vector<std::vector<SDL_Point>> *clips);

    //update clipping area for animation
    void updateClips() override;

    //set and begin animation
    void setAnim(int animNo, int animType, int animDelay);

    int getAnim();

protected:
    std::vector<std::vector<SDL_Point>> *clips;     //clipping areas of aniamtion frames on texture sheet
    int animNo, frameNo, animDelay, animStart, animType;    //current frame, delay between frames in ms, time that animation started, looping behaviour
};

//---------------------------------------- ActiveEntity ----------------------------------------
//objects with velocities
class ActiveEntity: public AnimEntity
{
public:
    ActiveEntity(int x, int y, SDL_Texture *tex, int w, int h, std::vector<std::vector<SDL_Point>> *clips, bool gravity, bool collisions);
    
    void updatePos() override;

    bool hasGravity() override;
    void updateGravity(float gravity) override;

    bool hasCollisions() override;
    void updateCollisions(Map *map) override;

    float getXVel();
    float getYVel();
    void setXVel(float xVel);
    void setYVel(float yVel);
    void addXVel(float xVel);
    void addYVel(float yVel);

protected:
    float xVel, yVel, prevX, prevY;
    bool gravity, collisions;
};

//---------------------------------------- Player ----------------------------------------
//player has additional members for movement and updating camera
class Player: public ActiveEntity
{
public:
    bool onGround, canDoubleJump;

    Player(int x, int y, SDL_Texture *tex, std::vector<std::vector<SDL_Point>> *clips,
        Mix_Chunk *jumpSfx, Mix_Chunk *dashSfx, Mix_Chunk *thudSfx,
        AnimEntity *dust1, AnimEntity *dust2);

    void render(int offsetX, int offsetY) override;

    bool isStunned();
    void stun(int duration);
    bool canDash();
    void dash();
    void jump();

    bool hasKey();
    void addKey();
    void useKey();

    void drag(float drag);

    void updateCollisions(Map *map) override;

private:
    int dashTime, stunTime, keys;           //time when dash/stun cooldown will be over, and number of keys player has
    Mix_Chunk *jumpSfx, *dashSfx, *thudSfx; //player-specific sfx
    AnimEntity *dust1, *dust2;              //dust clouds left behind by player
};

//---------------------------------------- Door ----------------------------------------
class Door : public AnimEntity
{
public:
    Door(int x, int y, SDL_Texture *tex, std::vector<std::vector<SDL_Point>> *clips, Mix_Chunk *doorSfx, Player *player);

    virtual void updateKeyDoor(Map *map) override;
private:
    bool open;
    Mix_Chunk *doorSfx;
    Player *player;
};

//---------------------------------------- Key ----------------------------------------
class Key : public AnimEntity
{
public:
    Key(int x, int y, SDL_Texture *tex, std::vector<std::vector<SDL_Point>> *clips, Mix_Chunk *keySfx, Player *player);

    virtual void updateKeyDoor(Map *map) override;
    void render(int offsetX, int offsetY) override;

private:
    bool collected;
    Mix_Chunk *keySfx;
    Player *player;
};