#include "animationContainer.h"
#include <SDL2/SDL_rect.h>
#include <memory>
#include <vector>


animationContainer::animationContainer(std::vector<SDL_Rect>& frames){
  this->currentFrame = 0;
  this->frames  = frames;
  this->size = frames.size();
}


SDL_Rect& animationContainer::getRectNextFrame(){
  return frames[this->currentFrame++ % size];
}
