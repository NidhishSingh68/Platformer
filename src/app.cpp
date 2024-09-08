#include "app.h"
#include "component.h"
#include "tex_manager.h"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <string>
#include <vector>


SDL_Rect Engine::camera;

SDL_Event Engine::event;
SDL_Window* Engine::window;
Engine::gameState Engine::currentState;
float Engine::scoreCounter;
bool Engine::isrunning;

Engine::Engine() {
  Engine::currentState = playing;
  isrunning = true;
}
void Engine::init(const char* title, int width , int height, bool fullscreen){
  if(SDL_Init(SDL_INIT_VIDEO)<0){
    std::cout<<"Lol"<<'\n';
  }
  if(fullscreen){
    SDL_DisplayMode dm;
  if (SDL_GetDesktopDisplayMode(0, &dm) != 0)
    {
     SDL_Log("SDL_GetDesktopDisplayMode failed: %s", SDL_GetError());
    }
  
    Engine::window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, dm.w, dm.h, 0);
    Engine::camera.w = dm.w /2;
    Engine::camera.h = dm.h /2;
  }
  else{
    Engine::window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, 0);
  }
  if(!Engine::window){
    std::cout<<"Couldn't initialize window"<<'\n';
  }
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if(!renderer){
    std::cout<<"Couldn't initialize renderer"<<'\n';
  }
 tex_manager::renderer = renderer;
  tex_manager::loadMapTex();
  std::size_t player = this->EM.createEntity();
  this->EM.addComponentToEntity<positionComponent>(player);
  this->EM.addComponentToEntity<velocityComponent>(player);
  this->EM.addComponentToEntity<gravity>(player);
  this->EM.addComponentToEntity<spriteComponent>(player,tex_manager::loadTexture("../../assets/testSprites/SpriteSheet_player.png"));
  SDL_Rect playerBB;
  playerBB.w = 64;
  playerBB.h = 64;
  playerBB.x = 0;
  playerBB.y = 0;
  this->EM.addComponentToEntity<collision>(player,playerBB, type::player);
  //BoundingBox position for IDLE Animation
  std::vector<SDL_Rect> IdleBB;
  //frame1
  SDL_Rect frame;
  frame.w = 20;
  frame.h = 30;
  frame.x = 15;
  frame.y = 15;
  IdleBB.push_back(frame);
  //frame2
  frame.w = 20;
  frame.h = 29;
  frame.x = 60;
  frame.y = 16;
  IdleBB.push_back(frame);
  //frame3
  frame.w = 20;
  frame.h = 29;
  frame.x = 105;
  frame.y = 16;
  IdleBB.push_back(frame);
  //frame4
  frame.w = 20;
  frame.h = 29;
  frame.x = 150;
  frame.y = 16;
  IdleBB.push_back(frame);
  //frame5
  frame.w = 20;
  frame.h = 30;
  frame.x = 195;
  frame.y = 15;
  IdleBB.push_back(frame);
  //frame6
  frame.w = 20;
  frame.h = 30;
  frame.x = 240;
  frame.y = 15;
  IdleBB.push_back(frame);
  //BoundingBox position for Running Animation
  std::vector<SDL_Rect> Running;
  //Frame1
  frame.w = 24;
  frame.h = 30;
  frame.x = 282;
  frame.y = 15;
  Running.push_back(frame);
  //frame2
  frame.w = 22;
  frame.h = 29;
  frame.x = 328;
  frame.y = 16;
  Running.push_back(frame);
  //frame3
  frame.w = 20;
  frame.h = 29;
  frame.x = 15;
  frame.y = 61;
  Running.push_back(frame);
  //frame4
  frame.w = 20;
  frame.h = 31;
  frame.x = 60;
  frame.y = 60;
  Running.push_back(frame);
  
  //frame5
  frame.w = 20;
  frame.h = 30;
  frame.x = 105;
  frame.y = 60;
  Running.push_back(frame);

  //frame6
  frame.w = 20;
  frame.h = 29;
  frame.x =150;
  frame.y = 61;
  Running.push_back(frame);

  //frame7
  frame.w = 19;
  frame.h = 29;
  frame.x = 196;
  frame.y = 61;
  Running.push_back(frame);


  //frame8
  frame.w = 20;
  frame.h = 30;
  frame.x = 241;
  frame.y = 60;
  Running.push_back(frame);

  
  //BoundingBox position for Jumping Animation
  std::vector<SDL_Rect> jump;
  frame.w = 20;
  frame.h = 24;
  frame.x = 284;
  frame.y = 66;

  //jump.push_back(frame);

  frame.w = 20;
  frame.h = 31;
  frame.x = 330;
  frame.y = 59;
  jump.push_back(frame);
  //BoundingBox position for shooting Animation
 
  std::vector<SDL_Rect> shooting;
  frame.w = 28;
  frame.h = 30;
  frame.x  = 104;
  frame.y = 105;

  shooting.push_back(frame);

  frame.w = 24;
  frame.h = 30;
  frame.x  = 150;
  frame.y = 105;
  shooting.push_back(frame);

  this->EM.addComponentToEntity<animation>(player, 40);
  auto animationComponent = this->EM.getComponent<animation>(player);
  animationComponent->insertAnimation("IDLE",IdleBB);
  animationComponent->insertAnimation("RUNNING",Running);
  animationComponent->insertAnimation("SHOOTING",shooting);
  animationComponent->insertAnimation("JUMPING",jump);
  this->EM.addComponentToEntity<keyBoardInput>(player, this->EM.getComponent<velocityComponent>(player));
  this->tileMap.init(&EM);
}

void Engine::handleEvents(){
  
  if(SDL_PollEvent(&Engine::event)!=0){
    if(Engine::event.type == SDL_KEYDOWN){
      switch (Engine::event.key.keysym.sym){
        case SDLK_ESCAPE:
          this->isrunning = false;
      }
    }       
  
  if(Engine::event.type == SDL_MOUSEBUTTONDOWN){
    switch (Engine::event.button.button) {
      case SDL_BUTTON_LEFT:
        this->EM.spawnBullet();
        break;
      
    }
  }
  }

}

void Engine::update(){
 
 this->EM.updateEntities(); 
 this->EM.destroyEntity();
  this->EM.spawnEnemy();
}
void Engine::render(){
  SDL_RenderClear(renderer);
  this->tileMap.renderMap(); 
  this->EM.renderEntites();
  SDL_RenderPresent(renderer);
}

bool Engine::running(){
  return isrunning;
}
void Engine::clean(){

}
Engine::~Engine()
{

}

