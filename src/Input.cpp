#include "Input.h"
#include "Globals.h"   // Para acessar gameState, player e os botões do menu.
#include "Config.h"    // Para constantes como JUMP_INITIAL_VELOCITY.
#include "GameLogic.h" // Para chamar funções de lógica de jogo como initGame().
#include <GL/glut.h>   // Para constantes do GLUT como GLUT_KEY_UP e funções como exit().
#include <stdio.h>     // Para a função printf (usada para depuração).
#include <stdlib.h>    // Para a função exit().


/**
 * Função auxiliar que verifica se um clique do mouse está dentro dos limites de um botão.
 * mouseX Coordenada X do mouse.
 * mouseY Coordenada Y do mouse.
 * button A struct do botão com sua posição e dimensões.
 * retorna 'true' se o clique foi dentro do botão, 'false' caso contrário.
 */
bool isClickInside(int mouseX, int mouseY, Button_s button) {
    // Retorna verdadeiro apenas se o clique estiver dentro das quatro bordas do botão.
    return (mouseX >= button.x && mouseX <= button.x + button.width &&
            mouseY >= button.y && mouseY <= button.y + button.height);
}


/**
 * Callback do GLUT para teclas NORMAIS pressionadas (letras, números, espaço, etc.).
 * key O código ASCII da tecla pressionada.
 * x A coordenada X do mouse no momento do clique.
 * y A coordenada Y do mouse no momento do clique.
 */
void keyboard(unsigned char key, int x, int y) {
    // Um switch para lidar com diferentes teclas.
    switch (key) {
        case 27: // Tecla ESC (código ASCII 27).
            exit(0); // Fecha o programa imediatamente.
            break;
        case ' ': // Barra de espaço.
            // A ação da barra de espaço só funciona na tela de Game Over.
            if (gameState == GAME_OVER) {
                initGame(); // Reinicia o jogo.
                gameState = PLAYING; // Define o estado como jogando.
            }
            break;
        case 'p':
        case 'P': // Tecla P (maiúscula ou minúscula).
            // Alterna o estado do jogo entre JOGANDO e PAUSADO.
            if (gameState == PLAYING) {
                gameState = PAUSED;
                printf("Jogo Pausado.\n");
            } else if (gameState == PAUSED) {
                gameState = PLAYING;
                printf("Jogo Retomado.\n");
            }
            break;
        
        // PULO com a tecla W.
        case 'w':
        case 'W':
            // O jogador só pode pular se o jogo estiver rodando, se não estiver já pulando e não estiver agachado.
            if (gameState == PLAYING && !player.jumping && !player.ducking) {
                player.jumping = 1; // Ativa a flag de pulo.
                player.jumpVelocity = JUMP_INITIAL_VELOCITY; // Dá ao jogador o impulso inicial do pulo.
                printf("Tecla W - Pulo iniciado.\n");
            }
            break;
        // AGACHAR com a tecla S (pressionar e segurar).
        case 's':
        case 'S':
            // O jogador só pode agachar se estiver jogando e não estiver no meio de um pulo.
            if (gameState == PLAYING && !player.jumping) {
                player.ducking = 1; // Ativa a flag de agachado.
                printf("Tecla S - Agachado.\n");
            }
            break;

        // Seleção de lixo com as teclas numéricas (atalho opcional).
        case '1': if(gameState == PLAYING) player.selectedTrash = PAPER;   printf("Lixo selecionado: Papel\n"); break;
        case '2': if(gameState == PLAYING) player.selectedTrash = GLASS;   printf("Lixo selecionado: Vidro\n"); break;
        case '3': if(gameState == PLAYING) player.selectedTrash = PLASTIC; printf("Lixo selecionado: Plastico\n"); break;
        case '4': if(gameState == PLAYING) player.selectedTrash = METAL;   printf("Lixo selecionado: Metal\n"); break;
        case '5': if(gameState == PLAYING) player.selectedTrash = ORGANIC; printf("Lixo selecionado: Organico\n"); break;
    }
}

/**
 * Callback do GLUT para quando uma tecla NORMAL é solta.
 */
void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
        case 's':
        case 'S': // Se a tecla 'S' for solta...
            if (gameState == PLAYING) {
                player.ducking = 0; // O jogador para de agachar.
                printf("Tecla S solta - Levantou.\n");
            }
            break;
    }
}

/**
 * Callback do GLUT para teclas ESPECIAIS pressionadas (Setas, F1, etc.).
 */
void specialKeyboard(int key, int x, int y) {
    // Ações das setas só funcionam durante o jogo.
    if (gameState == PLAYING) {
        switch (key) {
            case GLUT_KEY_UP: // Seta para Cima.
                // Mesma lógica da tecla W.
                if (!player.jumping && !player.ducking) {
                    player.jumping = 1;
                    player.jumpVelocity = JUMP_INITIAL_VELOCITY;
                     printf("Seta CIMA - Pulo iniciado.\n");
                }
                break;
            case GLUT_KEY_DOWN: // Seta para Baixo.
                // Mesma lógica da tecla S.
                 if (!player.jumping) {
                    player.ducking = 1;
                    printf("Seta BAIXO - Agachado.\n");
                }
                break;
        }
    }
}

/**
 * Callback do GLUT para quando uma tecla ESPECIAL é solta.
 */
void specialKeyboardUp(int key, int x, int y) {
    if (gameState == PLAYING) {
        switch (key) {
            case GLUT_KEY_DOWN: // Soltou a seta para baixo.
                player.ducking = 0; // O jogador para de agachar.
                printf("Seta BAIXO solta - Levantou.\n");
                break;
        }
    }
}

/**
 * Callback do GLUT para eventos de clique do mouse.
 */
void mouse(int button, int state, int x, int y) {
    // A coordenada Y do mouse em GLUT é invertida (0 é no topo), então a corrigimos
    // para corresponder ao sistema de coordenadas do OpenGL (0 é na base).
    int inverted_y = g_currentWindowHeight - y;

    // Ação só ocorre quando o botão é pressionado (e não quando é solto).
    if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON) {
        
        // --- Lógica de Clique para o Menu ---
        if (gameState == MENU) {
            if (!showControls) { // Se estiver na tela principal do menu...
                if (isClickInside(x, inverted_y, startButton)) {
                    printf("Botao Iniciar Jogo clicado.\n");
                    initGame(); // Inicia o jogo.
                }
                else if (isClickInside(x, inverted_y, controlsButton)) {
                    printf("Botao Controles clicado.\n");
                    showControls = true; // Mostra a tela de controles.
                }
                else if (isClickInside(x, inverted_y, exitButton)) {
                    printf("Botao Sair clicado. Fechando o jogo.\n");
                    exit(0); // Fecha o programa.
                }
            } else { // Se estiver na tela de controles...
                if (isClickInside(x, inverted_y, backButton)) {
                    printf("Botao Voltar clicado.\n");
                    showControls = false; // Volta para o menu principal.
                }
            }
        }
        // --- Lógica de Clique durante o Jogo ---
        else if (gameState == PLAYING) {
            spawnThrownTrashItem(); // Arremessa o lixo.
        }
        // --- Lógica de Clique na Tela de Pausa ---
        else if (gameState == PAUSED) {
            if (isClickInside(x, inverted_y, backToMenuButton)) {
                printf("Botao 'Voltar ao Menu' clicado.\n");
                gameState = MENU; // Muda o estado do jogo para MENU.
                showControls = false; // Garante que o menu principal seja mostrado da próxima vez.
            }
        }
    }
    // Ação do botão direito do mouse durante o jogo.
    else if (gameState == PLAYING && state == GLUT_DOWN && button == GLUT_RIGHT_BUTTON) {
         cycleSelectedTrash(); // Troca o tipo de lixo selecionado.
    }
    
    // Pede para o GLUT redesenhar a tela, para que as mudanças no menu (se houver) apareçam.
    glutPostRedisplay();
}