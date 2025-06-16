#ifndef CONFIG_H
#define CONFIG_H

// --- Constantes do Jogo ---
// Centralizar essas constantes aqui facilita o ajuste fino do gameplay.
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define PLAYER_HEIGHT 100 
#define PLAYER_WIDTH 60  
#define GROUND_LEVEL 100 // A coordenada Y onde o "chão" do jogo se encontra.
#define OBSTACLE_SPEED_BASE 3.0f // Velocidade inicial dos obstáculos.
#define JUMP_INITIAL_VELOCITY 22.0f // A força inicial do pulo do jogador.
#define GRAVITY 0.75f // A força que puxa o jogador para baixo durante um pulo.
#define TRASH_ITEM_SPEED_X 7.0f // Velocidade horizontal do lixo arremessado.
#define TRASH_ITEM_INITIAL_SPEED_Y 4.0f // Velocidade vertical inicial do lixo arremessado.
#define PLAYER_ANIMATION_FRAME_DURATION 0.12f // Duração de cada frame da animação de corrida.
#define MIN_OBSTACLE_SPACING 450 
#define RAND_OBSTACLE_SPACING 300 
#define MIN_TRASHBIN_SPACING 400  
#define RAND_TRASHBIN_SPACING 250 
// --- Estados do Jogo ---
// Usar um 'enum' torna o código mais legível do que usar números (ex: if (gameState == 1)).
enum GameState { 
    MENU,       // Estado para quando o jogo está no menu principal.
    PLAYING,    // Estado para quando o jogo está em andamento.
    PAUSED,     // Estado para quando o jogo está pausado.
    GAME_OVER   // Estado para quando o jogador perdeu.
};

// --- Tipos de Lixo e Obstáculos ---
// Enums para identificar os tipos de objetos do jogo de forma clara.
enum TrashType { 
    PAPER,      // Identificador para lixo de papel.
    GLASS,      // Identificador para lixo de vidro.
    PLASTIC,    // Identificador para lixo de plástico.
    METAL,      // Identificador para lixo de metal.
    ORGANIC,    // Identificador para lixo orgânico.
    TRASH_TYPE_COUNT // Um truque para saber quantos tipos de lixo existem.
};
enum ObstacleType { 
    HOLE,           // Identificador para obstáculo do tipo buraco.
    DOG,            // Identificador para obstáculo do tipo cachorro.
    BIKE,           // Identificador para obstáculo do tipo bicicleta.
    MONSTER,        // Identificador para obstáculo do tipo monstro terrestre.
    FLYING_MONSTER, // Identificador para obstáculo do tipo monstro voador.
    OBSTACLE_TYPE_COUNT // Truque para contar os tipos de obstáculos.
};

// Declaração para o array de nomes dos tipos de lixo (definido em Globals.cpp).
extern const char* TRASH_TYPE_NAMES[TRASH_TYPE_COUNT]; 

#endif // CONFIG_H