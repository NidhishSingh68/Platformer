#pragma once
#include "animationContainer.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <bitset>
#include <cstddef>
#include <iterator>
#include <memory>
#include <typeinfo>
#include <unordered_map>
#include <vector>
using namespace std;


enum type{
    player = 0,
    tile = 1,
    bullet = 2,
    box = 3,
    enemy = 4
};

class component{
  public:
};



class velocityComponent : public component{
  bool Moving;
  bool jumping;
  public:
  bool IDLE;
  velocityComponent();
  velocityComponent(float vx, float vy);
  void setVelocity(float vx , float vy);
  bool isMoving();
  bool isJumping();
  void setIsJumping(bool status);
  void setIsMoving(bool status);
  float vx,vy = 1;
};
class positionComponent : public component{
  std::shared_ptr<velocityComponent> velocity;
  public:
    positionComponent();   
    positionComponent(float x , float y);
    float x,y = 0;
    float getX();
    float getY();
    void setPosition(float x , float y);
};

class spriteComponent : public component{

    SDL_Point getsize(SDL_Texture *texture) {
      SDL_Point size;
      SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
      return size;
    }

    public:
    bool invert;
    SDL_Texture* texture;
    SDL_Rect srcRect, destRect;
    std::shared_ptr<positionComponent> position;
    std::shared_ptr<velocityComponent> velocity;
    spriteComponent(SDL_Texture* texture);
    spriteComponent(SDL_Texture* texture , SDL_Rect& srcRect,SDL_Rect& rect);
    spriteComponent(SDL_Texture* texture, std::shared_ptr<positionComponent> position , std::shared_ptr<velocityComponent> velocity);
    spriteComponent(SDL_Texture* texture , int sizeX , int sizeY);
    //spriteComponent(spriteComponent& component);
    void init();
};


class keyBoardInput : public component{

public:
  keyBoardInput(std::shared_ptr<velocityComponent> velocity);
  std::shared_ptr<velocityComponent> velocity;
};



class collision : public component{
  SDL_Rect boundingBox;
  type entityType;
public:
  
  collision(SDL_Rect& boundingBox , type Etype);
  type getEntityType();
  SDL_Rect& getBoundingBox();
  void updatePosition(float x , float y);
};


class gravity : public component{
  float accelerationDueToGravity;
public:
  gravity();
  float downwardsAcceleration();
};


class animation : public component{
  
  std::unordered_map<std::string, std::shared_ptr<animationContainer>> animationStateArray; 
  public:
  std::size_t frameCounter;
  Uint32 lastUpdatedTime;
  Uint32 frameDelay;
  std::string currentAnimation;
  animation(Uint32 delay);
  void insertAnimation(std::string name,std::vector<SDL_Rect>& frames);
  void setAnimation(std::string currentAnimation);
  std::string getCurrentAnimation();
  SDL_Rect& getRectNext();
};

class healthComponent : public component{

  public:
  healthComponent();
  float health; 
};



class enemyAI : public component{

  public:
  enemyAI();
  float thresholdDistance;
  bool isAttacking;


};




