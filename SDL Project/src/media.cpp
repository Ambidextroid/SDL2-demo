#include "../include/media.hpp"

//struct for loading and storing graphics/sounds and generating animation clips
Media::Media(SDL_Renderer *renderer)
: renderer(renderer)
{
    //textures
    playerTex = loadTexture("res/gfx/player.png");
    dustTex = loadTexture("res/gfx/dust.png");
    doorTex = loadTexture("res/gfx/door.png");
    keyTex = loadTexture("res/gfx/key.png");

    starTex = loadTexture("res/gfx/starfield.png");
    brickFloorTex = loadTexture("res/gfx/floor.png");
    brickBackgroundTex = loadTexture("res/gfx/background.png");
    windowTex = loadTexture("res/gfx/window.png");
    tutorialTex = loadTexture("res/gfx/tutorial.png");

    //animation clips
    playerClips = {
        makeClips(0, 4, 70, 70),
        makeClips(1, 4, 70, 70),
        makeClips(2, 4, 70, 70),
        makeClips(3, 4, 70, 70)};
    dustClips = {
        makeClips(0, 6, 50, 50),
        makeClips(1, 6, 50, 50),
        makeClips(2, 5, 50, 50),
        makeClips(3, 5, 50, 50),
        makeClips(4, 5, 50, 50),
        makeClips(5, 5, 50, 50)};
    doorClips = {
        makeClips(0, 1, 50, 200),
        makeClips(1, 15, 50, 200)};
    keyClips = {
        makeClips(0, 1, 50, 50),
        makeClips(1, 1, 50, 50)};

    //sound
    bgMusic = loadMusic("res/wav/music.wav");

    jumpSfx = loadSfx("res/wav/jump.wav");
    thudSfx = loadSfx("res/wav/thud.wav");
    dashSfx = loadSfx("res/wav/dash.wav");
    keySfx = loadSfx("res/wav/key.wav");
    doorSfx = loadSfx("res/wav/door.wav");
}

    //load png, return texture pointer
SDL_Texture *Media::loadTexture(const char *filePath)
{
    SDL_Texture *texture = NULL;
    texture = IMG_LoadTexture(renderer, filePath);  //load texture from path
    if(texture==NULL) std::cout << "Texture failed: " << SDL_GetError() << std::endl;   //catch error
    return texture;
}

Mix_Music *Media::loadMusic(const char *filePath)
{
    Mix_Music *music = NULL;
    music = Mix_LoadMUS(filePath);  //load texture from path
    if(music==NULL) std::cout << "Music failed: " << Mix_GetError() << std::endl;   //catch error
    return music;
}

Mix_Chunk *Media::loadSfx(const char *filePath)
{
    Mix_Chunk *sfx = NULL;
    sfx = Mix_LoadWAV(filePath);  //load texture from path
    if(sfx==NULL) std::cout << "Sfx failed: " << Mix_GetError() << std::endl;   //catch error
    return sfx;
}


//make a vector of points representing the top left corner of each animation frame
std::vector<SDL_Point> Media::makeClips(int animNo, int frames, int w, int h)
{
    std::vector<SDL_Point> clips;
    int y = animNo * h;             //all y values will be set to row number * frame height
    for (int i = 0; i < frames; i++)
    {
        int x = i * w;              //each x value is a multiple of frame width
        SDL_Point p{x, y};
        clips.push_back(p);
    }
    return clips;
}