#include"Game.h"
#include"Actor.h"
#include<algorithm>
#include<SDL2/SDL_image.h>
#include"SpriteComponent.h"
#include"BGSpriteComponent.h"
// el constructor setea todo a los valores iniciales
Game::Game(): mWindow(nullptr), mRenderer(nullptr), mIsRunning(true), mTicksCount(0), mUpdatingActors(false){}

bool Game::Initialize(){
  // Inicializa SDL
  int sdlResult = SDL_Init(SDL_INIT_VIDEO);
  if(sdlResult != 0){
    SDL_Log("Unabletoinitialize SDL: %s", SDL_GetError());
    return false;
  }
  // Crea la ventana
  mWindow = SDL_CreateWindow("titulo", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,800, 640, 0 );
  if(!mWindow){
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }
  // Crea el renderizador
  mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
  if(!mRenderer){
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    return false;
  }
  // Inicializa SDL_image
  if(IMG_Init(IMG_INIT_PNG) == 0){
    SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
    return false;
  }
  // Carga la data del juego
  LoadData();
  // consigue los ticks iniciales
  mTicksCount = SDL_GetTicks();
  return true;
}

//main loop: recibe, actualiza y devuelve mientras el juego corra
void Game::RunLoop(){
  while(mIsRunning){
    ProcessInput();
    UpdateGame();
    GenerateOutput();
  }
}

// cierra el juego y cierra todo lo correspondiente
void Game::Shutdown(){
  // Mirar la explicacion del metodo
  UnloadData();
  // cierra SDL_image
  IMG_Quit();
  //destruye la ventana y vacía el puntero
  SDL_DestroyWindow(mWindow);
  // destruye el renderizador y vacía el puntero
  SDL_DestroyRenderer(mRenderer);
  // cierra SDL
  SDL_Quit();
}

// Agrega un Actor nuevo a mActors o mPendingActors, dependiendo si se actualiza los actores en ese momento
void Game::AddActor(class Actor* actor){
 if(mUpdatingActors)
     mPendingActors.emplace_back(actor);
 else
     mActors.emplace_back(actor);
}

// Elimina el actor dependiendo del vector en el que está
void Game::RemoveActor(class Actor* actor){
  auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end()){
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end()){
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

// Procesa las entradas
void Game::ProcessInput(){
  //Un evento de SDL (no se bien que hace)
  SDL_Event event;
  while(SDL_PollEvent(&event)){
    switch(event.type){
      //si presionas la x en la ventana el juego se acaba
      case SDL_QUIT:
          mIsRunning = false;
          break;
    }
  }

  // El juego se acaba cuando preionas esc
  const Uint8* state = SDL_GetKeyboardState(NULL);
  if(state[SDL_SCANCODE_ESCAPE])
      mIsRunning = false;

}

//Actualiza el juego
void Game::UpdateGame(){

  while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));
  float deltaTime = (SDL_GetTicks()- mTicksCount)/1000.0f;

  if(deltaTime >0.05f)
      deltaTime = 0.05f;

  mTicksCount = SDL_GetTicks();

  mUpdatingActors = true;
  //va por todos los actores y los actualiza
  for(auto actor: mActors)
      actor->Update(deltaTime);
  mUpdatingActors = false;

  //los actores que no se puedieron agregar a mActors lo haran ahora
  for(auto pending: mPendingActors)
      mActors.emplace_back(pending);
  //vaciamos el vector mPendingActors ya que pasamos atodos sus actores al mActors
  mPendingActors.clear();
  //vector temporal para almacenar a los muertos
  std::vector<class Actor*> deadActors;
  //agregar actores muertos
  for(auto actor: mActors){
    if(actor->GetState() == Actor::EDead)
        deadActors.emplace_back(actor);
  }
  //eliminarlos de la faz de la tierra
  for(auto actor: deadActors)
      delete actor;
}

//Generar outputs
void Game::GenerateOutput(){
  // no me queda muy claro para ue es esto
  SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);
  //Limpias el Render
  SDL_RenderClear(mRenderer);
  // Dibujas todos los sprites necesarios
  for(auto sprite : mSprites)
      sprite->Draw(mRenderer);
  //????
  SDL_RenderPresent(mRenderer);
}

//Aqui cargas todos los datos del juego (lo hardcodeamos de momento)
void Game::LoadData(){

}

// Aqui nod quitamos la data del juego
void Game::UnloadData(){
  //vaciamos mActors
  while(!mActors.empty())
      delete mActors.back();
  //vaciamos mTextures
  for(auto i: mTextures)
      SDL_DestroyTexture(i.second);
  mTextures.clear();
}

//Obtenemos texturas
SDL_Texture* Game::GetTexture(const std::string& fileName){
  //creamos el puntero a una textura y lo inicializamos a nullptr
  SDL_Texture* tex = nullptr;
  //buscamos por el nombre del archivo si existe la textura
  auto iter = mTextures.find(fileName);
  if(iter != mTextures.end())
      //si ya tenemos la textura solo la devolvemos
      tex = iter->second;
  else{
      //si no la tenemos pos la creamos
      //creamos la superficie
    SDL_Surface* surf = IMG_Load(fileName.c_str());
    if(!surf){
        //si no se pudo crear la superficie pos f
      SDL_Log("Failed to load texture file %s", fileName.c_str());
      return nullptr;
    }
    //creamos la textura a partir de la superficie
    tex = SDL_CreateTextureFromSurface(mRenderer, surf);
    //liberamos la superficie
    SDL_FreeSurface(surf);
    if(!tex){
        //si la superficie no se convierte a textura pos f
      SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
      return nullptr;
    }
    //agregamos nuestra nueva textura a nuestro mapa
    mTextures.emplace(fileName.c_str(), tex);
  }
  // y devolvemos
  return tex;
}

void Game::AddSprite(SpriteComponent* sprite){
  //ordenamos este vector por orden de dibujo para dibujar el fondo primero y esa wea
  int myDrawOrder = sprite->GetDrawOrder();
  // nos ponemos al inicio del vector
  auto iter = mSprites.begin();
  // pasamos por todo el vector hasta encontrar su posicion adecuada (el vector se ordena de manera ascendente btw)
  for(;iter != mSprites.end(); ++iter)
      if(myDrawOrder < (*iter)->GetDrawOrder())
          break;
  // agregamos
  mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite){
  //encontramos la posicion del sprite y lo eliminamos
  auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
  mSprites.erase(iter);
}
