#ifndef GAME_H
#define GAME_H

#include<vector>
#include<SDL2/SDL.h>
#include<string>
#include<unordered_map>

class Game{
  public:
      //Constructor
      Game();
      // Inicializador del juego
      bool Initialize();
      // Main loop
      void RunLoop();
      // Cierra el juedgo "destructor"
      void Shutdown();

      // Agrega actores al vector mActors o mPending Actors
      void AddActor(class Actor* actor);

      // REmueve al actor 
      void RemoveActor(class Actor* actor);

      // Agrega y remueve sprites respectivamente
      void AddSprite(class SpriteComponent* sprite);
      void RemoveSprite(class SpriteComponent* sprite);

      // Obtiene una textura de mTextures si existe
      SDL_Texture* GetTexture(const std::string& filename);

  private:
      // mapa de nombre de textura con su textura
      std::unordered_map<std::string, SDL_Texture*> mTextures;
      // los sprites que se tienen que dibujar
      std::vector<class SpriteComponent*> mSprites;

      //se llaman en el main loop
      void ProcessInput();
      void UpdateGame();
      void GenerateOutput();

      // Carga y eliminacion de la data
      void LoadData();
      void UnloadData();

      //ventana
      SDL_Window* mWindow;
      // bool para saber si el juego corre
      bool mIsRunning;
      //renderer
      SDL_Renderer* mRenderer;
      // contador de ticks
      Uint32 mTicksCount;
      
      // actores activos y pendientes
      std::vector<class Actor*> mActors;
      std::vector<class Actor*> mPendingActors;

      // bool para saber en que vector poner un nuevo actor
      bool mUpdatingActors;
};

#endif
