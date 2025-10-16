/*
 * Programa Chaves Periodicas
 * 
 * Nome: [Gabriel Goldenbeg Moita]
 * Matricula: [211061752]
 * 
 * Este programa resolve o problema das "Chaves Periodicas", inspirado no problema
 * das "Cigarras Periodicas" (Beecrowd 2660). Calcula o primeiro ano futuro (>0)
 * em que todas as chaves se ativam simultaneamente, baseado no Menor Multiplo
 * Comum (MMC) dos ciclos Ci. Considera apenas anos de 1 a 50; se nao houver,
 * informa impossibilidade.
 * 
 * Entrada:
 * - N (1 <= N <= 10): numero de chaves.
 * - N inteiros Ci (2 <= Ci <= 20): ciclos de ativacao.
 * 
 * Saida:
 * - Um inteiro X (1 <= X <= 50): o ano sincronizado, ou "Nao e possivel."
 * 
 * Metodo: Calcula MMC cumulativo usando MDC (Euclides) e formula MMC(a,b) = a*b / MDC(a,b).
 * Validacao interativa com prompts e retries para entradas invalidas.
 * 
 * Autor: Inspirado em Maratona de Programacao SBC 2017.
 * 
 * Destaque com Cores: Usa codigos ANSI para colorir a saida no terminal
 * (funciona em terminais modernos; em alguns ambientes como Windows antigo,
 * pode nao renderizar cores).
 * Cores usadas:
 * - Titulo: Amarelo
 * - Erros: Vermelho
 * - Resultado final (ano ou mensagem): Verde para sucesso, vermelho para falha
 * - Texto normal: Padrao
 */

#include <stdio.h>

/*
 * Funcao para calcular o Maior Divisor Comum (MDC) de dois numeros inteiros
 * usando o algoritmo de Euclides de forma iterativa.
 * Parametros: a e b (inteiros positivos)
 * Retorno: O MDC de a e b
 */
int gcd(int a, int b) {
    while (b != 0) {
        int t = b;       // Armazena o valor atual de b
        b = a % b;       // Calcula o resto da divisao
        a = t;           // Atualiza a com o valor anterior de b
    }
    return a;            // Quando b == 0, a eh o MDC
}

/* 
 * Funcao para calcular o Menor Multiplo Comum (MMC) de dois numeros inteiros.
 * Usa a formula: MMC(a, b) = (a * b) / MDC(a, b)
 * Nota: A multiplicacao eh feita apos a divisao para evitar overflow desnecessario.
 * Parametros: a e b (inteiros positivos)
 * Retorno: O MMC de a e b
 */
int lcm(int a, int b) {
    return a / gcd(a, b) * b;  // Calcula e retorna o MMC
}

int main() {
    
    printf("\033[33m\033[1m=== CHAVES PERIODICAS - SINCRONIZACAO DE CICLOS ===\033[0m\n\n");
    
    int N;               // Numero de chaves
    int valid = 0;       // Flag para controlar a validacao de entrada
    
    /* 
     * Loop para ler e validar o numero de chaves (N)
     * Garante que N esteja entre 1 e 10
     */
    while (!valid) {
        printf("Digite o numero de chaves (1 a 10): ");
        if (scanf("%d", &N) != 1) {  // Verifica se a leitura foi um inteiro valido
            printf("\033[31mEntrada invalida! Tente novamente.\033[0m\n");
            while (getchar() != '\n'); // Limpa o buffer de entrada para evitar loops infinitos
            continue;                // Reinicia o loop
        }
        if (N < 1 || N > 10) {       // Verifica o range
            printf("\033[31mErro: O numero de chaves deve estar entre 1 e 10. Tente novamente.\033[0m\n");
            continue;                // Reinicia o loop
        }
        valid = 1;                   // Saida valida, prossegue
    }
    
    int C[10];           // Array para armazenar os ciclos das chaves (max 10)
    
    /* 
     * Loop para ler e validar cada ciclo das N chaves
     * Cada Ci deve estar entre 2 e 20
     */
    for (int i = 0; i < N; i++) {
        valid = 0;       // Reseta a flag para cada ciclo
        while (!valid) {
            printf("Digite o ciclo da chave %d (2 a 20): ", i + 1);
            if (scanf("%d", &C[i]) != 1) {  // Verifica se a leitura foi um inteiro valido
                printf("\033[31mEntrada invalida! Tente novamente.\033[0m\n");
                while (getchar() != '\n'); // Limpa o buffer
                continue;
            }
            if (C[i] < 2 || C[i] > 20) { // Verifica o range
                printf("\033[31mErro: O ciclo deve estar entre 2 e 20. Tente novamente.\033[0m\n");
                continue;
            }
            valid = 1;       // Saida valida para este ciclo
        }
    }
    
    /* 
     * Calculo do MMC cumulativo de todos os ciclos
     * Inicia com o primeiro ciclo e acumula o MMC com os demais
     */
    int res = C[0];      // MMC inicial = primeiro ciclo
    for (int i = 1; i < N; i++) {
        res = lcm(res, C[i]);  // Atualiza o MMC com o proximo ciclo
    }
    
    /* 
     * Verificacao e saida final
     * Se o MMC (res) estiver entre 1 e 50, imprime o ano sincronizado
     * Caso contrario, informa que nao eh possivel dentro do limite
     */
    if (res >= 1 && res <= 50) {
        printf("\033[32m\033[1m%d\033[0m\n", res);  // Ano sincronizado em verde bold
    } else {
        printf("\033[31m\033[1mNao e possivel.\033[0m\n");  // Mensagem de falha em vermelho bold
    }
    
    return 0;            // Termina o programa com sucesso
}