#pragma once

#include "componentManager.h"
#include "entity.h"
#include <SDL2/SDL_rect.h>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <vector>

class system{
public:
  system(componentManager& CM);
  std::vector<std::shared_ptr<entity>> entites;
  componentManager& CM;
  void addEntity(std::shared_ptr<entity> entity);
  void removeEntity(entityID ID);
  virtual void update() = 0;
};


class movementSystem : public system{
public:
  movementSystem(componentManager& CM);
  void update() override; 
  ~movementSystem();
};

class keyInputSystem: public system{
public:
  keyInputSystem(componentManager& CM);
  void spawnBullet();
  void update() override;
};

class renderSystem : public system{
  public:
    renderSystem(componentManager& CM);
    void update() override;
    void render();
 };

class collisionSystem : public system{

  bool detectCollision(SDL_Rect& A, SDL_Rect& B);
  bool Xcollision(SDL_Rect& A, SDL_Rect& B);
  bool sideCollision(SDL_Rect& A, SDL_Rect& B);
  bool Ycollision(SDL_Rect& A, SDL_Rect& B);
  void handleCollision();
  void handlePlayerCollision();
  void handleBulletCollision();  
  public:
    SDL_Rect collisionBB(SDL_Rect& A , SDL_Rect& B);
    bool checkHorizontalCollision(SDL_Rect& A , SDL_Rect& B);
    collisionSystem(componentManager& CM);  
    void update() override;
};

class gravitySystem : public system{
  public:
    gravitySystem(componentManager& CM);
    void update() override;
};


class animationSystem : public system{
  public:
    animationSystem(componentManager& CM);
    void update() override;
};


class aiSystem : public system{
  
public:
  aiSystem(componentManager& CM);
  void update() override;

};
