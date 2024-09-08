#pragma once
#include "entity.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <vector>
#include "entityManager.h"
#include "component.h"
#include "system.h"
#include "map.h"

class Engine{


   public:
  enum gameState{
    splashScreen,
    playing,
    restartMenu
  };
  static SDL_Window* window;
  static SDL_Rect camera;
  static SDL_Event event;
  static float scoreCounter;
  static gameState currentState;

  static bool isrunning;
   Engine();
   void init(const char* title, int width, int height, bool fullscreen);
   void handleEvents();
   void update();
   void render();
   void clean();
   bool running();
   ~Engine();
private:
  SDL_Renderer* renderer;
  std::vector<entity> entities;
  entityManager EM;
  map tileMap;


};

