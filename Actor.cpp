#include"Actor.h"
#include"Game.h"
#include"Component.h"
#include<algorithm>
//constructor setea a todos por un defecto
Actor::Actor(class Game* game): mState(EActive), mPosition(Vector2::Zero), mScale(1.0f), mRotation(0.0f), mGame(game){}
Actor::~Actor(){
  //eliminamos al actor del juego al que pertenece
  mGame->RemoveActor(this);
  // eliminamos las componentes
  while(!mComponents.empty())
      delete mComponents.back();
  //luego todo muere por defecto (no pienses mucho en ello)
}

//metodo principal para actualizar
void Actor::Update(float deltaTime){
  if(mState == EActive){
    //actualizamos las componentes y luego al actor
    UpdateComponents(deltaTime);
    UpdateActor(deltaTime);
  }
}

//vamos por todas las componentes y actualizamos cada una de ellas
void Actor::UpdateComponents(float deltaTime){
  for(auto comp: mComponents)
      comp->Update(deltaTime);
}

// esto varia dependiendo del actor que herede esta clase
void Actor::UpdateActor(float deltaTime){}

//agregamos componentes
void Actor::AddComponent(class Component* component){
  // al igual que el vector de sprites en Game.h este vector tambien se ordera de manera ascendente
  // esto para que los componentes se actualizen en el orden correcto
  int myOrder = component->GetUpdateOrder();
  auto iter = mComponents.begin();
  for(; iter != mComponents.end(); iter++)
      if(myOrder < (*iter)->GetUpdateOrder())
          break;
  mComponents.insert(iter, component);
}
void Actor::RemoveComponent(class Component* component){
  //encontramos y eliminamos :p
  auto iter = std::find(mComponents.begin(), mComponents.end(), component);
  if(iter != mComponents.end())
      mComponents.erase(iter);
}

