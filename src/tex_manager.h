#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <memory>
class tex_manager{
public:
  
  static SDL_Texture* grounTex;
  static SDL_Texture* BackgrounTex;
  static SDL_Texture* bulletTex;
  static SDL_Renderer* renderer;
  static SDL_Texture* loadTexture(const char* path);
  static void drawTexture(SDL_Texture* texture, bool flip, SDL_Rect* srcRect, SDL_Rect* destRect );
  static void loadMapTex();
};
