#include"Game.h"

int main(int argc, char** argv){
  // Creamos la clase juego
  Game game;
  // Inicializamos
  bool sucess = game.Initialize();
  // main loop
  if(sucess)
      game.RunLoop();
  //Cuando acabe el main loop cerramos el juego
  game.Shutdown();
  return 0;
}
