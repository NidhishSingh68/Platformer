#include "entityManager.h"
#include "component.h"
#include "entity.h"
#include "system.h"
#include "tex_manager.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>
#include <cstddef>
#include <memory>
#include <ostream>


entityManager::entityManager() : moveableEntites(this->CM) ,keyboardInputAffectedEntites(this->CM), collidables(this->CM), gravityAffectedEntites(this->CM) , texturedEntites(this->CM) , animatedEntites(this->CM),AIPoweredEntites(this->CM){
  this->EID = -1;
  this->entityCounter = 0;
}

std::size_t entityManager::createEntity(){
  this->EID++;
  this->entityCounter++;
  this->entites[EID] = std::make_shared<entity>(EID);
  system::addEntity(this->entites[EID]);
  return EID;
}

void entityManager::addSpriteToEntity(entityID ID , const char* path){
 spriteComponent component(tex_manager::loadTexture(path),this->CM.getComponent<positionComponent>(ID) , this->CM.getComponent<velocityComponent>(ID)); 
 this->entites[ID]->registerComponent<spriteComponent>();
 CM.addComponentToEntity(ID, component);

}

void entityManager::addTile(SDL_Rect& destRect){
  std::size_t tileID = this->createEntity();
  this->platforms.push_back(this->entites[tileID]);
  SDL_Rect srcRect;
  srcRect.x = srcRect.y = 0;
  SDL_QueryTexture(tex_manager::grounTex, NULL, NULL, &srcRect.w, &srcRect.h);
  this->addComponentToEntity<positionComponent>(tileID,destRect.x ,destRect.y);
  //this->addComponentToEntity<spriteComponent>(tileID, tex_manager::grounTex , srcRect , destRect);
  this->addComponentToEntity<collision>(tileID, destRect, type::tile);

}

void entityManager::renderEntites(){
  this->texturedEntites.render();
}

void entityManager::destroyEntity(){
  SDL_DisplayMode dm;
  SDL_GetDesktopDisplayMode(0, &dm);
  for(std::size_t bulletID =0 ; bulletID < this->bullets.size() ; bulletID ++){
    std::shared_ptr<spriteComponent> sprite = this->CM.getComponent<spriteComponent>(bullets[bulletID]->getID());
    std::shared_ptr<collision> BB = this->CM.getComponent<collision>(bullets[bulletID]->getID());
    if(sprite->destRect.x > dm.w || sprite->destRect.x + sprite->destRect.w < 0){
      this->CM.removeEntity(bullets[bulletID]->getID());
      bullets[bulletID]->setDOA(false);
      this->bullets.erase(bullets.begin() + bulletID);
      std::cout<<"Destroyed" << std::endl;
      std::cout<<"size" << this->bullets.size()<<std::endl;
    }
  }
 for(std::size_t ID = 0 ; ID < this->enemey.size() ; ID++){
    std::shared_ptr<entity> enemy = this->enemey[ID];
    if(!enemy->DOA()){
      this->CM.removeEntity(enemy->getID());
      this->enemey.erase(enemey.begin() + ID);
      
    }
  } 

  system::removeEntity();  
}

void entityManager::updateEntities(){
  this->spawnEnemy();
  this->keyboardInputAffectedEntites.update();
  this->gravityAffectedEntites.update();
  this->moveableEntites.update();
  this->collidables.update();
  this->texturedEntites.update();
  this->animatedEntites.update();
  this->AIPoweredEntites.update();

}

void entityManager::spawnBullet(){  
  entityID bulletID = this->createEntity();
  this->bullets.push_back(this->entites[bulletID]); 
  auto sprite = this->CM.getComponent<spriteComponent>(0);
  std::vector<SDL_Rect> bulletSpriteSheet;
  SDL_Rect frame;
  frame.h = 12;
  frame.w = 12;
  frame.x = 0;
  frame.y = 0;
  bulletSpriteSheet.push_back(frame);
  frame.x = 21;
  bulletSpriteSheet.push_back(frame);
  frame.x = 42;
  bulletSpriteSheet.push_back(frame);
  frame.x = 63;
  bulletSpriteSheet.push_back(frame);
  this->addComponentToEntity<animation>(bulletID, 50);
  auto animationComponent = this->CM.getComponent<animation>(bulletID);
  animationComponent->insertAnimation("PROPOGATE", bulletSpriteSheet);
  animationComponent->setAnimation("PROPOGATE");
  int bulletSpeed = 200;
  //Set the bullet speed wrt to the sprite position
  if( false == sprite->invert ){
    this->addComponentToEntity<positionComponent>(bulletID,sprite->destRect.x + sprite->destRect.w , sprite->destRect.y + 11);   
    this->addComponentToEntity<velocityComponent>(bulletID,bulletSpeed , 0 );
    //sprite->destRect.x -= 10;
  }
  else{
    this->addComponentToEntity<positionComponent>(bulletID,sprite->destRect.x , sprite->destRect.y + 11);   
    this->addComponentToEntity<velocityComponent>(bulletID,bulletSpeed*-1 , 0 );
    //sprite->destRect.x +=10;
  }
  this->addComponentToEntity<spriteComponent>(bulletID, tex_manager::bulletTex , 32, 32);
  SDL_Rect BBB;
  BBB.h = 12;
  BBB.w = 12;
  BBB.x = sprite->destRect.x;
  BBB.y = sprite->destRect.y + 11;
  this->addComponentToEntity<collision>(bulletID,BBB,type::bullet);
   
  
}


void entityManager::spawnEnemy(){
  if(this->enemey.size() < 10){
    std::size_t enemyID = this->createEntity();
    this->enemey.push_back(this->entites[enemyID]);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::mt19937::result_type> distr(0, this->platforms.size()-1);
    int platformIndex = distr(gen);
    std::shared_ptr<collision> tileBB = this->CM.getComponent<collision>(this->platforms[platformIndex]->getID());
    SDL_Rect enemyBB;
    enemyBB.w = 50;
    enemyBB.h = 50;
    std::uniform_int_distribution<std::mt19937::result_type> distr2(tileBB->getBoundingBox().x, tileBB->getBoundingBox().x+tileBB->getBoundingBox().w);
    int xCord = distr2(gen);
    enemyBB.x =xCord;
    enemyBB.y = tileBB->getBoundingBox().y - enemyBB.h ;

    this->addComponentToEntity<positionComponent>(enemyID,enemyBB.x ,enemyBB.y);
    this->addComponentToEntity<velocityComponent>(enemyID,20,0);
    this->addComponentToEntity<collision>(enemyID,enemyBB,enemy);
    this->addComponentToEntity<gravity>(enemyID);
    this->addComponentToEntity<spriteComponent>(enemyID,tex_manager::loadTexture("../../assets/Eye ball Monster/EyeBall Monster-Sheet.png") , enemyBB.w,enemyBB.h);
    this->addComponentToEntity<enemyAI>(enemyID);

    std::vector<SDL_Rect> IDLE;
    SDL_Rect enemyIdle;
    enemyIdle.x = 24;
    enemyIdle.y = 354;
    enemyIdle.w = 12;
    enemyIdle.h = 14;
    IDLE.push_back(enemyIdle);
    enemyIdle.y = 401; 
    enemyIdle.h = 15;  
    IDLE.push_back(enemyIdle);
    enemyIdle.x = 25;
    enemyIdle.y = 450;
    enemyIdle.h = 15;
    IDLE.push_back(enemyIdle);
    enemyIdle.x = 24;
    enemyIdle.y = 498;
    IDLE.push_back(enemyIdle);
    enemyIdle.y = 547;
    enemyIdle.h = 13;
    IDLE.push_back(enemyIdle);
    enemyIdle.x = 25;
    enemyIdle.y = 595;
    enemyIdle.h = 13;
    IDLE.push_back(enemyIdle);
    this->addComponentToEntity<animation>(enemyID,50);
    this->CM.getComponent<animation>(enemyID)->insertAnimation("WALKING",IDLE);
    this->CM.getComponent<animation>(enemyID)->setAnimation("WALKING");
    //Attack Animation
    
    
    
    
    
  }
}

