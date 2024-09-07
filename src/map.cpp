#include "map.h"
#include "app.h"
#include "componentManager.h"
#include "entityManager.h"
#include "tex_manager.h"
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstddef>



map::map(){

}

void map::init(entityManager* EM){
  this->background_sprite = tex_manager::BackgrounTex;
  this->ground_sprite = tex_manager::grounTex;
  this->EM  = EM;
  SDL_Rect orignalDim;
  SDL_Rect ScaledDim;
  SDL_QueryTexture(tex_manager::BackgrounTex, NULL, NULL, &orignalDim.w, &orignalDim.h);
  SDL_GetWindowSize(Engine::window,&ScaledDim.w, &ScaledDim.h);
  float scaleX = (float)ScaledDim.w / orignalDim.w;
 float scaleY = (float)ScaledDim.h / orignalDim.h;
  SDL_Rect dest;
  dest.w = 159 * scaleX;
  dest.h = 30 * scaleY;
  dest.x = 0 * scaleX;
  dest.y = 450 * scaleY;
  this->EM->addTile(dest);

  dest.w = 33 * scaleX;
  dest.h = 66 * scaleY;
  dest.x = 159 * scaleX;
  dest.y = 417 * scaleY;
  this->EM->addTile(dest);


  dest.w = 160 * scaleX;
  dest.h = 96 * scaleY;
  dest.x = 192 * scaleX;
  dest.y = 384 * scaleY;
  this->EM->addTile(dest);
  
  dest.w = 161 * scaleX;
  dest.h = 33 * scaleY;
  dest.x = 352 * scaleX;
  dest.y = 448 * scaleY;
  this->EM->addTile(dest);

  dest.w = 161 * scaleX;
  dest.h = 162 * scaleY;
  dest.x = 511 * scaleX;
  dest.y = 287 * scaleY;
  this->EM->addTile(dest);


  dest.w = 32 * scaleX;
  dest.h = 32 * scaleY;
  dest.x = 192 * scaleX;
  dest.y = 192 * scaleY;
  this->EM->addTile(dest);

  dest.w = 32 * scaleX;
  dest.h = 32 * scaleY;
  dest.x = 320 * scaleX;
  dest.y = 192 * scaleY;
  this->EM->addTile(dest);
  
  dest.w = 32 * scaleX;
  dest.h = 96 * scaleY;
  dest.x = 416 * scaleX;
  dest.y = 160 * scaleY;
  this->EM->addTile(dest);

  dest.w = 96 * scaleX;
  dest.h = 32 * scaleY;
  dest.x = 384 * scaleX;
  dest.y = 288 * scaleY;
  this->EM->addTile(dest);

}





void map::renderMap(){
    //Load the scene
  SDL_Rect dest;
  SDL_DisplayMode dm;
  SDL_GetDesktopDisplayMode(0, &dm );
  dest.x = 0;
  dest.y = 0;
  dest.h = dm.h;
  dest.w = dm.w;
  //Render background sprite
  tex_manager::drawTexture(this->background_sprite, false,NULL, &dest);
 }






