
#include "component.h"
#include "tex_manager.h"
#include <SDL2/SDL_render.h>
#include <cstddef>
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>

positionComponent::positionComponent(){

  this->x = this->y = 0;
}

positionComponent::positionComponent(float x ,float y){
  this->x = x;
  this->y = y;
}

void positionComponent::setPosition(float x , float y){
  this->x = x;
  this->y = y;
}

float positionComponent::getX(){
  return this->x;
}

float positionComponent::getY(){
  return this->y;
}

void spriteComponent::init( ){
  this->srcRect.x = this->srcRect.y = 0;
  this->srcRect.h = this->srcRect.w = 1024;
  this->destRect.h = this->destRect.w = 64;
  this->destRect.x = this->destRect.y = 0;
}
spriteComponent::spriteComponent(SDL_Texture* texture){
  this->texture = texture;
  SDL_Point size = this->getsize(this->texture);
  this->srcRect.x = 0;
  this->srcRect.y = 0;
  this->srcRect.h = size.y;
  this->destRect.h = 64;
  this->destRect.w = 64;
  this->invert = false;
}
spriteComponent::spriteComponent(SDL_Texture* texture , int sizeX , int sizeY){
  this->texture = texture;
  SDL_Point size = this->getsize(this->texture);
  this->srcRect.x = 0;
  this->srcRect.y = 0;
  this->srcRect.h = size.y;
  this->destRect.h = sizeX;
  this->destRect.w = sizeY;
  this->invert = false;
   //= size.x
}
spriteComponent::spriteComponent(SDL_Texture* texture , SDL_Rect& srcRect,SDL_Rect& rect){
  this->texture = texture;
  this->srcRect.x = srcRect.x;
  this->srcRect.y = srcRect.y;
  this->srcRect.w = srcRect.w;
  this->srcRect.h = srcRect.h;
  this->destRect.x = rect.x;
  this->destRect.y = rect.y;
  this->destRect.w = rect.w;
  this->destRect.h = rect.h;
}

spriteComponent::spriteComponent(SDL_Texture* texture, std::shared_ptr<positionComponent> position , std::shared_ptr<velocityComponent> velocity){
  this->texture = texture;
  this->position = position;
  this->velocity = velocity;
  this->destRect.x = 0;
  this->destRect.y = 0;
  this->init();
}


velocityComponent::velocityComponent(){
  this->vx = 0; 
  this->vy = 0;
  this->Moving = true;
  this->IDLE = false;
}
velocityComponent::velocityComponent(float vx, float vy){
  this->vx = vx;
  this->vy = vy;
  this->Moving = false;
  this->IDLE =false;
}

void velocityComponent::setVelocity(float vx , float vy){
  this->vx = vx;
  this->vy = vy;
}

bool velocityComponent::isMoving(){
  return this->Moving;
}

void velocityComponent::setIsMoving(bool status){
  this->Moving = status;
} 
bool velocityComponent::isJumping(){
  return this->jumping;
}

void velocityComponent::setIsJumping(bool status){
  this->jumping = status;
}

//keyBoardInput class functions
keyBoardInput::keyBoardInput(std::shared_ptr<velocityComponent> velocity){
  this->velocity = velocity;
}

collision::collision(SDL_Rect& boundingBox , type Etype){
  this->boundingBox.h = boundingBox.h;
  this->boundingBox.w = boundingBox.w;
  this->boundingBox.x = boundingBox.x;
  this->boundingBox.y = boundingBox.y;
  this->entityType = Etype;
}

void collision::updatePosition(float x , float y){
  this->boundingBox.x = x;
  this->boundingBox.y = y;
}

SDL_Rect& collision::getBoundingBox(){
  return this->boundingBox;
}

type collision::getEntityType(){
  return this->entityType;
}

gravity::gravity(){
  this->accelerationDueToGravity = 10;
}

float gravity::downwardsAcceleration(){
  return this->accelerationDueToGravity;
}


animation:: animation( Uint32 delay){
  this->frameCounter = 0;
  this->frameDelay = delay;
  this->lastUpdatedTime = 0;
  this->currentAnimation = "JUMPING";
}


void animation::insertAnimation(std::string name,std::vector<SDL_Rect>& frames){
  if(this->animationStateArray.find(name) == this->animationStateArray.end()){
    this->animationStateArray[name] = std::make_shared<animationContainer>(frames);
  }
  this->frameCounter =  frames.size();
  
}

void animation::setAnimation(std::string currentAnimation){
  this->currentAnimation = currentAnimation;
}

std::string animation::getCurrentAnimation(){
  return this->currentAnimation;
}

SDL_Rect& animation::getRectNext(){
  return this->animationStateArray[this->currentAnimation]->getRectNextFrame();
}

healthComponent::healthComponent(){
  this->health = 100;
}








