#pragma once


#include <SDL2/SDL_rect.h>
#include <memory>
#include <vector>
class animationContainer{
  std::size_t size;
  public:
  animationContainer(std::vector<SDL_Rect>& frames);
    std::vector<SDL_Rect> frames;
  int currentFrame;
  float delay;
  SDL_Rect& getRectNextFrame();
  

};
