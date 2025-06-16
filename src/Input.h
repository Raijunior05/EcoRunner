#ifndef INPUT_H
#define INPUT_H

// --- Protótipos de Funções ---
// Funções de callback que respondem às entradas do usuário (teclado e mouse).

void keyboard(unsigned char key, int x, int y);  // Processa eventos de teclas normais pressionadas (letras, números).
void keyboardUp(unsigned char key, int x, int y);  // Processa eventos de teclas normais soltas.
void specialKeyboard(int key, int x, int y);     // Processa eventos de teclas especiais pressionadas (setas, F1, etc.).
void specialKeyboardUp(int key, int x, int y);     // Processa eventos de teclas especiais soltas.
void mouse(int button, int state, int x, int y); // Processa eventos de clique do mouse.

#endif //INPUT_H