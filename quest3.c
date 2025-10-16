#include <stdio.h>

/*QUESTÃO 03:
 *
 * Programa Calculadora de Razao de Eficiencia
 * 
 * Nome: [Gabriel Goldenberg Moita]
 * Matricula: [211061752]
 * 
 * Este programa calcula a "Razao de Eficiencia" de um numero inteiro N,
 * definida como sigma(N) / tau(N), onde:
 * - tau(N): numero total de divisores de N.
 * - sigma(N): soma de todos os divisores de N.
 * 
 * O calculo eh baseado na fatoracao prima de N, usando trial division
 * (eficiente para N <= 100000).
 * 
 * Entrada: Um inteiro N (1 <= N <= 100000).
 * Saida: Fatoracao, tau, sigma e a razao com 2 casas decimais.
 * 
 * Autor: Inspirado em problemas de Teoria dos Numeros.
 * 
 * Destaque com Cores: Usa codigos ANSI para colorir a saida no terminal
 * (funciona em terminais modernos; em alguns ambientes como Windows antigo,
 * pode nao renderizar cores).
 * Cores usadas:
 * - Headers e bordas: Amarelo bold
 * - Resultados principais (tau, sigma, razao, lista de fatores): Verde bold
 * - Erros: Vermelho
 * - Texto normal: Padrao (reset)
 */
int main() {
    
    printf("\033[33m\033[1m==========================================\033[0m\n");
    printf("\033[33m\033[1m=== CALCULADORA DE RAZAO DE EFICIENCIA ===\033[0m\n");
    printf("\033[33m\033[1m==========================================\033[0m\n\n");

    int N;           // Variavel para armazenar o numero de entrada
    int valid = 0;   // Flag para controlar a validacao da entrada
    
    // Loop para validar entrada de N (1 a 100000)
    // Garante que N seja um inteiro valido no range especificado
    while (!valid) {
        printf("Digite o numero N (1 a 100000): ");
        if (scanf("%d", &N) != 1) {  // Verifica se a leitura foi um inteiro
            printf("\033[31mEntrada invalida! Tente um numero inteiro valido.\033[0m\n");
            while (getchar() != '\n'); // Limpa o buffer de entrada para evitar loops
            continue;
        }
        if (N < 1 || N > 100000) {   // Verifica o range permitido
            printf("\033[31mErro: N deve estar entre 1 e 100000. Tente novamente.\033[0m\n");
            continue;
        }
        valid = 1;                   // Entrada valida, sai do loop
    }
    
    printf("\nProcessando N = %d...\n\n", N);  // Confirma o valor processado
    
    // Tratamento especial para N=1: tau(1)=1, sigma(1)=1
    if (N == 1) {
        printf("\033[33m\033[1m==========================================\033[0m\n");
        printf("\033[33m\033[1m=== RESULTADO PARA N=1 (CASO ESPECIAL) ===\033[0m\n");
        printf("\033[33m\033[1m==========================================\033[0m\n\n");
        printf("Fatores primos: 1\n");
        printf("Calculo de tau(N): 1\n");
        printf("Calculo de sigma(N): 1\n");
        printf("\033[32m\033[1mRazao de Eficiencia(N) = 1.00\033[0m\n\n");
        printf("\033[33m\033[1m================================\033[0m\n\n");
        printf("Obrigado por usar a calculadora!\n\n");
        printf("\033[33m\033[1m================================\033[0m\n\n");
        return 0;  // Termina o programa para o caso especial
    }
    
    int original_N = N;  // Salva o valor original para uso na saida final
    int primes[20];      // Array para armazenar os fatores primos (suficiente para N<=1e5)
    int exps[20];        // Array para armazenar os expoentes correspondentes
    int count = 0;       // Contador de fatores primos encontrados
    
    // Secao de Fatoracao Prima
    // Usa trial division: testa divisores de 2 ate sqrt(N), divide repetidamente
    printf("\033[33m\033[1m=== FATORACAO PRIMA ===\033[0m\n\n");
    printf("Lista de fatores primos e seus expoentes:\n\n");
    for (int i = 2; i * i <= N; i++) {  // Loop ate i <= sqrt(N), sem usar sqrt()
        if (N % i == 0) {               // Se i divide N
            int exp = 0;                // Inicializa expoente
            while (N % i == 0) {        // Divide N por i o maximo de vezes possivel
                N /= i;
                exp++;
            }
            primes[count] = i;          // Armazena o primo
            exps[count] = exp;          // Armazena o expoente
            printf("\033[32m\033[1m%d^%d \033[0m", i, exp);  // Imprime o fator em verde bold
            count++;                    // Incrementa contador
        }
    }
    // Se N > 1 apos o loop, eh um primo restante (maior que sqrt(original_N))
    if (N > 1) {
        primes[count] = N;
        exps[count] = 1;
        printf("\033[32m\033[1m%d^1 \033[0m", N);  // Imprime o ultimo fator em verde bold
        count++;
    }
    printf("\n\n");  // Quebra de linha para separar secoes
    
    // Secao de Tau(N)
    // tau(N) = produto de (expoente_i + 1) para todos os fatores
    printf("\033[33m\033[1m=== CALCULO DE TAU(N) - NUMERO DE DIVISORES ===\033[0m\n\n");
    printf("Formula: Produto de (expoente + 1) para cada fator primo.\n");
    int tau = 1;     // Inicializa tau como 1 (elemento neutro para multiplicacao)
    printf("Calculo intermediario de tau(N): ");
    for (int j = 0; j < count; j++) {
        int term = exps[j] + 1;  // Termo: expoente + 1
        printf("(%d + 1)", exps[j]);  // Imprime o termo
        if (j < count - 1) {
            printf(" * ");          // Separador entre termos
        }
        tau *= term;             // Acumula o produto
    }
    printf(" = \033[32m\033[1m%d\033[0m\n\n", tau);  // Resultado final de tau em verde bold
    
    // Secao de Sigma(N)
    // sigma(N) = produto de (p^(a+1) - 1) / (p - 1) para cada p^a
    printf("\033[33m\033[1m=== CALCULO DE SIGMA(N) - SOMA DE DIVISORES ===\033[0m\n\n");
    printf("Formula: Produto de (p^(a+1) - 1) / (p - 1) para cada fator primo p^a.\n");
    int sigma = 1;   // Inicializa sigma como 1 (int suficiente para N<=1e5, evita warning %lld)
    printf("Calculo intermediario de sigma(N): ");
    for (int j = 0; j < count; j++) {
        int p = primes[j];   // Fator primo atual
        int a = exps[j];     // Expoente atual
        // Calcular p^(a+1) manualmente (loop para evitar overflow)
        int p_pow_a1 = 1;
        for (int k = 0; k < a + 1; k++) {
            p_pow_a1 *= p;
        }
        int term = (p_pow_a1 - 1) / (p - 1);  // Formula geometrica para soma
        printf("((%d^%d - 1) / (%d - 1))", p, a + 1, p);  // Imprime a formula
        if (j < count - 1) {
            printf(" * ");       // Separador
        }
        sigma *= term;       // Acumula o produto
    }
    printf(" = \033[32m\033[1m%d\033[0m\n\n", sigma);  // Resultado final de sigma em verde bold (agora com %d)
    
    // Secao Final: Razao de Eficiencia
    // Razao = sigma(N) / tau(N), como double para precisao decimal
    printf("\033[33m\033[1m=== RESULTADO FINAL ===\033[0m\n\n");
    double razao = (double)sigma / tau;  // Casting para double
    printf("Razao de Eficiencia(%d) = \033[32m\033[1m%.2f\033[0m\n\n", original_N, razao);  // Razao em verde bold
    printf("\033[33m\033[1m================================\033[0m\n\n");
    printf("Obrigado por usar a calculadora!\n\n");
    printf("\033[33m\033[1m================================\033[0m\n\n");
    
    return 0;  // Termina o programa com sucesso
}