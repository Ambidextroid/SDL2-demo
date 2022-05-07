#include "../include/entity.hpp"

extern SDL_Renderer *renderer;
extern int offsetX, offsetY, targetOffsetX, targetOffsetY;

//---------------------------------------- Entity ----------------------------------------
//any object with a position and an image
Entity::Entity(int x, int y, SDL_Texture *tex, int w, int h, int imageNo)
: x(x), y(y), tex(tex), currentClip({0, imageNo * h, w, h})
{}

float Entity::getX() { return x; }
float Entity::getY() { return y; }
void Entity::setX(float x) { this->x = x; }
void Entity::setY(float y) { this->y = y; }
void Entity::setPos(float x, float y)
{
    this->x = x;
    this->y = y;
}

//render entity tex to screen
void Entity::render(int offsetX, int offsetY)
{
    SDL_Rect dest;          //render destination
    dest.x = x + offsetX;   //entitiy world position + camera offsets
    dest.y = y + offsetY;
    dest.w = currentClip.w;
    dest.h = currentClip.h;
    SDL_RenderCopy(renderer, tex, &currentClip, &dest);
}

//virtual methods used by derived classes, for updating all entities iteratively
void Entity::updateClips() {}
void Entity::updatePos() {}
void Entity::updateGravity(float gravity) {}
void Entity::updateCollisions(Map *map) {}
bool Entity::hasGravity() { return false; }
bool Entity::hasCollisions() { return false; }
void Entity::updateKeyDoor(Map *map) {}



//---------------------------------------- AnimEntity ----------------------------------------
//objects with animations
AnimEntity::AnimEntity(int x, int y, SDL_Texture *tex, int w, int h, std::vector<std::vector<SDL_Point>> *clips)
: Entity(x, y, tex, w, h, 0), clips(clips), animNo(0), frameNo(0), animDelay(1), animStart(0), animType(cnst::ANIM_END)
{}

//update clipping area for animation
void AnimEntity::updateClips()
{
    int noOfFrames = clips->at(animNo).size();
    frameNo = ((float)SDL_GetTicks() - animStart) / animDelay;      //current frame is calculated from current time - animStart time
    
    if (animType == cnst::ANIM_DEFAULT && frameNo >= noOfFrames)                        //play once then set to default animation
    {
        animType = cnst::ANIM_REPEAT;
        animNo = 0;
    }
    if (animType == cnst::ANIM_END && frameNo >= noOfFrames) frameNo = noOfFrames - 1;  //play once and stop on last frame
    if (animType == cnst::ANIM_REPEAT) frameNo %= noOfFrames;                           //loop current animation
    //update clips
    currentClip.x = clips->at(animNo).at(frameNo).x;
    currentClip.y = clips->at(animNo).at(frameNo).y;
}

//set and begin animation
void AnimEntity::setAnim(int animNo, int animType, int animDelay)
{
    this->animNo = animNo;
    this->animType = animType;
    this->animDelay = animDelay;
    animStart = SDL_GetTicks();     //animation start time
}

int AnimEntity::getAnim() { return animNo; }


//---------------------------------------- ActiveEntity ----------------------------------------
//objects with velocities
ActiveEntity::ActiveEntity(int x, int y, SDL_Texture *tex, int w, int h, std::vector<std::vector<SDL_Point>> *clips, bool gravity, bool collisions)
: AnimEntity(x, y, tex, w, h, clips), xVel(0), yVel(0), gravity(gravity), collisions(collisions), prevX(x), prevY(y)
{}

void ActiveEntity::updatePos()
{
    if (fabs(xVel) < 0.01) xVel = 0;    //truncate if value is small
    if (fabs(yVel) < 0.01) yVel = 0;
    x += xVel;
    y += yVel;
}

bool ActiveEntity::hasGravity() { return gravity; }

void ActiveEntity::updateGravity(float gravity)
{
    yVel += gravity;
    if (yVel >= cnst::TERMINAL_VEL) yVel = cnst::TERMINAL_VEL;
}

bool ActiveEntity::hasCollisions() { return collisions; }

void ActiveEntity::updateCollisions(Map *map)
{
    if (xVel >= 0)      //if moving right
    {
        if (map->checkCollision('r', this, 1))  //if entity will collide with wall to the right
        {
            xVel = 0;   //stop
            if ((int)prevX % 50 > 0) x = (int)prevX - (int)prevX % 50 + 50; //align entity with wall
            else x = (int)prevX;
        }
    }
    else                //left
    {
        if (map->checkCollision('l', this, 1))
        {
            xVel = 0;
            if ((int)prevX % 50 > 0) x = (int)prevX - (int)prevX % 50;
            else x = (int)prevX;
        }
    }
    if (yVel >= 0)      //up
    {
        if (map->checkCollision('d', this, 1))
        {
            yVel = 0;
            if ((int)prevY % 50 > 0) y = (int)prevY - (int)prevY % 50 + 50;
            else y = (int)prevY;
        }
    }
    else                //down
    {
        if (map->checkCollision('u', this, 1))
        {
            yVel = 0;
            if ((int)prevY % 50 > 0) y = (int)prevY - (int)prevY % 50;
            else y = (int)prevY;
        }
    }
    prevX = x;
    prevY = y;
}

float ActiveEntity::getXVel() { return xVel; }
float ActiveEntity::getYVel() { return yVel; }
void ActiveEntity::setXVel(float xVel) { this->xVel = xVel; }
void ActiveEntity::setYVel(float yVel) { this->yVel = yVel; }
void ActiveEntity::addXVel(float xVel) { this->xVel += xVel; }
void ActiveEntity::addYVel(float yVel) { this->yVel += yVel; }


//---------------------------------------- Player ----------------------------------------
//player has additional members for movement and updating camera
Player::Player(int x, int y, SDL_Texture *tex, std::vector<std::vector<SDL_Point>> *clips,
Mix_Chunk *jumpSfx, Mix_Chunk *dashSfx, Mix_Chunk *thudSfx,
AnimEntity *dust1, AnimEntity *dust2)
: ActiveEntity(x, y, tex, 70, 70, clips, true, true), onGround(true), canDoubleJump(false), dashTime(0), stunTime(0), keys(0),
  jumpSfx(jumpSfx), dashSfx(dashSfx), thudSfx(thudSfx), dust1(dust1), dust2(dust2)
{}

void Player::render(int offsetX, int offsetY)
{
    SDL_Rect dest;
    dest.x = x + offsetX - 10;      //player sprite has extra offset
    dest.y = y + offsetY - 10;
    dest.w = currentClip.w;
    dest.h = currentClip.h;
    SDL_RenderCopy(renderer, tex, &currentClip, &dest);
}

bool Player::isStunned()
{
    return (SDL_GetTicks() < stunTime) ? true : false;      //if current time is not yet stunTime, player is stunned
}
void Player::stun(int duration)
{
    stunTime = SDL_GetTicks() + duration;                   //set stun time to current time + specified stun duration
}

bool Player::canDash()
{
    if (abs(xVel == 0)) return false;                       //player cannot dash if not moving
    return (SDL_GetTicks() < dashTime) ? false : true;      //if current time is not yet dashTime, player cannot dash
}
void Player::dash()
{
    yVel = 0;                           //stop player falling
    if (xVel > 0) xVel = 22;            //if payer is moving right, dash right
    else xVel = -22;
    dashTime = SDL_GetTicks() + 800;    //dash has an 800ms cooldown
    dust1->setPos(x - 20, y - 30);
    dust1->setAnim(4, cnst::ANIM_END, 30);
    dust2->setPos(x - 20, y + 30);
    dust2->setAnim(5, cnst::ANIM_END, 30);
    Mix_PlayChannel(-1, dashSfx, 0);    //dash sound
}
void Player::jump()
{
    yVel = -12;
    if (!onGround)      //if double jump
    {
        canDoubleJump = false;
        dust1->setPos(x - 30, y + 20);                //dust cloud
        dust1->setAnim(2, cnst::ANIM_END, 30);
        dust2->setPos(x + 30, y + 20);
        dust2->setAnim(3, cnst::ANIM_END, 30);
        Mix_PlayChannel(-1, jumpSfx, 0);    //double jump sound
    }
    onGround = false;
}

bool Player::hasKey() { return (keys > 0); }
void Player::addKey() { keys++; }
void Player::useKey() { if (keys > 0) keys--; }

void Player::drag(float drag) { xVel *= drag; }

//player collision
void Player::updateCollisions(Map *map)
{
    onGround = false;   //start by assuming player is not on ground, then update to true when player collides with ground
    if (xVel >= 0)      //if moving right
    {
        if (map->checkCollision('r', this, 1))  //if entity will collide with wall to the right
        {
            xVel = 0;   //stop
            if ((int)prevX % 50 > 0) x = (int)prevX - (int)prevX % 50 + 50; //align entity with wall
            else x = (int)prevX;
        }
    }
    else                //left
    {
        if (map->checkCollision('l', this, 1) || x + xVel < 0)
        {
            xVel = 0;
            if ((int)prevX % 50 > 0) x = (int)prevX - (int)prevX % 50;
            else x = (int)prevX;
        }
    }
    if (yVel >= 0)      //down
    {
        if (map->checkCollision('d', this, 1))
        {
            float oldYVel = yVel;
            yVel = 0;
            if ((int)prevY % 50 > 0) y = (int)prevY - (int)prevY % 50 + 50;
            else y = (int)prevY;

            if (oldYVel >= cnst::TERMINAL_VEL)                  //if player has hit the ground at max speed
            {
                xVel = 0;                                       //stop sideways motion
                setAnim(3, cnst::ANIM_DEFAULT, 100);            //play wince animation
                stun(400);                                      //stun player for a short period
                map->shakeCamera(12);
                dust1->setPos(x - 60, y);                       //dust cloud
                dust1->setAnim(0, cnst::ANIM_END, 100);
                dust2->setPos(x + 60, y);
                dust2->setAnim(1, cnst::ANIM_END, 100);
                Mix_PlayChannel(-1, thudSfx, 0);                //thud sound
            }
            onGround = true;
            canDoubleJump = true;
        }
    }
    else                //up
    {
        if (map->checkCollision('u', this, 1))
        {
            yVel = 0;
            if ((int)prevY % 50 > 0) y = (int)prevY - (int)prevY % 50;
            else y = (int)prevY;
        }
    }
    prevX = x;
    prevY = y;
}


//---------------------------------------- Door ----------------------------------------
Door::Door(int x, int y, SDL_Texture *tex, std::vector<std::vector<SDL_Point>> *clips, Mix_Chunk *doorSfx, Player *player)
: AnimEntity(x, y, tex, 50, 200, clips), open(false), doorSfx(doorSfx), player(player)
{}

void Door::updateKeyDoor(Map *map)
{
    //if player touches closed door while in possesion of a key
    if (!open && player->hasKey()
    && player->getX() + 50 >= x && player->getX() - currentClip.w <= x
    && player->getY() + 50 >  y && player->getY() - currentClip.h <  y)
    {
        open = true;
        player->useKey();
        setAnim(1, cnst::ANIM_END, 50);                             //open door animation
        player->stun(500);
        map->setTile(x/cnst::TILE_SIZE, y/cnst::TILE_SIZE, 2);      //set tiles under door to blackground
        map->setTile(x/cnst::TILE_SIZE, y/cnst::TILE_SIZE + 1, 2);
        map->setTile(x/cnst::TILE_SIZE, y/cnst::TILE_SIZE + 2, 2);
        map->setTile(x/cnst::TILE_SIZE, y/cnst::TILE_SIZE + 3, 2);
        Mix_PlayChannel(-1, doorSfx, 0);                            //door opening sound
    }
}

//---------------------------------------- Key ----------------------------------------
Key::Key(int x, int y, SDL_Texture *tex, std::vector<std::vector<SDL_Point>> *clips, Mix_Chunk *keySfx, Player *player)
: AnimEntity(x, y, tex, 50, 50, clips), collected(0), keySfx(keySfx), player(player)
{}

void Key::updateKeyDoor(Map *map)
{
    //if player touches key
    if (!collected
    && player->getX() + 50 > x && player->getX() - currentClip.w < x
    && player->getY() + 50 > y && player->getY() - currentClip.h < y)
    {
        collected = true;
        player->addKey();
        setAnim(1, cnst::ANIM_END, 1);      //blank frame
        Mix_PlayChannel(-1, keySfx, 0);     //key collect sound
    }
}

void Key::render(int offsetX, int offsetY)
{
    SDL_Rect dest;
    dest.x = x + offsetX;
    dest.y = y + offsetY + 5*sin(SDL_GetTicks()/200.0f);    //key hovers up and down using sin(time)
    dest.w = currentClip.w;
    dest.h = currentClip.h;
    SDL_RenderCopy(renderer, tex, &currentClip, &dest);
}