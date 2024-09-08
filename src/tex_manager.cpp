#include "tex_manager.h"
#include <SDL2/SDL_render.h>
#include <memory>
SDL_Renderer* tex_manager::renderer = nullptr; // needs to be defined in a source file for memory allocation
SDL_Texture* tex_manager::grounTex = nullptr;
SDL_Texture* tex_manager::BackgrounTex = nullptr;
SDL_Texture* tex_manager::bulletTex = nullptr;
SDL_Texture* tex_manager::loadTexture(const char* path){

    SDL_Surface* surface = IMG_Load(path); 
    SDL_Texture* texture = SDL_CreateTextureFromSurface(tex_manager::renderer,surface);
    SDL_FreeSurface(surface);
    return texture;

}
void tex_manager::drawTexture(SDL_Texture* texture, bool flip,SDL_Rect* srcRect = NULL, SDL_Rect* destRect = NULL ){
 int param ;
  if(flip){

  SDL_RenderCopyEx(tex_manager::renderer,texture,srcRect,destRect,0,NULL,SDL_FLIP_HORIZONTAL);
  }
  else{
  SDL_RenderCopyEx(tex_manager::renderer,texture,srcRect,destRect,0,NULL,SDL_FLIP_NONE);
  }
}


void tex_manager::loadMapTex(){
  tex_manager::grounTex = tex_manager::loadTexture("../../assets/testSprites/tilePlatform.png");
  tex_manager::BackgrounTex = tex_manager::loadTexture("../../assets/testSprites/Map1.png");
  tex_manager::bulletTex = tex_manager::loadTexture("../../assets/testSprites/Bulletspritesheet.png");
}

