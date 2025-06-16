#include "Texture.h"
#include "Globals.h" // Para acessar os GLuint das texturas globais e Config.h para enums/defines
#include <stdio.h>   // Para printf, fprintf
#include <GL/glu.h>  // Para gluErrorString (opcional)
#include "stb_image.h" // A biblioteca que faz o trabalho pesado de carregar as imagens

/**
 * Carrega uma imagem de um arquivo e a converte para uma textura OpenGL.
 * filename O caminho para o arquivo de imagem.
 * O ID da textura gerada pelo OpenGL, ou 0 se falhar.
 */
GLuint loadTextureFromFile(const char* filename) {
    // Variáveis para armazenar o ID da textura e as propriedades da imagem.
    GLuint textureID = 0;
    int width, height, nrChannels; // nrChannels = número de canais de cor (ex: 3 para RGB, 4 para RGBA)
    
    // 1. Usa a biblioteca stb_image para carregar os dados de pixel do arquivo.
    unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

    // 2. Se o carregamento da imagem foi bem-sucedido...
    if (data) {
        // --- Detecção do Formato da Imagem ---
        // Variáveis para dizer ao OpenGL como interpretar os dados da imagem.
        GLenum format = GL_RGB;       // Formato dos dados de pixel de origem (na variável 'data').
        GLint internalFormat = GL_RGB;  // Formato que o OpenGL deve usar para armazenar a textura na GPU.

        if (nrChannels == 1) { // Imagem em escala de cinza.
            format = GL_RED;
            internalFormat = GL_RED;
        } else if (nrChannels == 3) { // Imagem RGB padrão.
            format = GL_RGB;
            internalFormat = GL_RGB;
        } else if (nrChannels == 4) { // Imagem RGBA com canal de transparência.
            format = GL_RGBA;
            internalFormat = GL_RGBA;
        }

        // --- Criação da Textura no OpenGL ---
        // 3. Pede ao OpenGL para gerar um ID único para a nossa textura.
        glGenTextures(1, &textureID);
        // 4. "Seleciona" a textura recém-criada para que os próximos comandos se apliquem a ela.
        glBindTexture(GL_TEXTURE_2D, textureID);

        // 5. Configura como a textura deve se comportar.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Repete a textura no eixo horizontal.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); // Repete a textura no eixo vertical.
        // Filtros para quando a textura é esticada ou encolhida. GL_LINEAR dá um efeito suave.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 6. Envia os dados de pixel (a variável 'data') da CPU para a GPU.
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        
        // 7. Libera a memória da imagem na CPU, pois a GPU já tem sua própria cópia.
        stbi_image_free(data); 
        printf("Textura carregada: %s (ID: %u)\n", filename, textureID);
    } else { 
        // Se stbi_load falhou, imprime uma mensagem de erro.
        fprintf(stderr, "Falha ao carregar textura: %s (stbi_load: %s)\n", filename, stbi_failure_reason() ? stbi_failure_reason() : "razao desconhecida"); 
    }
    // Desseleciona a textura para evitar modificações acidentais. Boa prática.
    glBindTexture(GL_TEXTURE_2D, 0); 
    
    // Retorna o ID da textura para ser usado mais tarde.
    return textureID;
}

/**
 *  Função de conveniência que carrega todas as texturas necessárias para o jogo.
 */
void loadAllTextures() {
    // Inverte a imagem no eixo Y durante o carregamento para corrigir a orientação do OpenGL.
    // Deve ser chamado uma única vez antes de todos os carregamentos.
    stbi_set_flip_vertically_on_load(true); 
    printf("Carregando todas as texturas...\n");

    // Carrega cada textura e armazena seu ID na variável global correspondente.
    playerRunTexture1   = loadTextureFromFile("textures/player_run1.png"); 
    playerRunTexture2   = loadTextureFromFile("textures/player_run2.png"); 
    playerJumpTexture   = loadTextureFromFile("textures/player_jump.png");
    playerDuckTexture   = loadTextureFromFile("textures/player_duck.png");
    backgroundTexture   = loadTextureFromFile("textures/background.png");

    obstacleTextures[HOLE] = loadTextureFromFile("textures/obstacle_hole.png"); 
    obstacleTextures[DOG]  = loadTextureFromFile("textures/obstacle_dog.png"); 
    obstacleTextures[BIKE] = loadTextureFromFile("textures/obstacle_bike.png");
    obstacleTextures[MONSTER] = loadTextureFromFile("textures/obstacle_monster.png"); 
    obstacleTextures[FLYING_MONSTER] = loadTextureFromFile("textures/obstacle_flying_monster.png");

    trashBinTextures[PAPER]   = loadTextureFromFile("textures/trashbin_paper.png"); 
    trashBinTextures[GLASS]   = loadTextureFromFile("textures/trashbin_glass.png");
    trashBinTextures[PLASTIC] = loadTextureFromFile("textures/trashbin_plastic.png"); 
    trashBinTextures[METAL]   = loadTextureFromFile("textures/trashbin_metal.png"); 
    trashBinTextures[ORGANIC] = loadTextureFromFile("textures/trashbin_organic.png");

    trashItemTextures[PAPER]   = loadTextureFromFile("textures/trashitem_paper.png"); 
    trashItemTextures[GLASS]   = loadTextureFromFile("textures/trashitem_glass.png");
    trashItemTextures[PLASTIC] = loadTextureFromFile("textures/trashitem_plastic.png"); 
    trashItemTextures[METAL]   = loadTextureFromFile("textures/trashitem_metal.png"); 
    trashItemTextures[ORGANIC] = loadTextureFromFile("textures/trashitem_organic.png");
    
    
    printf("Carregamento de todas as texturas concluido.\n");

    // Verifica se as texturas mais importantes foram carregadas com sucesso.
    if (!playerRunTexture1 || !playerRunTexture2 || !playerJumpTexture || !playerDuckTexture || !backgroundTexture) {
        fprintf(stderr, "Aviso: Alguma textura essencial do jogador ou fundo pode não ter carregado.\n");
    }
}

void drawQuadWithTexture(GLuint texture, float x, float y, float width, float height) {
    // Diz ao OpenGL qual textura usar para o próximo desenho.
    glBindTexture(GL_TEXTURE_2D, texture); 
    // Garante que a textura não seja "tingida" por uma cor diferente de branco.
    glColor3f(1.0f, 1.0f, 1.0f); 
    
    // Inicia o desenho de um quadrilátero.
    glBegin(GL_QUADS);
        // Mapeia os cantos da imagem (de 0,0 a 1,1) para os cantos do retângulo na tela.
        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);                   // Canto inferior esquerdo
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + width, y);          // Canto inferior direito
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + width, y + height); // Canto superior direito
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y + height);         // Canto superior esquerdo
    glEnd();
}

/**
 * Libera a memória da GPU que foi alocada para todas as texturas.
 */
void cleanupTextures() {
    // Verifica se a textura existe (ID > 0) antes de tentar deletá-la.
    if (backgroundTexture) glDeleteTextures(1, &backgroundTexture);
    if (playerRunTexture1) glDeleteTextures(1, &playerRunTexture1);
    if (playerRunTexture2) glDeleteTextures(1, &playerRunTexture2);
    if (playerJumpTexture) glDeleteTextures(1, &playerJumpTexture);
    if (playerDuckTexture) glDeleteTextures(1, &playerDuckTexture);
    
    // Percorre os arrays de texturas para deletar cada uma.
    for(int i = 0; i < OBSTACLE_TYPE_COUNT; ++i) {
        if(obstacleTextures[i]) glDeleteTextures(1, &obstacleTextures[i]);
    }
    for(int i = 0; i < TRASH_TYPE_COUNT; ++i) {
        if(trashBinTextures[i]) glDeleteTextures(1, &trashBinTextures[i]);
        if(trashItemTextures[i]) glDeleteTextures(1, &trashItemTextures[i]);
    }
    printf("Texturas liberadas.\n");
}