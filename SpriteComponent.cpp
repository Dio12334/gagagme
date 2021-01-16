#include"SpriteComponent.h"
#include"Actor.h"
#include"Game.h"

// la misma wea de cualquier constructor
SpriteComponent::SpriteComponent(class Actor* owner, int drawOrder): Component(owner), mTexture(nullptr), mDrawOrder(drawOrder), mTexWidth(0), mTexHeight(0){
  //Agrega el sprite al Game
  mOwner->GetGame()->AddSprite(this);
}
SpriteComponent::~SpriteComponent(){
  //removemos el sprite del Game
  mOwner->GetGame()->RemoveSprite(this);
}
void SpriteComponent::Draw(SDL_Renderer* renderer){
  if(mTexture){
    //creamos el rectangulo en el que dibujaremos el sprite
    SDL_Rect r;
    r.w = static_cast<int>(mTexWidth* mOwner->GetScale());
    r.h = static_cast<int>(mTexHeight*mOwner->GetScale());
    r.x = static_cast<int>(mOwner->GetPosition().x - r.w/2);
    r.y = static_cast<int>(mOwner->GetPosition().y - r.h/2);

    // Esta funcion dibuja el sprite en la pantalla
    SDL_RenderCopyEx(renderer, mTexture, nullptr, &r, -Math::ToDegrees(mOwner->GetRotation()), nullptr, SDL_FLIP_NONE);
  }
}
//un setter 
void SpriteComponent::SetTexture(SDL_Texture* texture){
  mTexture = texture;
  // ni idea para que es esta wea (creo que conseguia el tamaño de la textura)
  SDL_QueryTexture(texture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}


