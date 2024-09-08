
#include "app.h"
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_timer.h>
#include <ostream>


int main(){
  Engine app;
  float FPS = 60;
  Uint32 prevTime , currentTime;
  Uint32 frameTime = (1.f/FPS) * 1000;
  app.init("Platformer", 1900, 1350, 1);

  while(app.running()){
    prevTime = SDL_GetTicks();
    app.handleEvents();
    app.update();
    app.render();
    currentTime = SDL_GetTicks();
    if( (currentTime - prevTime) < frameTime){
      SDL_Delay(currentTime - prevTime);
    }
  }
  app.clean();
}
