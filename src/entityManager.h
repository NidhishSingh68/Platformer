#pragma once
#include "system.h"
#include "tex_manager.h"
#include "componentManager.h"
#include "component.h"
#include "entity.h"
#include <SDL2/SDL_rect.h>
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <iterator>
#include <memory>
#include <set>
#include <unordered_map>
#include <utility>
#include <vector>
#include <random>
class entityManager{

  private:
    componentManager CM;

    std::unordered_map<entityID,std::shared_ptr<entity>> entites;
    std::vector<std::shared_ptr<entity>> bullets;
    std::vector<std::shared_ptr<entity>> platforms;
    std::vector<std::shared_ptr<entity>> enemey;
    std::size_t EID;
    
    std::size_t entityCounter;
    movementSystem moveableEntites;
    collisionSystem collidables;
    keyInputSystem keyboardInputAffectedEntites;
    gravitySystem gravityAffectedEntites;
    renderSystem texturedEntites;
    animationSystem animatedEntites;
    aiSystem AIPoweredEntites;
  public:
    entityManager();
    std::size_t createEntity(); 
    void destroyEntity();
    template<typename T ,typename... Args>
    void addComponentToEntity(entityID ID,Args&&... args){

      entites[ID]->registerComponent<T>();
      T component(std::forward<Args>(args)...);
      CM.addComponentToEntity(ID, component);
    }
    
    template<typename T>
    std::shared_ptr<T> getComponent(entityID ID){
      return this->CM.getComponent<T>(ID);
      
    }
    void spawnBullet(); 
    void addSpriteToEntity( entityID ID, const char* path);     
    void addTile( SDL_Rect& destRect);
    void updateEntities();
    void renderEntites();
    void spawnEnemy();
    

   
};
