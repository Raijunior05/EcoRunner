// --- Inclusão de Bibliotecas e Módulos ---
// Inclui a biblioteca GLUT, essencial para criar janelas e gerenciar eventos.
#include <GL/glut.h>
// Inclui bibliotecas padrão do C para entrada/saída e tempo.
#include <stdio.h>
#include <time.h>

// Inclui nossos próprios módulos, trazendo as declarações de funções e tipos de cada um.
#include "Config.h"
#include "Globals.h"
#include "Texture.h"
#include "GameLogic.h"
#include "Renderer.h"
#include "Input.h"

// Definição do STB_IMAGE_IMPLEMENTATION (APENAS EM UM ARQUIVO .CPP)
// Esta linha diz à biblioteca stb_image.h para incluir aqui o código-fonte
// das funções de carregamento de imagem. Isso deve ser feito em apenas um
// arquivo .cpp do projeto para evitar erros de compilação.
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Bloco para depuração do diretório de trabalho atual.
// Ajuda a resolver problemas de "arquivo não encontrado" para as texturas.
#ifdef _WIN32
    #include <direct.h>
    #define GETCWD _getcwd
#else
    #include <unistd.h>
    #define GETCWD getcwd
#endif

int main(int argc, char** argv) {
    // Código de depuração para imprimir o diretório de trabalho atual no console.
    char current_path_buffer[FILENAME_MAX];
    if (GETCWD(current_path_buffer, sizeof(current_path_buffer)) != NULL) {
        printf("Diretorio de trabalho atual: %s\n", current_path_buffer);
    } else {
        perror("Falha ao obter o diretorio de trabalho atual");
    }

    // --- INICIALIZAÇÃO DO GLUT E DA JANELA ---
    // Inicializa a biblioteca GLUT, passando os argumentos da linha de comando.
    glutInit(&argc, argv);
    // Configura o modo de exibição da janela.
    // GLUT_DOUBLE -> Usa um buffer duplo para evitar cintilação (flickering) nas animações.
    // GLUT_RGB    -> Define o modo de cor para Vermelho, Verde e Azul.
    // GLUT_ALPHA  -> Habilita o canal alfa, necessário para transparência das texturas .png.
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_ALPHA);
    // Define o tamanho inicial da janela usando as constantes de Config.h.
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    // Cria a janela com o título especificado.
    glutCreateWindow("Eco Runner: Missão Reciclar (GLUT Modular)");

    // --- CONFIGURAÇÕES INICIAIS DO OPENGL ---
    // Define a cor de fundo (um azul-céu) que será usada ao limpar a tela.
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
    // Habilita a mistura de cores (blending), essencial para a transparência.
    glEnable(GL_BLEND);
    // Define como a transparência funcionará, permitindo que pixels transparentes de uma imagem
    // revelem o que está por trás.
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Chama nossa função para carregar todas as imagens do jogo para a memória da GPU.
    loadAllTextures();

    // --- REGISTRO DE CALLBACKS ---
    // Esta é a parte central do GLUT. Dizemos ao GLUT qual de nossas funções chamar
    // quando um determinado evento acontece.
    glutDisplayFunc(display);           // Quando a tela precisar ser desenhada, chame a função 'display'.
    glutReshapeFunc(reshape);           // Quando a janela for redimensionada, chame a função 'reshape'.
    glutKeyboardFunc(keyboard);         // Quando uma tecla normal for pressionada, chame 'keyboard'.
    glutKeyboardUpFunc(keyboardUp);     // Quando uma tecla normal for solta, chame 'keyboardUp'.
    glutSpecialFunc(specialKeyboard);   // Quando uma tecla especial (setas, F1) for pressionada, chame 'specialKeyboard'.
    glutSpecialUpFunc(specialKeyboardUp); // Quando uma tecla especial for solta, chame 'specialKeyboardUp'.
    glutMouseFunc(mouse);               // Quando ocorrer um clique do mouse, chame 'mouse'.
    
    // Configura o timer para o nosso loop de lógica do jogo.
    // Diz ao GLUT para chamar a função 'updateGame' após 16 milissegundos (~60 FPS).
    glutTimerFunc(16, updateGame, 0);

    // Inicializa o gerador de números aleatórios usando o tempo atual como semente.
    // Isso garante que a sequência de obstáculos seja diferente a cada vez que o jogo é executado.
    srand(time(NULL));
    
    printf("Iniciando loop principal do GLUT...\n");
    // Inicia o loop de eventos do GLUT. O programa fica "preso" aqui, esperando por
    // eventos (teclado, mouse, timer) e chamando as funções de callback registradas.
    glutMainLoop();
    
    // Esta parte do código só é alcançada quando o glutMainLoop termina (geralmente ao fechar a janela).
    // Libera a memória da GPU que foi alocada para as texturas.
    cleanupTextures();
    return 0;
}