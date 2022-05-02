#pragma once

#include "../include/cnst.hpp"

//struct used for objects that are drawn to the screen with a position, texture and animation information
struct Entity    //objects and sprites on screen
{
    float x, y, velX, velY;          //position
    SDL_Texture* tex;       //texture sheet
    SDL_Rect currentFrame;  //current clipping area of texture sheet
    std::vector<std::vector<SDL_Rect>>* frameClips;  //clipping areas for animations
    int animNo, animDelay, animStart, repeatType, frameNo;

    Entity(int x, int y, SDL_Texture* tex, std::vector<std::vector<SDL_Rect>>* frameClips, int animNo)
    :x(x), y(y), velX(0), velY(0), tex(tex), frameClips(frameClips), animNo(animNo), animDelay(80), animStart(0), repeatType(cnst::ANIM_REP), frameNo(0)
    {
        if (frameClips == NULL) //if there are no clips
        {
            currentFrame.x = 0; //set default clipping area
            currentFrame.y = animNo * cnst::TILE_SIZE;
            currentFrame.w = cnst::TILE_SIZE;
            currentFrame.h = cnst::TILE_SIZE;
        }
        else
        {
            currentFrame.x = frameClips->at(animNo).at(0).x; //set default to first frame of current animation
            currentFrame.y = frameClips->at(animNo).at(0).y;
            currentFrame.w = frameClips->at(animNo).at(0).w;
            currentFrame.h = frameClips->at(animNo).at(0).h;
        }
    }

    void updateAnim()
    {
        if (frameClips == NULL || animStart == 0) //if there are no clips or if animation is off
        {
            frameNo = 0;
            currentFrame.x = 0; //set clipping area to first frame of animation
            currentFrame.y = animNo * cnst::TILE_SIZE;
            currentFrame.w = cnst::TILE_SIZE;
            currentFrame.h = cnst::TILE_SIZE;
        }
        else if (repeatType != cnst::ANIM_REP)  //else, if animation is not repeating
        {
            frameNo = ((float)SDL_GetTicks()-animStart) / animDelay;   //current frame is calculated as (current time - animation start time) / duration of frames, rounded to the integer
            int noOfFrames = frameClips->at(animNo).size();
            if (frameNo < noOfFrames) //if animation has not yet finished
            {
                currentFrame.x = frameClips->at(animNo).at(frameNo).x;  //update clipping plane to current frame
                currentFrame.y = frameClips->at(animNo).at(frameNo).y;
                currentFrame.w = frameClips->at(animNo).at(frameNo).w;
                currentFrame.h = frameClips->at(animNo).at(frameNo).h;
            }
            else if (repeatType == cnst::ANIM_DEF)   //if animation should return to default position when finished
            {
                animNo = 0; //return to default animation
                repeatType = cnst::ANIM_REP;
            }
            else frameNo = noOfFrames;
        }
        else if (repeatType == cnst::ANIM_REP)    //repeating animation
        {
            frameNo = (int)( ((float)SDL_GetTicks()-animStart) / animDelay) % frameClips->at(animNo).size();    //the current frame number is moduloed to create a repeating frame index
            currentFrame.x = frameClips->at(animNo).at(frameNo).x;
            currentFrame.y = frameClips->at(animNo).at(frameNo).y;
            currentFrame.w = frameClips->at(animNo).at(frameNo).w;
            currentFrame.h = frameClips->at(animNo).at(frameNo).h;
        }
    }

    void setAnim(int anim, int rep, int delay) //set animation number, repeat type and delay between frames
    {
        frameNo = 0;
        animStart = SDL_GetTicks();
        animNo = anim;
        repeatType = rep;
        animDelay = delay;
    }
};

//struct for player, including direction and cooldowns for certain moves/actions
struct Player : Entity
{
    bool onGround, canDoubleJump, hasKey;
    int dashTime, direction, stunTime;

    Player(int x, int y, SDL_Texture* tex, std::vector<std::vector<SDL_Rect>>* frameClips, int animNo)
    : Entity(x, y, tex, frameClips, animNo), onGround(false), canDoubleJump(true), hasKey(false), dashTime(0), direction(1), stunTime(0)
    {}
};
