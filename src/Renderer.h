#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glut.h> // Para void* font

// --- Protótipos de Funções ---
// Funções responsáveis por desenhar todos os elementos visuais do jogo.

void display();              // Função principal de desenho, chamada pelo GLUT.
void reshape(int w, int h);    // Chamada quando a janela é redimensionada para ajustar a projeção.
void drawText(float x, float y, float r, float g, float b, void* font, const char *string); // Desenha uma string de texto na tela.
void drawMenu();             // Desenha a tela do menu principal e de controles.
void drawGame();             // Desenha a cena principal do jogo (jogador, obstáculos, etc.).
void drawPause();            // Desenha a tela de pausa sobre a cena do jogo.
void drawGameOver();         // Desenha a tela de "Game Over" sobre a cena do jogo.
void drawBackground();       // Desenha o fundo do jogo.
void drawObstacles();        // Desenha todos os obstáculos ativos.
void drawTrashBins();        // Desenha todas as lixeiras ativas.
void drawThrownTrashItems(); // Desenha todos os itens de lixo arremessados ativos.

#endif //RENDERER_H