#include "Globals.h"

// Variável que controla o estado geral do jogo (MENU, PLAYING, etc.). Começa no MENU.
GameState gameState = MENU;
// A struct que contém todas as informações do jogador (posição, estado de pulo, etc.).
Player_s player;
// Arrays que funcionam como "pools" de objetos para todos os elementos do jogo.
Obstacle_s obstacles[5];
TrashBin_s trashBins[TRASH_TYPE_COUNT];
TrashItem_s thrownTrashItems[10];

// --- Variáveis de Estado da Partida ---
int score = 0;      // Pontuação atual do jogador.
int lives = 3;      // Vidas restantes do jogador.
int nextLifeScore = 2000; // Pontuação necessária para ganhar a próxima vida extra.
float currentObstacleSpeed = OBSTACLE_SPEED_BASE; // Velocidade atual dos obstáculos, que aumenta com o tempo.
float gameTime = 0.0f;    // Contador de tempo de jogo, usado para aumentar a dificuldade.

// --- Variáveis de Controle de Animação ---
float backgroundOffset1 = 0.0f; // Deslocamento da primeira imagem de fundo para o efeito de parallax.
float backgroundOffset2 = WINDOW_WIDTH; // Deslocamento da segunda imagem de fundo.
float playerAnimationTimer = 0.0f; // Timer para controlar a troca de frames da animação de corrida.
int currentPlayerRunFrame = 0;     // O frame atual da animação de corrida (0 ou 1).

// --- Variáveis para as Texturas ---
// São variáveis do tipo GLuint que irão armazenar os IDs únicos de cada textura do OpenGL.
// Elas são inicializadas com 0 e recebem seus valores reais na função loadAllTextures().
GLuint playerRunTexture1, playerRunTexture2, playerJumpTexture, playerDuckTexture;
GLuint backgroundTexture;
GLuint obstacleTextures[OBSTACLE_TYPE_COUNT];
GLuint trashBinTextures[TRASH_TYPE_COUNT];
GLuint trashItemTextures[TRASH_TYPE_COUNT];

// Array de strings usado para exibir o nome do lixo selecionado no HUD (Heads-Up Display).
const char* TRASH_TYPE_NAMES[TRASH_TYPE_COUNT] = {"Papel", "Vidro", "Plastico", "Metal", "Organico"};

// --- DEFINIÇÕES DO MENU E JANELA (DEFINIDAS APENAS UMA VEZ) ---
// Flag que controla se a tela de controles está visível ou não.
bool showControls = false;
// Structs que definem a posição e o tamanho de cada botão na interface.
Button_s startButton = {0, 0, 200, 50};
Button_s controlsButton = {0, 0, 200, 50};
Button_s backButton = {0, 0, 150, 40};
Button_s backToMenuButton = {0, 0, 200, 50};
Button_s exitButton = {0, 0, 200, 50};

// Variáveis que armazenam as dimensões atuais da janela, a posição da câmera e o fator de escala.
int g_currentWindowWidth = WINDOW_WIDTH;
int g_currentWindowHeight = WINDOW_HEIGHT;
float cameraX = 0.0f;
float cameraY = 0.0f;
float g_dynamicScale = 1.0f;