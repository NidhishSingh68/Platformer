#include "system.h"
#include "app.h"
#include "component.h"
#include "componentManager.h"
#include "entity.h"
#include "tex_manager.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <vector>

 std::vector<std::shared_ptr<entity>> system::entites;
system::system(componentManager& CM): CM(CM) {

}
void system::addEntity(std::shared_ptr<entity> entity){
  system::entites.push_back(entity);
}

void system::removeEntity(){
  for(std::size_t i = 0 ; i < entites.size() ; i++){
    std::shared_ptr<entity> entity = entites[i];
    if(entity->DOA() == false){
      entites.erase(entites.begin() + i); 
    }
  }
}

movementSystem::movementSystem(componentManager& CM) : system(CM){
}

void movementSystem::addEntity(std::shared_ptr<entity> entity){
    this->entites.push_back(entity);
}
void movementSystem::update(){
   SDL_DisplayMode dm;
  SDL_GetDesktopDisplayMode(0, &dm);
  for (std::size_t i =0; i < entites.size() ; i++){
    auto entity = entites[i];
    if(entity->hasComponent<velocityComponent>() && entity->hasComponent<spriteComponent>() && entity->DOA() == true){
      auto position = this->CM.getComponent<positionComponent>(entity->getID());
      auto velocity = this->CM.getComponent<velocityComponent>(entity->getID());
      auto collisionComponent = this->CM.getComponent<collision>(entity->getID());
      std::shared_ptr<spriteComponent> sprite = this->CM.getComponent<spriteComponent>(entity->getID());
      //Only apply tranforamtion if the character is moving
      if(velocity->vx !=0){
        if(velocity->vx < 0){
          sprite->invert = true;
        }
        else{
          sprite->invert = false;
        }
      }
      float newPosx = position->x + (velocity->vx  * (1.f / 60.f) );
      float newPosy = position->y + (velocity->vy * (1.f / 60.f)) ;
      if( ( (newPosx <= 0 || newPosx + sprite->destRect.w >= dm.w) )){
        if(collisionComponent->getEntityType() == player){
          position->setPosition(position->x , newPosy);
        }
        if(collisionComponent->getEntityType() == bullet ){
          entity->setDOA(false);
        }
        if(collisionComponent->getEntityType() == enemy){
          velocity->vx*=-1;
        }
      }
      else{

      position->setPosition(newPosx , newPosy);
      }
    }
  }
}


movementSystem::~movementSystem(){

}



keyInputSystem::keyInputSystem(componentManager& CM) : system(CM){
}
void keyInputSystem::spawnBullet(){

   
}
void keyInputSystem::addEntity(std::shared_ptr<entity> entity){
    this->entites.push_back(entity);
}
void keyInputSystem::update(){


  auto velocity = this->CM.getComponent<velocityComponent>(0);
  auto collisionComponent = this->CM.getComponent<collision>(0);
  auto sprite= this->CM.getComponent<spriteComponent>(0);
  auto animationComponent= this->CM.getComponent<animation>(0);
  if(Engine::event.type == SDL_KEYDOWN){
    switch (Engine::event.key.keysym.sym) {
      case SDLK_a:
        velocity->vx = -30;
        velocity->IDLE = false;
        sprite->invert = true;
        animationComponent->setAnimation("RUNNING");
        break;
      case SDLK_d:
        velocity->vx = 30;
        velocity->IDLE = false;
        sprite->invert = false;
        animationComponent->setAnimation("RUNNING");
        break;
      case SDLK_SPACE:
        if(!velocity->isJumping()){ 
          velocity->setIsJumping(true);
          velocity->IDLE = false;
          animationComponent->setAnimation("JUMPING");
          velocity->vy -= 75;
        }
        break;

      default:
        break;


    }
  }
  if(Engine::event.type == SDL_KEYUP){
    switch (Engine::event.key.keysym.sym) {
      case SDLK_a:
        velocity->vx = 0;
        break;
      case SDLK_d:
        velocity->vx = 0;
        break;
      default:
        break;
    }
  }
  if(Engine::event.type == SDL_MOUSEBUTTONDOWN){
    switch (Engine::event.button.button) {
      case SDL_BUTTON_LEFT:
        animationComponent->setAnimation("SHOOTING");
        break;
      
    }
  }
  if(Engine::event.type == SDL_MOUSEBUTTONUP){
    switch (Engine::event.button.button) {
      case SDL_BUTTON_LEFT:
        animationComponent->setAnimation("IDLE");
        break;
      
    }
  }

}

renderSystem::renderSystem(componentManager& CM) : system(CM){};
void renderSystem::addEntity(std::shared_ptr<entity> entity){
    this->entites.push_back(entity);
}

void renderSystem::update() {
  for (std::size_t i =0 ; i < entites.size() ; i++){
    std::shared_ptr<entity> entity = entites[i];
    if(entity->hasComponent<spriteComponent>() && entity->hasComponent<velocityComponent>() && entity->DOA() == true){
      std::shared_ptr<spriteComponent> sprite = this->CM.getComponent<spriteComponent>(entity->getID());
      std::shared_ptr<positionComponent> position = this->CM.getComponent<positionComponent>(entity->getID());
      std::shared_ptr<collision> collisionComponent = this->CM.getComponent<collision>(entity->getID());
      sprite->destRect.x = position->getX();
      sprite->destRect.y = position->getY();
      collisionComponent->getBoundingBox().x = sprite->destRect.x;
      collisionComponent->getBoundingBox().y = sprite->destRect.y;
    }
  }
}

void renderSystem::render(){
  std::shared_ptr<entity> player = entites[0];
  std::shared_ptr<spriteComponent> playerPos = this->CM.getComponent<spriteComponent>(0);
  Engine::camera.x = playerPos->destRect.x + (playerPos->destRect.w / 2 ) - (Engine::camera.w / 2);
  if(Engine::camera.x < 0 ){
    Engine::camera.x = 0;
  }
  if(Engine::camera.x > map::COLUMNS * map::tileWidth - Engine::camera.w){
    Engine::camera.x =  map::COLUMNS * map::tileWidth - Engine::camera.w;
  }
  for (std::size_t i = 0 ; i < entites.size() ; i++){
    std::shared_ptr<entity> entity = entites[i];
    if(entity->hasComponent<spriteComponent>() && entity->DOA() == true){

      auto sprite= this->CM.getComponent<spriteComponent>(entity->getID());
      tex_manager::drawTexture(sprite->texture, sprite->invert,&sprite->srcRect, &sprite->destRect);
    }
  }
}


collisionSystem::collisionSystem(componentManager& CM) : system(CM) {}
bool collisionSystem::detectCollision(SDL_Rect& A, SDL_Rect& B){
  bool result = false;
  //AABB collision
  if( ((A.x + A.w) > B.x ) && ((B.x + B.w) > A.x) && ((A.y + A.h) > B.y ) && ((B.y + B.h) > A.y)){
    result = true; 
  }
  return result;
}

bool collisionSystem::Xcollision(SDL_Rect& A, SDL_Rect& B){
  bool result = false;
  if( ((A.x + A.w) > B.x ) && ((B.x + B.w) > A.x) ){
    result = true; 
  }
  return  result;
}
bool collisionSystem::Ycollision(SDL_Rect& A, SDL_Rect& B){
  bool result = false;
  if(((A.y + A.h) > B.y ) && ((B.y + B.h) > A.y)){
    result = true; 
  }
  return  result;
}
bool collisionSystem::sideCollision(SDL_Rect& A, SDL_Rect& B){

  //Left and right face for box A
  int l1Face = A.x;
  int r1Face = A.x + A.w;

  //Left and right face for box B 
  int l2Face = B.x;
  int r2Face = B.x + B.w;

  if(r1Face > l2Face && r2Face > l1Face){
    return true;
  }
  else{
    return false;
  }

}

SDL_Rect collisionSystem::collisionBB(SDL_Rect& A , SDL_Rect& B){
  SDL_Rect BB;
  float leftP = std::max(A.x , B.x);
  float rightP = std::min(A.x + A.w , B.x + B.w);
  float topP = std::min(A.y + A.h , B.y + B.h); 
  float bottomP = std::max(A.y , B.y);
  float dx = rightP - leftP;
  float dy = topP - bottomP;
  BB.w = dx;
  BB.h = dy;
  return BB;
}

bool collisionSystem::checkHorizontalCollision(SDL_Rect& A , SDL_Rect& B){
  float leftP = std::max(A.x , B.x);
  float rightP = std::min(A.x + A.w , B.x + B.w);
  float topP = std::min(A.y + A.h , B.y + B.h); 
  float bottomP = std::max(A.y , B.y);
  float dx = rightP - leftP;
  float dy = topP - bottomP;
  if(dx > dy){
    return true;
  }
  else{
    return false;
  }
}
void collisionSystem::addEntity(std::shared_ptr<entity> entity){
    this->entites.push_back(entity);
}
void collisionSystem::handlePlayerCollision(){

  auto playerPos = this->CM.getComponent<positionComponent>(0);
  auto playerSprite = this->CM.getComponent<spriteComponent>(0);
  std::shared_ptr<collision> collisionComponentPlayer = this->CM.getComponent<collision>(0);
  auto animationComponent  = this->CM.getComponent<animation>(0);
  for(std::shared_ptr<entity> entity : entites){
    if(entity->DOA() == true && entity->getID()!=0){
      auto collisionComponentTile =  this->CM.getComponent<collision>(entity->getID());
      SDL_Rect playerBB = collisionComponentPlayer->getBoundingBox();
      if( true == this->detectCollision(playerBB, collisionComponentTile->getBoundingBox())) {
        auto velocity = this->CM.getComponent<velocityComponent>(0);
        if(this->Ycollision(playerBB,collisionComponentTile->getBoundingBox())){
          velocity->setIsJumping(false); 
          SDL_Rect tileBB = collisionComponentTile->getBoundingBox();
         //Horizontal collision resolution
          if(checkHorizontalCollision(tileBB, playerBB) ){
            //Head collision
            if(playerBB.y + playerBB.h > tileBB.y + tileBB.h){
              playerPos->y = tileBB.y + tileBB.h;
              velocity->vy*=-1;
            }
            //floor collision
            else{
              //Cap the y component
              playerPos->y = tileBB.y  - playerBB.h;
              //Set the Y velocity to zero 
              if(velocity->vy > 0){
                velocity->vy = 0;           
              }
              if(velocity->vx == 0 && animationComponent->currentAnimation!= "SHOOTING"){
                animationComponent->setAnimation("IDLE");
              }
            }
          }
          //Vertical collision resolution
          else{

            velocity->vx *= -1;
            //right face collision
            if(playerBB.x + playerBB.w >tileBB.x && playerBB.x < tileBB.x){
              playerPos->x = tileBB.x- playerBB.w;
            }
            if(tileBB.x + tileBB.w > playerBB.x && playerBB.x > tileBB.x){
              playerPos->x = tileBB.x +tileBB.w;  
            }
          }
        }
      }

    }
  }
}

void collisionSystem::handleCollision(){
  //Handle player tile collision
  this->handlePlayerCollision(); 
  for(std::size_t i  = 0 ;  i < entites.size() ; i++ ){
    std::shared_ptr<entity> entity1 = entites[i];
    if(entity1->hasComponent<collision>() && entity1->DOA() == true){
      //Check this entity against all entites
      std::shared_ptr<collision> collisionComponentE1 = this->CM.getComponent<collision>(entity1->getID());
      std::shared_ptr<positionComponent> positionE1 = this->CM.getComponent<positionComponent>(entity1->getID());
      for(std::size_t j = i+1 ; j < entites.size() ; j++){
        std::shared_ptr<entity> entity2 = entites[j];
        if(entity2->hasComponent<collision>() && entity2->DOA() == true  && (i!=j)){
       std::shared_ptr<positionComponent> positionE2 = this->CM.getComponent<positionComponent>(entity2->getID());  
      std::shared_ptr<collision> collisionComponentE2 = this->CM.getComponent<collision>(entity2->getID());
          if(detectCollision(collisionComponentE1->getBoundingBox(),collisionComponentE2->getBoundingBox())  ){
            type typeE1 = collisionComponentE1->getEntityType();
            type typeE2 = collisionComponentE2->getEntityType();

            //Bullet wall collision
            if( (typeE1 == bullet && typeE2 == tile) || (typeE2 == bullet && typeE1 == tile) ){
              std::shared_ptr<entity> bullet;
              if(collisionComponentE1->getEntityType() == type::bullet){
                bullet = entity1;
              } 
              else{
                bullet = entity2;
              }
              bullet->setDOA(false);
            }
            //Enemy Tile collision 
            if((typeE1 ==  tile && typeE2 == enemy) || ( typeE1 == enemy && typeE2 == tile ) ){
              std::shared_ptr<positionComponent> enemyPos;
              std::shared_ptr<collision> enemyBB;
              std::shared_ptr<collision> platformBB;
              std::shared_ptr<velocityComponent> enemyVelocity;
              std::shared_ptr<spriteComponent> enemySprite;
              if(typeE1 == enemy){
                enemyPos = positionE1;
                platformBB = collisionComponentE2;
                enemyVelocity = this->CM.getComponent<velocityComponent>(entity1->getID());
                enemySprite= this->CM.getComponent<spriteComponent>(entity1->getID());
                enemyBB = collisionComponentE1;
              }
              else{
                enemyPos = positionE2;
                platformBB = collisionComponentE1;
                enemyVelocity = this->CM.getComponent<velocityComponent>(entity2->getID());

                enemySprite= this->CM.getComponent<spriteComponent>(entity2->getID());
                enemyBB = collisionComponentE2;
              }

                SDL_Rect BB  = collisionBB(enemyBB->getBoundingBox(),platformBB->getBoundingBox());
                //Floor collision 
                if(BB.w > BB.h){
                  enemyPos->y = platformBB->getBoundingBox().y -enemyBB->getBoundingBox().h;
                  enemyVelocity->vy = 0;
                }
                //Vertical collision
                if(BB.h > BB.w){
                if(enemyPos->x < platformBB->getBoundingBox().x){
                  enemyPos->x= platformBB->getBoundingBox().x - enemyBB->getBoundingBox().w;
                }
                else{
                  enemyPos->x= platformBB->getBoundingBox().x + platformBB->getBoundingBox().w;
                }
                  enemyBB->getBoundingBox().x = enemyPos->x;
                  enemyVelocity->vx= enemyVelocity->vx * -1; 

                }

              } 
              //bullet enemy collision
              if(typeE1 == type::enemy && typeE2 == type::bullet || typeE1 == type::bullet && typeE2 == type::enemy){
              std::shared_ptr<entity> enemy;
              std::shared_ptr<entity> bullet;
              if(typeE1 == type::enemy){
                enemy = entity1;
                bullet = entity2;
              }
              else{
                enemy = entity2;
                bullet = entity2;
              }
              enemy->setDOA(false);
              bullet->setDOA(false);
              Engine::scoreCounter++;
            }
            //player enemy collision
              if(typeE1 == type::enemy && typeE2 == type::player || typeE1 == type::player && typeE2 == type::enemy){
              std::shared_ptr<entity> enemy;
              std::shared_ptr<entity> player;
              if(typeE1 == type::enemy){
                enemy = entity1;
                player = entity2;
              }
              else{
                enemy = entity2;
                player = entity1;
              }
              player->setDOA(false);
              Engine::isrunning = false;
            }
          }
        }
      }  
    } 

  } 
}

void collisionSystem::update(){
  this->handleCollision();
}

gravitySystem::gravitySystem(componentManager& CM) : system(CM){
}
void gravitySystem::addEntity(std::shared_ptr<entity> entity){
    this->entites.push_back(entity);
}
void gravitySystem::update(){
  for(std::shared_ptr<entity> entity : entites){
    if(entity->hasComponent<gravity>()){
      auto velocity =  this->CM.getComponent<velocityComponent>(entity->getID());
      auto gravityAcceleration = this->CM.getComponent<gravity>(entity->getID());
        velocity->setVelocity(velocity->vx, velocity->vy  + ((1.f/60.f) * gravityAcceleration->downwardsAcceleration()));
    }
  }
}


animationSystem::animationSystem(componentManager& CM) : system(CM){

}
void animationSystem::addEntity(std::shared_ptr<entity> entity){
    this->entites.push_back(entity);
}
void animationSystem::update(){
  for(std::size_t i = 0 ; i < entites.size() ; i++){
    std::shared_ptr<entity> entity = entites[i];
    if(entity->hasComponent<animation>() && entity->DOA() == true){
      std::shared_ptr<animation> animationComponent = this->CM.getComponent<animation>(entity->getID());
      std::shared_ptr<spriteComponent> sprite = this->CM.getComponent<spriteComponent>(entity->getID());
      std::shared_ptr<velocityComponent> velocity = this->CM.getComponent<velocityComponent>(entity->getID());
      Uint32 delay = animationComponent->frameDelay;
      Uint32 lastUpdatedTime = animationComponent->lastUpdatedTime;
      Uint32 currentTime = SDL_GetTicks();
      if(currentTime - lastUpdatedTime >= delay || lastUpdatedTime==0){
        sprite->srcRect = animationComponent->getRectNext();
        animationComponent->lastUpdatedTime = currentTime;
      }
    }
  }
}


aiSystem::aiSystem(componentManager& CM) : system(CM){

}
void aiSystem::addEntity(std::shared_ptr<entity> entity){
    this->entites.push_back(entity);
}
void aiSystem::update(){
  auto playerPos = this->CM.getComponent<positionComponent>(0);
  for (auto entity : entites){
    if(entity->hasComponent<enemyAI>() && entity->DOA() == true ){
      auto AI =this->CM.getComponent<enemyAI>(entity->getID());
      auto enemyAnimation = this->CM.getComponent<animation>(entity->getID());
      auto enemyPos = this->CM.getComponent<positionComponent>(entity->getID());  
      auto enemyVelocity = this->CM.getComponent<velocityComponent>(entity->getID());
      int dirctionX = (playerPos->x - enemyPos->x) / (playerPos->x - enemyPos->x); 
      enemyVelocity->vx *= dirctionX;
      if(std::abs(playerPos->x - enemyPos->x) < AI->thresholdDistance){
        AI->isAttacking = true;
        enemyAnimation->setAnimation("WALKING");
      } 
      else{
        AI->isAttacking = false;
        enemyAnimation->setAnimation("WALKING");
      }
    }
  }
}
