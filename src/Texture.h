#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/glut.h> // Para GLuint

// --- Protótipos de Funções ---
// Funções para gerenciar o carregamento e desenho de texturas (imagens).

GLuint loadTextureFromFile(const char* filename); // Carrega uma única textura de um arquivo e retorna seu ID.
void loadAllTextures();                           // Carrega todas as texturas necessárias para o jogo.
void drawQuadWithTexture(GLuint texture, float x, float y, float width, float height); // Desenha um retângulo na tela com uma textura aplicada.
void cleanupTextures();                           // Libera a memória da GPU alocada para as texturas.

#endif // TEXTURE_H