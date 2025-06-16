#include "Player.h"
#include <stdio.h>   // Para a função printf (se for necessário para depuração).
#include <GL/glut.h> // Para tipos e funções do OpenGL, se necessário.
#include "Texture.h" // Para a função drawQuadWithTexture, que desenha o jogador.

/**
 * Inicializa ou reseta as variáveis do jogador para o estado padrão de início de jogo.
 */
void initPlayer() {
    // Define a posição inicial do jogador no cenário.
    player.x = 100;
    player.y = GROUND_LEVEL;
    // Define as dimensões da caixa de colisão (hitbox) do jogador.
    player.width = PLAYER_WIDTH;
    player.height = PLAYER_HEIGHT;
    // Flags que controlam o estado de pulo e agachamento. Começam em 0 (falso).
    player.jumping = 0;
    player.ducking = 0;
    // A velocidade vertical do jogador, começa em 0.
    player.jumpVelocity = 0;
    // O tipo de lixo que o jogador começa segurando.
    player.selectedTrash = PLASTIC; 
    // Variáveis para controlar a animação de corrida, são resetadas aqui.
    playerAnimationTimer = 0.0f;
    currentPlayerRunFrame = 0;
}

/**
 * Atualiza a lógica de movimento (pulo) e de animação do jogador a cada quadro.
 * Esta função é chamada repetidamente pela função updateGame.
 */
void updatePlayerAnimation() {
    // --- Lógica de Pulo ---
    // Este bloco só é executado se o jogador estiver no estado "pulando".
    if (player.jumping) {
        // Atualiza a posição vertical do jogador com base na sua velocidade de pulo.
        player.y += player.jumpVelocity;
        // Aplica a força da gravidade, diminuindo a velocidade de subida (ou aumentando a de descida).
        player.jumpVelocity -= GRAVITY;
        
        // Verifica se o jogador tocou ou atravessou o chão.
        if (player.y <= GROUND_LEVEL) {
            // Se tocou o chão, "trava" o jogador na posição correta para evitar que afunde.
            player.y = GROUND_LEVEL;
            // Desativa o estado de pulo e zera a velocidade para encerrar o movimento.
            player.jumping = 0;
            player.jumpVelocity = 0;
        }
    } 
    // --- Lógica de Animação de Corrida ---
    // Este bloco só é executado se o jogador NÃO estiver pulando E NÃO estiver agachado.
    else if (!player.ducking) { 
        // Acumula o tempo decorrido desde o último quadro (aproximadamente 1/60 de segundo).
        playerAnimationTimer += 0.016f; 
        // Verifica se já se passou tempo suficiente para trocar o frame da animação.
        if (playerAnimationTimer >= PLAYER_ANIMATION_FRAME_DURATION) {
            // Se sim, reseta o timer...
            playerAnimationTimer = 0.0f;
            // ...e alterna o frame da animação de corrida (de 0 para 1, e de 1 para 0).
            currentPlayerRunFrame = (currentPlayerRunFrame + 1) % 2; 
        }
    }
}

/**
 * Desenha o jogador na tela, escolhendo a textura correta com base no seu estado atual.
 */
void drawPlayer() {
    // Define uma textura padrão para o caso de nenhuma outra ser selecionada.
    GLuint texToUse = playerRunTexture1; 
    
    // --- Lógica de Seleção de Textura (Máquina de Estados Visual) ---
    if (player.jumping) {
        // Se estiver pulando, usa a textura de pulo.
        texToUse = playerJumpTexture;
    } else if (player.ducking) {
        // Se estiver agachado, usa a textura de agachar.
        texToUse = playerDuckTexture;
    } else { 
        // Caso contrário (está correndo), alterna entre as duas texturas de corrida.
        if (currentPlayerRunFrame == 0) {
            texToUse = playerRunTexture1;
        } else {
            texToUse = playerRunTexture2;
        }
    }
    // Verificação de segurança: se a textura selecionada for inválida, usa a padrão.
    if ((!texToUse || texToUse == 0) && playerRunTexture1) {
        texToUse = playerRunTexture1; 
    }
    
    // Chama a função de renderização para desenhar o jogador.
    // Usa um operador ternário para ajustar a altura do jogador e da sua hitbox:
    // Se (player.ducking for verdadeiro), a altura é reduzida; senão, usa a altura normal.
    drawQuadWithTexture(texToUse, player.x, player.y, player.width, (player.ducking ? player.height / 1.8f : player.height));
}