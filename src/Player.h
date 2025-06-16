#ifndef PLAYER_H
#define PLAYER_H

#include "Globals.h" // Inclui Globals.h para ter acesso à struct Player_s e constantes.

// --- Protótipos de Funções ---
// Estes são os "anúncios" das funções que estão definidas em Player.cpp.
// Qualquer outro arquivo que inclua Player.h saberá que essas funções existem.

void initPlayer();          // Para inicializar o estado do jogador.
void updatePlayerAnimation(); // Para atualizar a física e animação do jogador a cada quadro.
void drawPlayer();          // Para desenhar o jogador na tela.

#endif // PLAYER_H