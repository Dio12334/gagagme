#ifndef ACTOR_H
#define ACTOR_H

#include<vector>
#include"Math.h"

class Actor{
  public:
      //Estado del actor
      enum State{
        EActive,
        EPaused,
        EDead
      };
      //constructor y destructor
      Actor(class Game* game);
      virtual ~Actor();
      // estos se encargan de actualizar al actor
      void Update(float deltaTime);
      void UpdateComponents(float deltaTime);
      virtual void UpdateActor(float deltaTime);
      
      // getters y setters
      void SetState(State state){mState = state;}
      State GetState() const{return mState;}
      void SetPosition(const Vector2& position){mPosition = position;}
      const Vector2& GetPosition() const{return mPosition;}
      void SetScale(float scale){mScale = scale;}
      float GetScale() const{return mScale;}
      void SetRotation(float rotation){mRotation = rotation;}
      float GetRotation() const{ return mRotation;}
      class Game* GetGame() const{return mGame;}

      // metodos para agregar y quitar componentes
      void AddComponent(class Component* component);
      void RemoveComponent(class Component* component);

  private:
      State mState;
      Vector2 mPosition;
      float mScale;
      float mRotation;
      std::vector<class Component*> mComponents;
      class Game* mGame;
};
#endif
