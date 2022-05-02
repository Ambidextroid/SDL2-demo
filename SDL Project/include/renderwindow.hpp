#pragma once

#include "../include/cnst.hpp"
#include "../include/entity.hpp"

struct RenderWindow  //program window
{
    SDL_Window* window; //program window
    SDL_Renderer* renderer; //window renderer
    int offsetX, offsetY, destOffsetX, destOffsetY; //rendering offsets for camera position
    float shakeDecay, decayRate, shakeSpeed;    //shaking effect

    RenderWindow(const char* title, int w, int h)
    :window(NULL), renderer(NULL), offsetX(0), offsetY(0), destOffsetX(0), destOffsetY(0), shakeDecay(0), decayRate(0), shakeSpeed(0)
    {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, SDL_WINDOW_SHOWN);   //create window
        if (window==NULL) std::cout << "Window failed: " << SDL_GetError() << std::endl;    //catch error

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);    //GPU accelerated renderer
    }

    SDL_Texture* loadTexture(const char* filePath)  //load tex from PNG file path
    {
        SDL_Texture* texture = NULL;
        texture = IMG_LoadTexture(renderer, filePath);  //load texture from path
        if(texture==NULL) std::cout << "Texture failed: " << SDL_GetError() << std::endl;   //catch error
        return texture;
    }

    std::vector<SDL_Rect> makeClips(int row, int frames)  //make a vector of SDL_Rects representing animation frames for entities
    {
        std::vector<SDL_Rect> clips;
        int y = row * cnst::TILE_SIZE;    //all y values will be set to the row number * TILE_SIZE
        for (int i = 0; i < frames; i++)
        {
            int x = i * cnst::TILE_SIZE;    //each x value is a multiple of TILE_SIZE
            SDL_Rect r{x, y, cnst::TILE_SIZE, cnst::TILE_SIZE};
            clips.push_back(r);
        }
        return clips;
    }

    std::vector<SDL_Rect> makeClips(int row, int frames, int w, int h)  //overloaded method with custom frame width/height
    {
        std::vector<SDL_Rect> clips;
        int y = row * h;    //all y values will be set to the row number * frame height
        for (int i = 0; i < frames; i++)
        {
            int x = i * w;    //each x value is a multiple of the frame width
            SDL_Rect r{x, y, w, h};
            clips.push_back(r);
        }
        return clips;
    } 

    void calcOffsets(Entity* player, SDL_Rect zone) //given the player's position and the current camera zone, calculate the camera offsets
    {
        int visTilesX = cnst::WIN_W/cnst::TILE_SIZE;  //number of tiles that can fit on the screen
        int visTilesY = cnst::WIN_H/cnst::TILE_SIZE;
        destOffsetX = player->x - visTilesX/2 * cnst::TILE_SIZE + cnst::TILE_SIZE/2;    //offset sprites to keep character in centre of screen
        destOffsetY = player->y - visTilesY/2 * cnst::TILE_SIZE + cnst::TILE_SIZE/2;    //(these are coordinates of the camera relative to the map)
        
        if (destOffsetX < zone.x) destOffsetX = zone.x;   //clamp offset at borders so camera stops at the side of a room
        if (destOffsetY < zone.y) destOffsetY = zone.y;
        if (destOffsetX > zone.x + zone.w - visTilesX*cnst::TILE_SIZE) destOffsetX = zone.x + zone.w - visTilesX*cnst::TILE_SIZE;
        if (destOffsetY > zone.y + zone.h - visTilesY*cnst::TILE_SIZE) destOffsetY = zone.y + zone.h - visTilesY*cnst::TILE_SIZE;

        //move the camera towards the target position by a certain fraction of the distance each frame to produce a smooth camera effect:
        //camera position += distance between camera and target * fraction
        float smoothMultiplier = 0.08;  //fraction used to determine the speed of the smooth camera movement
        if (destOffsetX-offsetX > 1) offsetX += 1 + (destOffsetX - offsetX) * smoothMultiplier;
        else if (destOffsetX-offsetX < -1) offsetX += -1 + (destOffsetX - offsetX) * smoothMultiplier;
        else offsetX = destOffsetX; //truncate small distances
        if (destOffsetY-offsetY > 1) offsetY += 1 + (destOffsetY - offsetY) * smoothMultiplier;
        else if (destOffsetY-offsetY < -1) offsetY += -1 + (destOffsetY - offsetY) * smoothMultiplier;
        else offsetY = destOffsetY;

        if (shakeDecay != 0)   //camera shake
        {
            shakeDecay *= decayRate;
            offsetX += sin(SDL_GetTicks()/shakeSpeed) * shakeDecay; //shake the camera using a sine with a constantly decreasing amplitude
            if (abs(shakeDecay) < 0.001) shakeDecay = 0;
        }
    }

    void setShake(float amplitude, float freq, float decay) //set camera shake values
    {
        shakeDecay = amplitude;
        shakeSpeed = freq;
        decayRate = decay;
    }

    void clear(int r, int g, int b) //clear screen with colour
    {
        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderClear(renderer);
    }

    void render(Entity& entity, int offsetX, int offsetY)   //render an entity to the screen using its position plus an offset
    {
        //set src rect to clipping area size and location
        SDL_Rect src;
        src.x = entity.currentFrame.x;
        src.y = entity.currentFrame.y;
        src.w = entity.currentFrame.w;
        src.h = entity.currentFrame.h;
        //set dst rect to clipping area size and entity's x/y location
        SDL_Rect dst;
        dst.x = entity.x + offsetX;
        dst.y = entity.y + offsetY;
        dst.w = entity.currentFrame.w;
        dst.h = entity.currentFrame.h;
        SDL_RenderCopy(renderer, entity.tex, &src, &dst);
    }

    int getRefreshRate()    //return refresh rate of monitor in Hz
    {
        int displayIndex = SDL_GetWindowDisplayIndex(window);
        SDL_DisplayMode mode;
        SDL_GetDisplayMode(displayIndex, 0, &mode);

        return mode.refresh_rate;
    }
};