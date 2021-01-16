#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include"Component.h"
#include<SDL2/SDL.h>

// clase que hereda de Component
class SpriteComponent: public Component{
  public:
      //constructor y destructor
      SpriteComponent(class Actor* owner, int drawOrder = 100);
      ~SpriteComponent();
      
      // dibujado y seteo de textura
      virtual void Draw(SDL_Renderer* renderer);
      virtual void SetTexture(SDL_Texture* texture);
      
      // getters
      int GetDrawOrder() const {return mDrawOrder;}
      int GetTexHeight() const {return mTexHeight;}
      int GetTexWidth() const {return mTexWidth;}
  protected:
      SDL_Texture* mTexture;
      int mDrawOrder;
      int mTexWidth;
      int mTexHeight;
};

#endif
