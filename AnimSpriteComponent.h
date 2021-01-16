#ifndef ANIMSPRITECOMPONENT_H
#define ANIMSPRITECOMPONENT_H

#include"SpriteComponent.h"
#include<vector>


//clase hija de SpriteComponent
class AnimSpriteComponent: public SpriteComponent{
  public:
      //constructor
      AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
      //Aqui hacemos nuestra implementacion del Update
      void Update(float deltaTime) override;
      //setters y getters
      void SetAnimTextures(const std::vector<SDL_Texture*>& textures);
      float GetAnimFPS() const {return mAnimFPS;}
      void SetAnimFPS(float fps){mAnimFPS = fps;}
  private:
      std::vector<SDL_Texture*> mAnimTextures;
      float mCurrFrame;
      float mAnimFPS;
};

#endif
