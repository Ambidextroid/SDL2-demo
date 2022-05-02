#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <cmath>

#include "../include/cnst.hpp"
#include "../include/entity.hpp"
#include "../include/renderwindow.hpp"
#include "../include/map.hpp"

int main(int argc, char* args[])
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) > 0 ) std::cout << "SDL failed: " << SDL_GetError() << std::endl;   //init SDL, catch error
    if(!IMG_Init(IMG_INIT_PNG)) std::cout << "IMG failed: " << IMG_GetError() << std::endl;         //init IMG, catch error
    if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0 ) std::cout << "Sound failed: " << Mix_GetError() << std::endl; //init sound, catch error
    RenderWindow window("Game", cnst::WIN_W, cnst::WIN_H);  //program window
    const float refreshRate = window.getRefreshRate();  //monitor Hz
    
    //----- load media and entities -----

    //player
    std::vector<std::vector<SDL_Rect>> playerClips {window.makeClips(0, 4, 70, 70), window.makeClips(1, 4, 70, 70), window.makeClips(2, 4, 70, 70), window.makeClips(3, 4, 70, 70)}; //player animation fame clips
    Player player(750, 350, window.loadTexture("res/gfx/player.png"), &playerClips, 0);   //player entity
    player.setAnim(0, cnst::ANIM_REP, 80); //set player animation

    //dust effects
    SDL_Texture* dustTex = window.loadTexture("res/gfx/dust.png");
    std::vector<std::vector<SDL_Rect>> dustClips {window.makeClips(0,5), window.makeClips(1,5), window.makeClips(2,4), window.makeClips(3,4), window.makeClips(4,4), window.makeClips(5,4)};
    Entity dustLeft(-100, -100, dustTex, &dustClips, 1);
    Entity dustRight(-100, -100, dustTex, &dustClips, 0);
    dustLeft.setAnim(1, cnst::ANIM_REP, 100);
    dustRight.setAnim(0, cnst::ANIM_REP, 100);

    //stars
    SDL_Texture* starTex = window.loadTexture("res/gfx/starfield.png"); //stars
    std::vector<std::vector<SDL_Rect>> starClips {window.makeClips(0, 1, 1000, 800)};
    Entity starTL(0, 0, starTex, &starClips, 0);
    Entity starTR(0, 0, starTex, &starClips, 0);
    Entity starBL(0, 0, starTex, &starClips, 0);
    Entity starBR(0, 0, starTex, &starClips, 0);

    //map entities
    Map map = Map();    //world map
    map.floorTex = window.loadTexture("res/gfx/floor.png"); //map textures
    map.bgTex = window.loadTexture("res/gfx/background.png");
    map.windowTex = window.loadTexture("res/gfx/window.png");
    Entity key(-50, -50, window.loadTexture("res/gfx/key.png"), NULL, 0);   //key
    map.key = &key;
    //door opening animation frames
    std::vector<std::vector<SDL_Rect>> doorClips {window.makeClips(0, 1, 50, 250), window.makeClips(1, 15, 50, 250)};
    Entity door(0, 0, window.loadTexture("res/gfx/door.png"), &doorClips, 0);
    door.setAnim(0, cnst::ANIM_END, 1);
    map.door = &door;

    //tutorial sign
    std::vector<std::vector<SDL_Rect>> tutClips {window.makeClips(0, 1, 450, 250)};
    Entity tutorial(50, 50, window.loadTexture("res/gfx/tutorial.png"), &tutClips, 0);
    
    //music
    Mix_Music *bgMusic = Mix_LoadMUS("res/wav/music.wav");
    //sfx
    Mix_Chunk *jumpSound = Mix_LoadWAV("res/wav/jump.wav");
    Mix_Chunk *thudSound = Mix_LoadWAV("res/wav/thud.wav");
    Mix_Chunk *dashSound = Mix_LoadWAV("res/wav/dash.wav");
    Mix_Chunk *keySound = Mix_LoadWAV("res/wav/key.wav");
    Mix_Chunk *doorSound = Mix_LoadWAV("res/wav/door.wav");
    

    SDL_Event event;    //event handler

    int accumulator = 0;
    int prevStartTicks = SDL_GetTicks();

    if(Mix_PlayingMusic() == 0) Mix_PlayMusic(bgMusic, -1); //play music

    bool gameRunning = true;
    while(gameRunning)
    {
        int startTicks = SDL_GetTicks();    //ticks (since init) at the start of the game loop
        int prevFrameTicks = startTicks - prevStartTicks;   //number of ticks since start of last frame (i.e. duration of last frame)
        prevStartTicks = startTicks;
        accumulator += prevFrameTicks;  //add the previous frame time to an accumulator
        while(accumulator >= cnst::TIME_STEP)  //only progress simulation when enough time is accumulated (this decouples frame rate and game speed)
        {
            float prevX = player.x; //player's x/y position from previous frame
            float prevY = player.y;
            
            const Uint8* keyState = SDL_GetKeyboardState(NULL); //current state of keyboard
            while(SDL_PollEvent(&event))    //process events on queue
            {
                if (event.type == SDL_QUIT) gameRunning = false;
                if (event.type == SDL_KEYDOWN && event.key.repeat == 0)  //if a key is pressed (register once)
                {
                    switch(event.key.keysym.sym)
                    {
                    case SDLK_SPACE:    //press space to double jump mid-air
                        if (!player.onGround && player.canDoubleJump && player.velY > -10)
                        {
                            player.velY = -12;  //set upwards velocity
                            player.canDoubleJump = false;
                            dustLeft.x = player.x + 30;    //double jump dust cloud visual effect
                            dustLeft.y = player.y + 20;
                            dustLeft.setAnim(3, cnst::ANIM_END, 30);
                            dustRight.x = player.x - 30;
                            dustRight.y = player.y + 20;
                            dustRight.setAnim(2, cnst::ANIM_END, 30);
                            Mix_PlayChannel( -1, jumpSound, 0 );    //play jump sound effect
                        }
                        
                        break;
                    case SDLK_LSHIFT:   //press shift to dash sideways
                        //only dash if dash cooldown is over, and either a direction is being held or the player is mid-air
                        if (player.dashTime < startTicks && player.stunTime < startTicks && (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT] || !player.onGround)) 
                        {
                            player.dashTime = startTicks + 800; //dash has an 800ms cooldown
                            if (player.direction == 1)  //right
                            {
                                player.velY = 0;    //stop vertical movement
                                player.velX = 22;   //dash right
                                dustLeft.x = player.x - 20;    //dash visual effect
                                dustLeft.y = player.y + 30;
                                dustLeft.setAnim(5, cnst::ANIM_END, 30);
                                dustRight.x = player.x - 20;
                                dustRight.y = player.y - 30;
                                dustRight.setAnim(4, cnst::ANIM_END, 30);
                                Mix_PlayChannel( -1, dashSound, 0 );    //play dash sound effect
                            }
                            if (player.direction == -1) //left
                            {
                                player.velY = 0;
                                player.velX = -22;
                                dustLeft.x = player.x + 20;
                                dustLeft.y = player.y + 30;
                                dustLeft.setAnim(5, cnst::ANIM_END, 30);
                                dustRight.x = player.x + 20;
                                dustRight.y = player.y - 30;
                                dustRight.setAnim(4, cnst::ANIM_END, 30);
                                Mix_PlayChannel( -1, dashSound, 0 );    //play dash sound effect
                            }
                        }
                        break;
                    }
                }
            }

            //if no drection is being held, return to idle animation
            if (!keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT] && player.animNo != 0 && player.animNo != 3) player.setAnim(0, cnst::ANIM_REP, 80);

            //if a key is held (register continuously)
            if (player.stunTime < startTicks)   //if player is not stunned
            {
                if(keyState[SDL_SCANCODE_LEFT]) //if left is being held
                {
                    if (player.animNo != 2) player.setAnim(2, cnst::ANIM_REP, 80); //moving left animation
                    player.direction = -1;  //facing left
                    player.velX -= 0.6; //accelerate leftward
                }
                if(keyState[SDL_SCANCODE_RIGHT]) //if right is being held
                {
                    if (player.animNo != 1) player.setAnim(1, cnst::ANIM_REP, 80);
                    player.direction = 1;
                    player.velX += 0.6;
                }
                if(keyState[SDL_SCANCODE_SPACE]) //if space is being held
                {
                    if (player.onGround)
                    {
                        player.velY = -12;  //add 12 upward velocity to player to jump
                        player.onGround = false;
                    } 
                }
            }

            //extra drag if no input and player is on ground
            if(!keyState[SDL_SCANCODE_LEFT] && !keyState[SDL_SCANCODE_RIGHT] && player.onGround) player.velX *= 0.7;
            else player.velX *= 0.9;    //normal drag

            player.velY += 0.5; //gravity, accelerate down
            const float terminalVel = 23;   //max downward velocity


            //----------- collisions -----------

            if (player.velX >= 0)   //if player is moving right
            {
                if (map.checkCollision('r', &player, 3) || map.checkCollision('r', &player, 4) || map.checkCollision('r', &player, 6))    //returns true if player is colliding with tile 3 or 4 to the right
                //if player would move into wall, instead:
                {
                    player.velX = 0;    //stop moving
                    if ((int)prevX % 50 > 0) player.x = (int)prevX - (int)prevX % 50 + 50;  //add remainding distance, to position player against wall
                    else player.x = (int)prevX; //if there is no remaining distance, do nothing
                }
            }
            else    //if player is moving left
            {
                if (map.checkCollision('l', &player, 3) || map.checkCollision('l', &player, 4) || map.checkCollision('l', &player, 6) || player.x+player.velX < 0)
                {
                    player.velX = 0;
                    if ((int)prevX % 50 > 0) player.x = (int)prevX - (int)prevX % 50;
                    else player.x = (int)prevX;
                }
            }
            if (player.velY >= 0)   //if player is falling
            {
                if (map.checkCollision('d', &player, 3) || map.checkCollision('d', &player, 4))
                {
                    if ((int)prevY % 50 > 0) player.y = (int)prevY - (int)prevY % 50 + 50;
                    else player.y = (int)prevY;

                    if (player.velY >= terminalVel) //if player has hit the ground at max speed
                    {
                        player.velX = 0;    //stop sideways motion
                        player.setAnim(3, cnst::ANIM_DEF, 100);  //wince animation
                        player.stunTime = startTicks + 400;     //stun player for a short period (cannot move while stunned)
                        dustLeft.x = player.x - cnst::TILE_SIZE - 10;    //dust effect, create dust clouds next to player
                        dustLeft.y = player.y;
                        dustLeft.setAnim(0, cnst::ANIM_END, 100);
                        dustRight.x = player.x + cnst::TILE_SIZE + 10;
                        dustRight.y = player.y;
                        dustRight.setAnim(1, cnst::ANIM_END, 100);
                        window.setShake(12, 10, 0.95);  //shake screen
                        Mix_PlayChannel( -1, thudSound, 0 );    //play thud sound effect
                    }
                    player.onGround = true;
                    player.canDoubleJump = true;    //replenish double jump
                    player.velY = 0;
                }
                else player.onGround = false;
            }
            else    //if player is jumping
            {
                if (map.checkCollision('u', &player, 3) || map.checkCollision('u', &player, 4) || map.checkCollision('u', &player, 6))
                {
                    player.velY = 0;
                    if ((int)prevY % 50 > 0) player.y = (int)prevY - (int)prevY % 50;
                    else player.y = (int)prevY;
                }
            }
            //if player collides with key tile
            if(!player.hasKey && (map.checkCollision('u', &player, 5) || map.checkCollision('d', &player, 5) || map.checkCollision('l', &player, 5) || map.checkCollision('r', &player, 5)))
            {
                player.hasKey = true;   //give player key
                key.animNo = 1; //made key disappear
                key.updateAnim();
                Mix_PlayChannel( -1, keySound, 0 );    //play key sound effect
            }
            //if player collides with door and has key
            if(player.hasKey && (map.checkCollision('u', &player, 4) || map.checkCollision('d', &player, 4) || map.checkCollision('l', &player, 4) || map.checkCollision('r', &player, 4)))
            {
                map.map.at(4).at(17) = 1;   //set wall tiles to background tiles
                map.map.at(5).at(17) = 1;
                map.map.at(6).at(17) = 1;
                map.map.at(7).at(17) = 1;
                door.setAnim(1, cnst::ANIM_END, 50);
                player.stunTime = SDL_GetTicks() + 500;
                Mix_PlayChannel( -1, doorSound, 0 );    //play door sound effect
            }
            

            if (abs(player.velX) < 0.01) player.velX = 0;   //truncate small velocities
            if (player.velY > terminalVel) player.velY = terminalVel;         //set cap on downward velocity to terminalVel

            
            player.x += player.velX;    //apply final velocities to player's position
            player.y += player.velY;

            player.updateAnim();    //update animations
            dustLeft.updateAnim();
            dustRight.updateAnim();
            door.updateAnim();

            map.updateCamZone(&player); //update current camera zone
            window.calcOffsets(&player, map.currentRoom);   //calculate camera offsets
            
            accumulator -= cnst::TIME_STEP;    //remove simulated time from accumulator
        }

        window.clear(0,0,50); //clear screen with dark blue

        int starSpeedFactor = 5;    //determines rate that stars move (for parallax effect)
        //4 tiled starry sky textures are used and their positions are moduloed to wrap around
        window.render(starTL, -window.offsetX/starSpeedFactor % 1000 + 1000, -window.offsetY/starSpeedFactor % 800 + 800);
        window.render(starTR, -window.offsetX/starSpeedFactor % 1000, -window.offsetY/starSpeedFactor % 800 + 800);
        window.render(starBL, -window.offsetX/starSpeedFactor % 1000 + 1000, -window.offsetY/starSpeedFactor % 800);
        window.render(starBR, -window.offsetX/starSpeedFactor % 1000, -window.offsetY/starSpeedFactor % 800);

        map.drawMap(&window, window.offsetX, window.offsetY);  //draw map
        //draw other objects
        window.render(key, -window.offsetX, -window.offsetY);
        window.render(door, -window.offsetX, -window.offsetY);
        window.render(tutorial, -window.offsetX, -window.offsetY);

        window.render(player, -window.offsetX-10, -window.offsetY-10);  //draw player

        if (dustLeft.frameNo < dustClips.at(dustLeft.animNo).size())    //if dust animation is playing
        {
            window.render(dustLeft, -window.offsetX, -window.offsetY);  //draw dust clouds
            window.render(dustRight, -window.offsetX, -window.offsetY);
        }

        SDL_RenderPresent(window.renderer);   //present rendered image to screen

        int frameTicks = SDL_GetTicks() - startTicks;   //ticks since start of frame
        if(frameTicks < 1000/refreshRate)   //if frame was too quick
        {
            SDL_Delay(1000/refreshRate - frameTicks);   //delay to match refresh rate
        }
    }

    SDL_DestroyWindow(window.window);   //destroy window
    SDL_Quit();         //close sdl
    
    return 0;
}