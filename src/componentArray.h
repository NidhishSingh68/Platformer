#pragma once
#include <cstddef>
#include <memory>
#include <unordered_map>
#include "entity.h"

using componentID = std::size_t; 

class IcomponentArray{
public:
  virtual ~IcomponentArray() = default;
  virtual void destroyEntity(entityID ID) = 0;
};



template <typename T>
class componentArray : public IcomponentArray{
  private:
    std::unordered_map<entityID, std::shared_ptr<T>> components; 
  public:
    void addComponent(entityID ID, T component){
      components[ID] = std::make_shared<T>(component);
    }
    void destroyEntity(entityID ID) override{
    if(components.find(ID)!= components.end()){
      components.erase(ID);
    }
        

    }
    void removeComponent(entityID ID){
    if(components.find(ID)!= components.end()){
      components.erase(ID);
    }
    }
    
    std::shared_ptr<T> getComponent(entityID ID){
      
      return components[ID];
    }
        
    
};

