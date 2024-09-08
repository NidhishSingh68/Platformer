#pragma once
#include "component.h"
#include "componentArray.h"
#include "entity.h"
#include <cstddef>
#include <iterator>
#include <memory>
#include <unordered_map>
#include <vector>
#include <typeinfo>
using namespace std;


class componentManager{
  private:
    
    std::vector<std::shared_ptr<IcomponentArray>> componentPool;
    std::unordered_map<const char*, std::size_t> indexMap;
    
    template<typename T>
    std::size_t getIndex(){
      const char* typeName = typeid(T).name();
      return indexMap[typeName];
    }
  public:
    template<typename T>
    void registerComponent(){
      if(indexMap.find(typeid(T).name()) == indexMap.end()){
        indexMap[typeid(T).name()] = componentPool.size(); 
        componentPool.push_back(std::make_shared<componentArray<T>>());
      }
    }
    template<typename T>
    void addComponentToEntity(entityID ID , T component){
      if(indexMap.find(typeid(T).name()) == indexMap.end()){
        this->registerComponent<T>();
      }
      static_cast<componentArray<T>*>(componentPool[this->getIndex<T>()].get())->addComponent(ID , component);
    }
    
    template<typename T> 
    std::shared_ptr<T> getComponent(entityID ID){
      if(indexMap.find(typeid(T).name()) == indexMap.end()){
        return nullptr;
      }
      else{
        auto ptr = static_cast<componentArray<T>*>(componentPool[this->getIndex<T>()].get());        
        return ptr->getComponent(ID);
      
      }
    }

    void removeEntity(entityID ID){
      for(std::shared_ptr<IcomponentArray> arr : componentPool){
        arr->destroyEntity(ID);
      }
      
       
    }

};


