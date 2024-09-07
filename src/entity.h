#pragma once
#include "component.h"
#include <algorithm>
#include <bitset>
#include <cstddef>
#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>
#include <vector>
#include "iostream"
using entityID = std::size_t;
using componentID = std::size_t;
#define MAX_COMPONENTS 32
class entity{
  private:
    entityID ID;
    bool Active;
    std::bitset<MAX_COMPONENTS> signature;
    componentID incrementComponentCounter(){
      static componentID CIDTracker = 0;
      return CIDTracker++;
    }

    template<typename T>
    componentID getComponentTypeID(){
      static componentID TypeID = incrementComponentCounter();
      
      return TypeID;
    }
    
  public:
    entity(entityID ID){
      this->ID = ID;
      this->signature.reset();
      this->setDOA(true);
    }
    


    entityID getID(){
      return this->ID;
    }
    void setDOA(bool DOA){
      this->Active = DOA;
    }
    bool DOA(){
      return this->Active;
    }
    template<typename T>
    void registerComponent(){
      signature[getComponentTypeID<T>()] = true;
    }
    template<typename T>
    bool hasComponent(){
      return signature.test(getComponentTypeID<T>());
    }
   
};
