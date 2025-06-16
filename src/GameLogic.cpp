#include "GameLogic.h"
#include "Globals.h"
#include "Config.h"
#include "Player.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>

/**
 * Prepara o jogo para uma nova partida, resetando todos os estados e posições.
 * É chamada no início do jogo ou após um "Game Over".
 */
void initGame() {
    // Reseta a posição e estado do jogador para os valores iniciais.
    initPlayer();

    // --- GERAÇÃO PROCEDURAL INICIAL DE OBJETOS ---
    float lastObjectX = g_currentWindowWidth + 100;

    // Itera para criar e posicionar cada tipo de lixeira.
    for (int i = 0; i < TRASH_TYPE_COUNT; i++) {
        trashBins[i].type = (TrashType)i; // Define o tipo da lixeira (PAPEL, VIDRO, etc.).
        trashBins[i].width = 70;
        trashBins[i].height = 90;
        // Calcula a posição X com um espaçamento mínimo e um valor aleatório adicional.
        trashBins[i].x = lastObjectX + MIN_TRASHBIN_SPACING + (rand() % RAND_TRASHBIN_SPACING);
        trashBins[i].y = GROUND_LEVEL; // Todas as lixeiras ficam no nível do chão.
        trashBins[i].active = 1;       // Ativa a lixeira para que ela apareça no jogo.
        // Atualiza a referência da posição do último objeto.
        lastObjectX = trashBins[i].x + trashBins[i].width;
    }

    lastObjectX += 150; // Adiciona um espaço extra entre o último objeto e o primeiro obstáculo.
    // Itera para criar e posicionar os 5 obstáculos iniciais.
    for (int i = 0; i < 5; i++) { 
        obstacles[i].active = 1; // Ativa o obstáculo.
        // Escolhe um tipo de obstáculo aleatório da lista definida no enum 'ObstacleType'.
        obstacles[i].type = (ObstacleType)(rand() % OBSTACLE_TYPE_COUNT);

        // Usa uma estrutura switch, que é mais limpa e legível do que múltiplos 'if/else if',
        // para configurar as propriedades específicas de cada tipo de obstáculo.
        switch (obstacles[i].type) {
            case HOLE:
                obstacles[i].width = 90; obstacles[i].height = 20; 
                obstacles[i].y = GROUND_LEVEL - 10; // Fica um pouco abaixo do chão.
                break;
            case DOG:
                obstacles[i].width = 70; obstacles[i].height = 50;
                obstacles[i].y = GROUND_LEVEL;
                break;
            case BIKE:
                obstacles[i].width = 80; obstacles[i].height = 70;
                obstacles[i].y = GROUND_LEVEL;
                break;
            case MONSTER:
                obstacles[i].width = 75; 
                obstacles[i].height = 90;
                obstacles[i].y = GROUND_LEVEL;
                break;
            case FLYING_MONSTER:
                obstacles[i].width = 60;
                obstacles[i].height = 60;
                // Posicionado mais alto para que o jogador precise se agachar.
                obstacles[i].y = GROUND_LEVEL + (PLAYER_HEIGHT / 1.5f);
                break;
        }
        // Após definir as propriedades, calcula a posição X do obstáculo.
        obstacles[i].x = lastObjectX + MIN_OBSTACLE_SPACING + (rand() % RAND_OBSTACLE_SPACING);
        lastObjectX = obstacles[i].x + obstacles[i].width;
    }

    // Prepara o "pool" de lixos arremessáveis, desativando todos eles no início.
    for (int i = 0; i < 10; i++) {
        thrownTrashItems[i].active = 0;
    }

    // --- RESET DE VARIÁVEIS DA PARTIDA ---
    // Zera ou redefine todas as variáveis de controle do jogo.
    score = 0;
    lives = 3;
    nextLifeScore = 2000; // Marco de pontuação para ganhar a primeira vida extra.
    currentObstacleSpeed = OBSTACLE_SPEED_BASE;
    gameTime = 0.0f;
    backgroundOffset1 = 0.0f; // Reseta a posição do fundo.
    backgroundOffset2 = g_currentWindowWidth;

    // Finalmente, muda o estado do jogo para "PLAYING". Isso "liga" o motor do jogo.
    gameState = PLAYING;
}

/**
 * O motor do jogo. É chamado repetidamente pelo timer do GLUT (~60 vezes por segundo).
 * O parâmetro 'value' é passado pelo glutTimerFunc (não utilizado aqui).
 */
void updateGame(int value) {
    // A lógica do jogo só é executada se o estado for "PLAYING".
    if (gameState == PLAYING) {
               
        // Acumula o tempo de jogo, usado para aumentar a dificuldade.
        gameTime += 0.016f;
        // Chama a função que atualiza a física do pulo e a animação de corrida do jogador.
        updatePlayerAnimation();

        // Calcula um multiplicador de velocidade para o fundo, criando um efeito de parallax.
        float speedMultiplier = currentObstacleSpeed / OBSTACLE_SPEED_BASE;

        // --- MOVIMENTO E REPOSICIONAMENTO DE OBSTÁCULOS ---
        for (int i = 0; i < 5; i++) {
            if (obstacles[i].active) {
                // Move o obstáculo para a esquerda com base na velocidade atual do jogo.
                obstacles[i].x -= currentObstacleSpeed;
                
                // Se o obstáculo saiu completamente da tela pela esquerda...
                if (obstacles[i].x < -obstacles[i].width - 200) {
                    
                    // ...é hora de reposicioná-lo na frente.
                    // Primeiro, encontramos a posição do obstáculo mais à direita na tela.
                    float rightmost_x = 0.0f;
                    for(int k=0; k<5; ++k) {
                        // Lógica aprimorada: só considera obstáculos que ainda estão visíveis ou
                        // parcialmente visíveis, para evitar bugs de posicionamento.
                        if (obstacles[k].active && obstacles[k].x > -obstacles[k].width) {
                            if (obstacles[k].x + obstacles[k].width > rightmost_x) {
                                rightmost_x = obstacles[k].x + obstacles[k].width;
                            }
                        }
                    }
                    // A base para o novo posicionamento é a borda da tela ou o obstáculo mais à direita, o que for maior.
                    float spawn_base_x = fmax((float)g_currentWindowWidth, rightmost_x);
                    // Calcula a nova posição X com um espaçamento mínimo e um fator aleatório.
                    obstacles[i].x = spawn_base_x + MIN_OBSTACLE_SPACING + (rand() % RAND_OBSTACLE_SPACING);

                    // --- LÓGICA DE RE-SORTEIO ---
                    // Ao reposicionar um obstáculo, seu tipo e propriedades são sorteados novamente.
                    // Isso aumenta a variedade e torna o jogo menos repetitivo.
                    obstacles[i].type = (ObstacleType)(rand() % OBSTACLE_TYPE_COUNT);
                    if (obstacles[i].type == HOLE) {
                        obstacles[i].width = 90; obstacles[i].height = 20;
                        obstacles[i].y = GROUND_LEVEL - 10;
                    } else if (obstacles[i].type == DOG){
                        obstacles[i].width = 70; obstacles[i].height = 50;
                        obstacles[i].y = GROUND_LEVEL;
                    } else if (obstacles[i].type == BIKE) {
                        obstacles[i].width = 80; obstacles[i].height = 100;
                        obstacles[i].y = GROUND_LEVEL;
                    } else if (obstacles[i].type == MONSTER) {
                        obstacles[i].width = 75;
                        obstacles[i].height = 90;
                        obstacles[i].y = GROUND_LEVEL;
                    } else if (obstacles[i].type == FLYING_MONSTER) {
                        obstacles[i].width = 60;
                        obstacles[i].height = 60;
                        obstacles[i].y = GROUND_LEVEL + (PLAYER_HEIGHT / 1.5f);
                    }
                }
            }
        }

        // --- MOVIMENTO E REPOSICIONAMENTO DE LIXEIRAS ---
        for (int i = 0; i < TRASH_TYPE_COUNT; i++) {
            if (trashBins[i].active) {
                // Lixeiras se movem um pouco mais devagar que os obstáculos para dar variedade.
                trashBins[i].x -= currentObstacleSpeed * 0.90f;
                // Lógica de reposicionamento similar à dos obstáculos.
                if (trashBins[i].x < -trashBins[i].width - 150) {
                    float rightmost_x_bin = 0;
                    for(int k=0; k < TRASH_TYPE_COUNT; ++k) {
                        if(trashBins[k].active && trashBins[k].x > -trashBins[k].width)
                            rightmost_x_bin = fmax(rightmost_x_bin, trashBins[k].x + trashBins[k].width);
                    }
                    trashBins[i].x = fmax((float)g_currentWindowWidth, rightmost_x_bin) + MIN_TRASHBIN_SPACING + (rand() % RAND_TRASHBIN_SPACING);
                }
            }
        }

        // --- MOVIMENTO DOS LIXOS ARREMESSADOS ---
        for (int i = 0; i < 10; i++) {
             if (thrownTrashItems[i].active) {
                // Atualiza a posição X e Y com base na velocidade do projétil.
                thrownTrashItems[i].x += thrownTrashItems[i].velocityX;
                thrownTrashItems[i].y += thrownTrashItems[i].velocityY;
                // Aplica uma força de gravidade para criar o arco da parábola.
                thrownTrashItems[i].velocityY -= GRAVITY * 0.35f;

                // Se o item saiu da tela, ele é desativado e retorna para o "pool" de objetos.
                if (thrownTrashItems[i].y < -50 || thrownTrashItems[i].x > g_currentWindowWidth + 50) {
                    thrownTrashItems[i].active = 0;
                }
            }
        }

        // --- MOVIMENTO DO FUNDO (PARALLAX SCROLLING) ---
        // Move as duas imagens de fundo para a esquerda.
        backgroundOffset1 -= 2.0f * speedMultiplier;
        backgroundOffset2 -= 2.0f * speedMultiplier;
        // Quando uma imagem sai completamente da tela, ela é reposicionada à direita da outra,
        // criando a ilusão de um fundo infinito.
        if (backgroundOffset1 <= -g_currentWindowWidth) backgroundOffset1 = backgroundOffset2 + g_currentWindowWidth;
        if (backgroundOffset2 <= -g_currentWindowWidth) backgroundOffset2 = backgroundOffset1 + g_currentWindowWidth;

        // --- ATUALIZAÇÕES FINAIS DA PARTIDA ---
        // Aumenta a dificuldade do jogo gradualmente, tornando-o mais rápido com o tempo.
        currentObstacleSpeed += gameTime * 0.000015f;
        // Chama a função que verifica todas as colisões.
        checkAllCollisions();
        // Verifica se as vidas do jogador acabaram para encerrar o jogo.
        if (lives <= 0) gameState = GAME_OVER;
    }
    // Informa ao GLUT que a tela precisa ser redesenhada, pois os estados dos objetos mudaram.
    glutPostRedisplay();
    // Agenda a próxima chamada a esta mesma função, criando o loop contínuo de ~60 FPS.
    glutTimerFunc(16, updateGame, 0);
}

/**
 * Verifica colisões entre o jogador, obstáculos, lixos arremessados e lixeiras.
 */
void checkAllCollisions() {
    // Define a "hitbox" (caixa de colisão) do jogador.
    float pHeight = player.ducking ? player.height / 1.8f : player.height;
    float playerTop = player.y + pHeight, playerBottom = player.y;
    float playerLeft = player.x, playerRight = player.x + player.width;

    // --- Colisão: Jogador vs. Obstáculos ---
    for (int i = 0; i < 5; i++) {
        if(obstacles[i].active) {
            float obsTop = obstacles[i].y + obstacles[i].height, obsBottom = obstacles[i].y;
            float obsLeft = obstacles[i].x, obsRight = obstacles[i].x + obstacles[i].width;
            
            // Verifica se as caixas de colisão do jogador e do obstáculo se sobrepõem.
            if (playerRight > obsLeft && playerLeft < obsRight && playerTop > obsBottom && playerBottom < obsTop) {
                lives--; // Perde uma vida.
                obstacles[i].x = g_currentWindowWidth + 250 + (rand()%200) + i * 20; // Joga o obstáculo para longe.
                if (lives <= 0) gameState = GAME_OVER;
                return; // Sai da função para evitar que o jogador perca múltiplas vidas em um único quadro.
            }
        }
    }
    
    // --- Colisão: Lixo Arremessado vs. Outros Objetos ---
    for (int i = 0; i < 10; i++) {
        if (thrownTrashItems[i].active) {
            float itemTop = thrownTrashItems[i].y + thrownTrashItems[i].height, itemBottom = thrownTrashItems[i].y;
            float itemLeft = thrownTrashItems[i].x, itemRight = thrownTrashItems[i].x + thrownTrashItems[i].width;

            // vs. Monstros
            for (int k = 0; k < 5; k++) {
                if (obstacles[k].active && (obstacles[k].type == MONSTER|| obstacles[k].type == FLYING_MONSTER)) {
                    float monsterTop = obstacles[k].y + obstacles[k].height;
                    float monsterBottom = obstacles[k].y;
                    float monsterLeft = obstacles[k].x;
                    float monsterRight = obstacles[k].x + obstacles[k].width;

                    if (itemRight > monsterLeft && itemLeft < monsterRight && itemTop > monsterBottom && itemBottom < monsterTop) {
                        if (thrownTrashItems[i].type == METAL) { // Apenas lixo de METAL destrói monstros.
                            score += 30;
                            obstacles[k].active = 0; // "Mata" o monstro.
                        }
                        thrownTrashItems[i].active = 0; // Desativa o projétil.
                        goto next_trash_item; // Pula para o próximo item de lixo.
                    }
                }
            }
            // vs. Lixeiras
            for (int j = 0; j < TRASH_TYPE_COUNT; j++) {
                if (trashBins[j].active) {
                    float binTop = trashBins[j].y + trashBins[j].height;
                    float binBottom = trashBins[j].y;
                    float binLeft = trashBins[j].x;
                    float binRight = trashBins[j].x + trashBins[j].width;

                    if (itemRight > binLeft && itemLeft < binRight && itemTop > binBottom && itemBottom < binTop) {
                        if (thrownTrashItems[i].type == trashBins[j].type) { // Acertou a lixeira correta.
                            score += 10;
                            if (score >= nextLifeScore) { // Verifica se ganhou vida extra.
                                if (lives < 3) { // Só ganha se não tiver o máximo de vidas.
                                    lives++;
                                    printf("Vida extra! Total de vidas: %d\n", lives);
                                } else {
                                    printf("Pontuacao para vida extra alcancada, mas vidas ja estao no maximo!\n");
                                }
                                // Define o próximo marco, 2000 pontos a partir do marco atual.
                                nextLifeScore += 2000; 
                                printf("Proxima vida extra em %d pontos.\n", nextLifeScore);
                            }
                        } else { // Errou a lixeira.
                            score -= 5;
                            if (score < 0) score = 0;
                        }
                        thrownTrashItems[i].active = 0; // Desativa o projétil.
                        goto next_trash_item; // Pula para o próximo item de lixo.
                    }
                }
            }
            next_trash_item:; // Label para o 'goto'. Usado para sair de loops aninhados eficientemente.
        }
    }
}

/**
 * "Cria" um novo item de lixo arremessado usando a técnica de "object pooling".
 * Em vez de criar e destruir objetos, reutiliza objetos de um "pool" pré-alocado.
 */
void spawnThrownTrashItem() {
    // Procura pelo primeiro item de lixo inativo no array.
    for (int i = 0; i < 10; i++) {
        if (!thrownTrashItems[i].active) {
            thrownTrashItems[i].active = 1; // Ativa-o.
            // Configura suas propriedades iniciais.
            thrownTrashItems[i].width = 25;
            thrownTrashItems[i].height = 25;
            thrownTrashItems[i].x = player.x + player.width; // Posição inicial perto do jogador.
            thrownTrashItems[i].y = player.y + (player.ducking ? player.height / 3.6f : player.height / 1.8f);
            thrownTrashItems[i].type = player.selectedTrash; // Usa o tipo de lixo que o jogador está segurando.
            thrownTrashItems[i].velocityX = TRASH_ITEM_SPEED_X;
            thrownTrashItems[i].velocityY = TRASH_ITEM_INITIAL_SPEED_Y;
            break; // Sai do loop assim que um item livre é encontrado e configurado.
        }
    }
}

/**
 * Alterna o tipo de lixo que o jogador está segurando.
 */
void cycleSelectedTrash() {
    int currentType = (int)player.selectedTrash;
    // Usa o operador módulo (%) para ciclar pelos tipos de lixo.
    // Quando chega ao fim da lista, ele volta para o primeiro tipo (0).
    currentType = (currentType + 1) % TRASH_TYPE_COUNT;
    player.selectedTrash = (TrashType)currentType;
}