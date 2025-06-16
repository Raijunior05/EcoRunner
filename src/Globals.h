#ifndef GLOBALS_H
#define GLOBALS_H

#include <GL/glut.h>
#include "Config.h"

// --- Estruturas de Dados (Structs) ---
// Agrupam múltiplas variáveis em um único tipo de dado.

// Define a estrutura de dados para o jogador.
typedef struct {
    float x, y;
    float width, height;
    int jumping;
    float jumpVelocity;
    int ducking; 
    TrashType selectedTrash;
} Player_s;

// Define a estrutura de dados para os obstáculos.
typedef struct {
    float x, y;
    float width, height;
    ObstacleType type;
    int active;
} Obstacle_s;

// Define a estrutura de dados para as lixeiras.
typedef struct {
    float x, y;
    float width, height;
    TrashType type;
    int active;
} TrashBin_s;

// Define a estrutura de dados para o lixo arremessado.
typedef struct {
    float x, y;
    float width, height;
    TrashType type;
    int active;
    float velocityX;
    float velocityY;
} TrashItem_s;

// Define a estrutura de dados para os botões do menu.
typedef struct {
    float x, y, width, height;
} Button_s;


// --- DECLARAÇÃO DE VARIÁVEIS GLOBAIS ---
// 'extern' "anuncia" uma variável que será definida em outro arquivo (Globals.cpp).

extern GameState gameState;              // Armazena o estado atual do jogo (MENU, PLAYING, etc.).
extern Player_s player;                  // A struct com todos os dados do jogador.
extern Obstacle_s obstacles[5];          // Array para guardar os obstáculos do jogo.
extern TrashBin_s trashBins[TRASH_TYPE_COUNT]; // Array para guardar as lixeiras.
extern TrashItem_s thrownTrashItems[10]; // Array para guardar os itens de lixo arremessados.
extern int score;                        // Pontuação atual do jogador.
extern int lives;                        // Vidas restantes do jogador.
extern int nextLifeScore;                // Pontuação necessária para ganhar a próxima vida.
extern float currentObstacleSpeed;       // Velocidade atual dos obstáculos, que aumenta com o tempo.
extern float gameTime;                   // Contador de tempo de jogo.
extern float backgroundOffset1;          // Deslocamento do primeiro plano de fundo para efeito parallax.
extern float backgroundOffset2;          // Deslocamento do segundo plano de fundo para efeito parallax.
extern float playerAnimationTimer;       // Timer para controlar a animação de corrida do jogador.
extern int currentPlayerRunFrame;        // Frame atual da animação de corrida (0 ou 1).

// IDs das texturas do OpenGL.
extern GLuint playerRunTexture1, playerRunTexture2, playerJumpTexture, playerDuckTexture;
extern GLuint backgroundTexture;
extern GLuint obstacleTextures[OBSTACLE_TYPE_COUNT];
extern GLuint trashBinTextures[TRASH_TYPE_COUNT];
extern GLuint trashItemTextures[TRASH_TYPE_COUNT];

// Variáveis do Menu e da Janela.
extern bool showControls;               // Flag para mostrar ou não a tela de controles.
extern Button_s startButton, controlsButton, backButton, backToMenuButton, exitButton;
extern int g_currentWindowWidth, g_currentWindowHeight; // Dimensões atuais da janela.
extern float cameraX, cameraY;          // Posição da câmera do jogo.
extern float g_dynamicScale;            // Fator de escala para redimensionamento da janela.

#endif // GLOBALS_H