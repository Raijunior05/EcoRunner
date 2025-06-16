#include "Renderer.h"
#include "Globals.h"
#include "Config.h"
#include "Texture.h"
#include "Player.h"
#include <GL/glut.h>
#include <stdio.h>
#include <string.h>

// Protótipos para funções de desenho que são usadas apenas dentro deste arquivo.
void drawGame();
void drawMenu();
void drawPause();
void drawGameOver();
void drawBackground();
void drawObstacles();
void drawTrashBins();
void drawThrownTrashItems();
void drawButton(Button_s button, const char* text);


/**
 *  Função principal de desenho, registrada como callback do GLUT.
 * Atua como um "roteador" que decide qual cena desenhar com base no estado do jogo.
 */
void display() {
    // Limpa o buffer de cores com a cor de fundo definida em main.cpp.
    glClear(GL_COLOR_BUFFER_BIT);

    // Usa um switch para chamar a função de desenho apropriada para o estado atual do jogo.
    switch (gameState) {
        case MENU:      drawMenu(); break;
        case PLAYING:   drawGame(); break;
        case PAUSED:    drawPause(); break;
        case GAME_OVER: drawGameOver(); break;
    }
    // Troca o buffer de fundo (onde desenhamos) pelo buffer da frente (o que é exibido).
    // Essencial para animações suaves, evitando o efeito de "piscar" (flickering).
    glutSwapBuffers();
}

/**
 * Callback do GLUT chamado quando a janela é criada ou redimensionada.
 * w Nova largura da janela.
 * h Nova altura da janela.
 */
void reshape(int w, int h) {
    if (h == 0) h = 1; // Evita divisão por zero se a janela for minimizada.

    // Atualiza as variáveis globais com as novas dimensões da janela.
    g_currentWindowWidth = w;
    g_currentWindowHeight = h;

    // --- CÁLCULO DA ESCALA DINÂMICA ---
    // Calcula um fator de escala para que os elementos do jogo se ajustem à altura da janela.
    g_dynamicScale = (float)g_currentWindowHeight / (float)WINDOW_HEIGHT;
    printf("Nova escala dinamica calculada: %f\n", g_dynamicScale);
    
    // Define a área da janela que o OpenGL usará para desenhar.
    glViewport(0, 0, w, h);

    // Configura o sistema de coordenadas 2D.
    glMatrixMode(GL_PROJECTION); // Muda para a matriz de projeção.
    glLoadIdentity(); // Reseta a matriz.
    // Define uma projeção ortográfica 2D, onde as coordenadas correspondem aos pixels da janela.
    gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);

    // Recalcula a posição dos botões do menu para que permaneçam centralizados.
    startButton.x = (float)w / 2.0f - startButton.width / 2.0f;
    startButton.y = (float)h / 2.0f + 20.0f;
    controlsButton.x = (float)w / 2.0f - controlsButton.width / 2.0f;
    controlsButton.y = (float)h / 2.0f - 50.0f;
    exitButton.x = (float)w / 2.0f - exitButton.width / 2.0f;
    exitButton.y = (float)h / 2.0f - 120.0f;
    backToMenuButton.x = (float)w / 2.0f - backToMenuButton.width / 2.0f;
    backToMenuButton.y = (float)h / 2.0f - 80.0f;
    
    // Retorna para a matriz de modelo/visão, que é usada para transformações de objetos.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
 * Função auxiliar para desenhar texto na tela.
 */
void drawText(float x, float y, float r, float g, float b, void* font, const char *string) {
    // Define a cor do texto.
    glColor3f(r, g, b);
    // Define a posição inicial do texto na tela.
    glRasterPos2f(x, y);
    // Itera sobre a string e desenha cada caractere.
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(font, *c);
    }
}

/**
 * Função auxiliar para desenhar um botão com texto centralizado.
 */
void drawButton(Button_s button, const char* text) {
    // Desenha o fundo do botão.
    glDisable(GL_TEXTURE_2D);
    glColor3f(0.6f, 0.6f, 0.8f);
    glBegin(GL_QUADS);
        glVertex2f(button.x, button.y);
        glVertex2f(button.x + button.width, button.y);
        glVertex2f(button.x + button.width, button.y + button.height);
        glVertex2f(button.x, button.y + button.height);
    glEnd();

    // Desenha a borda do botão.
    glColor3f(0.2f, 0.2f, 0.3f);
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(button.x, button.y);
        glVertex2f(button.x + button.width, button.y);
        glVertex2f(button.x + button.width, button.y + button.height);
        glVertex2f(button.x, button.y + button.height);
    glEnd();

    // Calcula a posição do texto para centralizá-lo dentro do botão.
    int textWidth = 0;
    void* font = GLUT_BITMAP_HELVETICA_18;
    for (const char* c = text; *c != '\0'; c++) {
        textWidth += glutBitmapWidth(font, *c);
    }
    float textX = button.x + (button.width - textWidth) / 2.0f;
    float textY = button.y + (button.height / 2.0f) - 7;
    // Desenha o texto.
    drawText(textX, textY, 1.0f, 1.0f, 1.0f, font, text);
}


/**
 * Desenha a tela de Menu ou a tela de Controles.
 */
void drawMenu() {
    // Define uma cor de fundo sólida para o menu.
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    
    // Verifica se deve mostrar a tela de controles ou o menu principal.
    if (!showControls) {
        // --- TELA INICIAL DO MENU ---
        const char* title = "Eco Runner: Missao Reciclar";
        void* titleFont = GLUT_BITMAP_TIMES_ROMAN_24;
        int titleWidth = 0;
        for (const char* c = title; *c != '\0'; c++) {
            titleWidth += glutBitmapWidth(titleFont, *c);
        }
        float titleX = (g_currentWindowWidth - titleWidth) / 2.0f;
        drawText(titleX, g_currentWindowHeight - 120, 0.1f, 0.2f, 0.4f, titleFont, title);

        // Usa a nova função para desenhar os botões definidos em Globals.cpp.
        drawButton(startButton, "Iniciar Jogo");
        drawButton(controlsButton, "Controles");
        drawButton(exitButton, "Sair");

    } else {
        // --- TELA DE CONTROLES (ATUALIZADA) ---
        drawText(g_currentWindowWidth / 2.0f - 60, g_currentWindowHeight - 100, 0.1f, 0.1f, 0.2f, GLUT_BITMAP_TIMES_ROMAN_24, "Controles");

        int y_pos = g_currentWindowHeight - 180;
        float x_pos = g_currentWindowWidth * 0.2f; // Posição X relativa.
        void* font = GLUT_BITMAP_HELVETICA_18;
        
        drawText(x_pos, y_pos,      0.0f, 0.0f, 0.0f, font, "W / Seta para Cima: Pular");
        drawText(x_pos, y_pos - 30, 0.0f, 0.0f, 0.0f, font, "S / Seta para Baixo (Segurar): Agachar");
        drawText(x_pos, y_pos - 60, 0.0f, 0.0f, 0.0f, font, "Clique Esquerdo do Mouse: Arremessar Lixo");
        drawText(x_pos, y_pos - 90, 0.0f, 0.0f, 0.0f, font, "Clique Direito do Mouse: Trocar tipo de lixo");
        
        // --- NOVA LINHA ADICIONADA AQUI ---
        glColor3f(0.8f, 0.1f, 0.1f); // Cor vermelha para destacar.
        drawText(x_pos, y_pos - 120, 1.0f, 0.1f, 0.1f, font, "Lixo de METAL derrota os monstros!");
        
        // Linhas seguintes ajustadas.
        drawText(x_pos, y_pos - 150, 0.0f, 0.0f, 0.0f, font, "P: Pausar / Retomar Jogo");
        drawText(x_pos, y_pos - 180, 0.0f, 0.0f, 0.0f, font, "ESC: Sair do Jogo");

        // Botão para retornar ao menu principal.
        drawButton(backButton, "Voltar");
    }
}

/**
 *  Desenha a cena principal do jogo quando o estado é PLAYING.
 */
void drawGame() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Salva a matriz de transformação atual. Isso é como criar um "checkpoint".
    glPushMatrix();

        // 1. Aplica a Escala Dinâmica. Afeta tudo o que for desenhado daqui para frente.
        glScalef(g_dynamicScale, g_dynamicScale, 1.0f);

        // 2. Move a "câmera". Na verdade, estamos movendo o "mundo" na direção oposta à da câmera.
        // Isso também é afetado pela escala, o que é o comportamento correto.
        glTranslatef(-cameraX, -cameraY, 0.0f);

        // 3. Desenha todos os elementos do MUNDO DO JOGO.
        // Estes elementos serão afetados pela câmera e pela escala.
        glEnable(GL_TEXTURE_2D);
            drawBackground();
            drawTrashBins();
            drawObstacles();
            drawThrownTrashItems();
            drawPlayer();
        glDisable(GL_TEXTURE_2D);

    // Restaura a matriz de transformação ao seu estado anterior (antes do PushMatrix).
    // Isso "remove" a escala e a translação da câmera para os desenhos seguintes.
    glPopMatrix();


    // 4. Desenha o HUD (Heads-Up Display: placar, vidas, etc.).
    // Como está fora do bloco Push/Pop Matrix, o HUD não é afetado pela câmera nem pela escala.
    // Isso garante que ele fique fixo na tela.
    char hudText[100];
    sprintf(hudText, "Pontos: %d", score);
    drawText(10, g_currentWindowHeight - 25, 0.0f, 0.0f, 0.0f, GLUT_BITMAP_HELVETICA_18, hudText);
    sprintf(hudText, "Vidas: %d", lives);
    drawText(10, g_currentWindowHeight - 50, 0.0f, 0.0f, 0.0f, GLUT_BITMAP_HELVETICA_18, hudText);
    if (player.selectedTrash >= 0 && player.selectedTrash < TRASH_TYPE_COUNT) {
        sprintf(hudText, "Lixo: %s", TRASH_TYPE_NAMES[player.selectedTrash]);
        drawText(g_currentWindowWidth - 200, g_currentWindowHeight - 25, 0.0f, 0.0f, 0.0f, GLUT_BITMAP_HELVETICA_18, hudText);
    }
}

/**
 *  Desenha a tela de Pausa.
 */
void drawPause() {
    // 1. Desenha a cena do jogo congelada no fundo.
    drawGame(); 
    // 2. Desenha um retângulo escuro e semi-transparente sobre toda a tela para escurecê-la.
    glEnable(GL_BLEND); 
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f); // Cor preta com 50% de opacidade.
    glBegin(GL_QUADS); 
        glVertex2f(0,0); 
        glVertex2f(g_currentWindowWidth,0); 
        glVertex2f(g_currentWindowWidth,g_currentWindowHeight); 
        glVertex2f(0,g_currentWindowHeight); 
    glEnd();
    glDisable(GL_BLEND);
    // 3. Desenha o texto e os botões da tela de pausa por cima da camada escura.
    drawText(g_currentWindowWidth/2.0f - 50, g_currentWindowHeight/2.0f + 10, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_TIMES_ROMAN_24, "PAUSADO");
    drawText(g_currentWindowWidth/2.0f - 110, g_currentWindowHeight/2.0f - 20, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18, "Pressione P para continuar");

    // --- DESENHA O NOVO BOTÃO AQUI ---
    drawButton(backToMenuButton, "Voltar ao Menu");
}

/**
 *  Desenha a tela de Game Over.
 */
void drawGameOver() {
    // A lógica é a mesma da tela de pausa: desenhar o jogo por baixo e uma camada por cima.
    drawGame();
    // A camada de Game Over é mais escura.
    glEnable(GL_BLEND); 
    glColor4f(0.1f, 0.1f, 0.1f, 0.85f); // Cor cinza escuro com 85% de opacidade.
    glBegin(GL_QUADS);
        glVertex2f(0,0);
        glVertex2f(g_currentWindowWidth,0);
        glVertex2f(g_currentWindowWidth,g_currentWindowHeight);
        glVertex2f(0,g_currentWindowHeight);
    glEnd();
    glDisable(GL_BLEND);
    // Desenha os textos da tela de Game Over.
    drawText(g_currentWindowWidth/2.0f - 70, g_currentWindowHeight/2.0f + 60, 1.0f, 0.2f, 0.2f, GLUT_BITMAP_TIMES_ROMAN_24, "GAME OVER");
    char finalScoreText[50]; sprintf(finalScoreText, "Pontuacao Final: %d", score);
    drawText(g_currentWindowWidth/2.0f - 80, g_currentWindowHeight/2.0f + 20, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18, finalScoreText);
    drawText(g_currentWindowWidth/2.0f - 130, g_currentWindowHeight/2.0f - 20, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_HELVETICA_18, "Pressione ESPACO para jogar novamente");
    drawText(g_currentWindowWidth/2.0f - 70, g_currentWindowHeight/2.0f - 50, 0.8f, 0.8f, 0.8f, GLUT_BITMAP_HELVETICA_12, "Pressione ESC para sair");
}

/**
 *Desenha o fundo com efeito de parallax scrolling.
 */
void drawBackground() {
    if (!backgroundTexture) return;
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glColor3f(1.0f, 1.0f, 1.0f);

    // Desenha duas cópias da mesma imagem de fundo, uma ao lado da outra.
    // As variáveis backgroundOffset1 e backgroundOffset2 são atualizadas em GameLogic.cpp
    // para mover as imagens para a esquerda, criando a ilusão de um fundo infinito.
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(backgroundOffset1, 0);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(backgroundOffset1 + g_currentWindowWidth, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(backgroundOffset1 + g_currentWindowWidth, g_currentWindowHeight);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(backgroundOffset1, g_currentWindowHeight);
    glEnd();

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(backgroundOffset2, 0);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(backgroundOffset2 + g_currentWindowWidth, 0);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(backgroundOffset2 + g_currentWindowWidth, g_currentWindowHeight);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(backgroundOffset2, g_currentWindowHeight);
    glEnd();
}

/**
 * Itera sobre o array de obstáculos e desenha apenas os que estão ativos.
 */
void drawObstacles() {
    for (int i = 0; i < 5; i++) {
        // A flag 'active' faz parte do sistema de "object pooling".
        // Apenas desenhamos os obstáculos que estão atualmente em uso no jogo e visíveis na tela.
        if (obstacles[i].active && obstacles[i].x + obstacles[i].width > 0 && obstacles[i].x < g_currentWindowWidth) {
            drawQuadWithTexture(obstacleTextures[obstacles[i].type], obstacles[i].x, obstacles[i].y, obstacles[i].width, obstacles[i].height);
        }
    }
}

/**
 * Itera sobre o array de lixeiras e desenha apenas as que estão ativas.
 */
void drawTrashBins() {
    for (int i = 0; i < TRASH_TYPE_COUNT; i++) {
        if (trashBins[i].active && trashBins[i].x + trashBins[i].width > 0 && trashBins[i].x < g_currentWindowWidth) {
            drawQuadWithTexture(trashBinTextures[trashBins[i].type], trashBins[i].x, trashBins[i].y, trashBins[i].width, trashBins[i].height);
        }
    }
}

/**
 *Itera sobre o array de lixo arremessado e desenha apenas os que estão ativos.
 */
void drawThrownTrashItems() {
    for (int i = 0; i < 10; i++) {
        if (thrownTrashItems[i].active && thrownTrashItems[i].x + thrownTrashItems[i].width > 0 && thrownTrashItems[i].x < g_currentWindowWidth) {
            drawQuadWithTexture(trashItemTextures[thrownTrashItems[i].type], thrownTrashItems[i].x, thrownTrashItems[i].y, thrownTrashItems[i].width, thrownTrashItems[i].height);
        }
    }
}