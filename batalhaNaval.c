#include <stdio.h>
#include <stdlib.h> // Incluído para usar a função abs()

// Definições de Constantes
#define TAMANHO_TABULEIRO 10
#define TAMANHO_HABILIDADE 5
#define METADE_HABILIDADE 2 // (5-1) / 2 = 2

// Códigos de Exibição
#define COD_AGUA 0
#define COD_NAVIO 3
#define COD_AFETADO 5
#define CHAR_AGUA '.'
#define CHAR_NAVIO 'N'
#define CHAR_AFETADO 'X'

// --- Funções Auxiliares (Protótipos) ---
void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
void resetarEfeitoHabilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);
void imprimirCabecalho(const char *titulo);
void exibirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]);

// --- Funções de Habilidade (Implementações) ---

/**
 * @brief Cria a matriz de efeito Cone (ponto de origem no topo, expandindo para baixo).
 */
void criarMatrizCone(int matriz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    int i, j;
    
    for (i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Inicializa a posição como 0 (não afetada)
            matriz[i][j] = 0;
            
            // Condicional: Área afetada (1) expande-se conforme a linha (i) aumenta.
            // Largura do cone = 2*i + 1
            if (j >= METADE_HABILIDADE - i && j <= METADE_HABILIDADE + i) {
                matriz[i][j] = 1;
            }
        }
    }
}

/**
 * @brief Cria a matriz de efeito Cruz (ponto de origem no centro).
 */
void criarMatrizCruz(int matriz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    int i, j;
    
    for (i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (j = 0; j < TAMANHO_HABILIDADE; j++) {
            // Condicional: É 1 se estiver na linha central OU na coluna central.
            if (i == METADE_HABILIDADE || j == METADE_HABILIDADE) {
                matriz[i][j] = 1;
            } else {
                matriz[i][j] = 0;
            }
        }
    }
}

/**
 * @brief Cria a matriz de efeito Octaedro/Losango (ponto de origem no centro).
 * A forma é definida pela distância de Manhattan até o centro.
 */
void criarMatrizOctaedro(int matriz[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE]) {
    int i, j;
    
    for (i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (j = 0; j < TAMANHO_HABILIDADE; j++) {
            
            // Condicional: Distância de Manhattan deve ser <= METADE_HABILIDADE.
            int distancia_manhattan = abs(i - METADE_HABILIDADE) + abs(j - METADE_HABILIDADE);
            
            if (distancia_manhattan <= METADE_HABILIDADE) {
                matriz[i][j] = 1;
            } else {
                matriz[i][j] = 0;
            }
        }
    }
}

// --- Lógica Principal de Sobreposição e Exibição ---

/**
 * @brief Inicializa o tabuleiro 10x10 com água (0) e navios (3).
 */
void inicializarTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    int i, j;
    
    // Preenche com água
    for (i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (j = 0; j < TAMANHO_TABULEIRO; j++) {
            tabuleiro[i][j] = COD_AGUA;
        }
    }

    // Posiciona navios (código 3) - Fixos para teste
    // Navio Horizontal (4x1)
    for (j = 3; j < 7; j++) {
        tabuleiro[3][j] = COD_NAVIO;
    }
    // Navio Vertical (3x1)
    for (i = 6; i < 9; i++) {
        tabuleiro[i][2] = COD_NAVIO;
    }
}

/**
 * @brief Remove apenas o efeito da habilidade (5), mantendo os navios (3) e água (0).
 */
void resetarEfeitoHabilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    int i, j;
    for (i = 0; i < TAMANHO_TABULEIRO; i++) {
        for (j = 0; j < TAMANHO_TABULEIRO; j++) {
            if (tabuleiro[i][j] == COD_AFETADO) {
                tabuleiro[i][j] = COD_AGUA;
            }
        }
    }
}

/**
 * @brief Sobrepõe o efeito da habilidade ao tabuleiro, marcando a área afetada (código 5).
 */
void aplicarHabilidade(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO], 
                       int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE],
                       int origem_linha, int origem_coluna) {
    int i, j;
    
    // Loop aninhado para iterar sobre a matriz de habilidade
    for (i = 0; i < TAMANHO_HABILIDADE; i++) {
        for (j = 0; j < TAMANHO_HABILIDADE; j++) {
            
            // 1. Verificar se a posição na matriz de habilidade está ativada (valor 1)
            if (matriz_habilidade[i][j] == 1) {
                
                // 2. Calcular a posição correspondente no tabuleiro (centralizando)
                int tab_linha = origem_linha + (i - METADE_HABILIDADE);
                int tab_coluna = origem_coluna + (j - METADE_HABILIDADE);
                
                // 3. Condicional: Verificar se a posição calculada está dentro dos limites do tabuleiro
                if (tab_linha >= 0 && tab_linha < TAMANHO_TABULEIRO &&
                    tab_coluna >= 0 && tab_coluna < TAMANHO_TABULEIRO) {
                    
                    // 4. Marcar a área afetada (5), mas NUNCA sobrescrever um Navio (3).
                    if (tabuleiro[tab_linha][tab_coluna] != COD_NAVIO) {
                        tabuleiro[tab_linha][tab_coluna] = COD_AFETADO;
                    }
                }
            }
        }
    }
}

/**
 * @brief Exibe o tabuleiro no console utilizando caracteres para diferenciação.
 */
void exibirTabuleiro(int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO]) {
    int i, j;
    
    // Cabeçalho de Colunas
    printf("   ");
    for (j = 0; j < TAMANHO_TABULEIRO; j++) {
        printf("%d ", j);
    }
    printf("\n");
    
    // Loop aninhado para exibição
    for (i = 0; i < TAMANHO_TABULEIRO; i++) {
        printf("%d |", i); // Linhas
        for (j = 0; j < TAMANHO_TABULEIRO; j++) {
            
            // Condicional: Escolhe o caractere a ser exibido
            if (tabuleiro[i][j] == COD_AFETADO) {
                printf("%c ", CHAR_AFETADO); // X para Área Afetada (5)
            } else if (tabuleiro[i][j] == COD_NAVIO) {
                printf("%c ", CHAR_NAVIO);   // N para Navio (3)
            } else {
                printf("%c ", CHAR_AGUA);    // . para Água (0)
            }
        }
        printf("\n");
    }
}

/**
 * @brief Imprime o cabeçalho e o título da peça no console.
 */
void imprimirCabecalho(const char *titulo) {
    printf("----------------------------------------\n");
    printf("%s\n", titulo);
    printf("----------------------------------------\n");
}

int main() {
    int tabuleiro[TAMANHO_TABULEIRO][TAMANHO_TABULEIRO];
    int matriz_habilidade[TAMANHO_HABILIDADE][TAMANHO_HABILIDADE];
    
    // Ponto de origem para todas as habilidades
    const int origem_linha = 5;
    const int origem_coluna = 5;

    // Inicializa o tabuleiro com navios ANTES do primeiro teste
    inicializarTabuleiro(tabuleiro);

    // --- 1. Habilidade CONE ---
    
    imprimirCabecalho("1. HABILIDADE CONE (Origem: 5, 5)");
    criarMatrizCone(matriz_habilidade);
    aplicarHabilidade(tabuleiro, matriz_habilidade, origem_linha, origem_coluna);
    exibirTabuleiro(tabuleiro);
    printf("\n");

    // Limpa apenas o efeito da habilidade (5) para o próximo teste
    resetarEfeitoHabilidade(tabuleiro);
    
    // --- 2. Habilidade CRUZ ---
    
    imprimirCabecalho("2. HABILIDADE CRUZ (Origem: 5, 5)");
    criarMatrizCruz(matriz_habilidade);
    aplicarHabilidade(tabuleiro, matriz_habilidade, origem_linha, origem_coluna);
    exibirTabuleiro(tabuleiro);
    printf("\n");

    // Limpa apenas o efeito da habilidade (5) para o próximo teste
    resetarEfeitoHabilidade(tabuleiro);

    // --- 3. Habilidade OCTAEDRO (Losango) ---

    imprimirCabecalho("3. HABILIDADE OCTAEDRO (Origem: 5, 5)");
    criarMatrizOctaedro(matriz_habilidade);
    aplicarHabilidade(tabuleiro, matriz_habilidade, origem_linha, origem_coluna);
    exibirTabuleiro(tabuleiro);
    printf("\n");


    return 0;
}
