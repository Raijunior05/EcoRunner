#ifndef GAMELOGIC_H
#define GAMELOGIC_H

// --- Protótipos de Funções ---
// Funções que controlam as regras e o estado do jogo.

void initGame();             // Inicializa ou reinicia todo o estado de uma partida.
void updateGame(int value);    // Executa a lógica de atualização do jogo (movimento, colisões, etc.).
void checkAllCollisions();     // Verifica todas as possíveis colisões entre os objetos do jogo.
void spawnThrownTrashItem();   // Cria uma nova instância de lixo arremessado pelo jogador.
void cycleSelectedTrash();     // Alterna o tipo de lixo que o jogador está segurando.

#endif // GAMELOGIC_H