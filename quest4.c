/*
QUESTÃO 4 PARTE 1 (EXTRA)
Aluno: [Gabriel Goldenberg Moita]
Matrícula: [211061752]

====================================================
    RESPOSTAS NO COMENTÁRIO AO FINAL DO CÓDIGO!!
====================================================
*/

#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#endif

// Função para calcular o máximo divisor comum (MDC) com exibição dos passos
int mdcComPassos(int a, int b) {
    int resto;
    while (b != 0) {
        resto = a % b;  // Calcula o resto da divisão de a por b
        printf("Algoritmo de Euclides: %d mod %d = %d\n", a, b, resto);
        a = b;          // Atualiza a para o valor de b
        b = resto;      // Atualiza b para o valor do resto
    }
    return a;  // Quando b chega a 0, a contém o MDC
}

// Função para calcular o inverso modular usando o Algoritmo Estendido de Euclides
int inversoModular(int a, int m) {
    int m0 = m, t, q;        // Guarda o valor original de m
    int x0 = 0, x1 = 1;      // Coeficientes para a equação de Bézout
    int A = a, B = m;        // Guarda os valores originais para exibição

    // Primeiro calculamos o MDC para verificar se existe inverso
    mdcComPassos(a,m);

    // Algoritmo Estendido de Euclides para encontrar o inverso
    while (m != 0) {
        q = a / m;      // Quociente da divisão
        t = m;          // Armazena m temporariamente
        m = a % m;      // Atualiza m com o resto
        a = t;          // Atualiza a com o m anterior

        // Atualiza os coeficientes x0 e x1
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    
    // Ajusta para garantir que o inverso seja positivo
    if (x1 < 0) {
        x1 += m0;
    }
    
    // Exibe o resultado de forma didática
    printf("\nSubstituindo, temos que o inverso de %d em %d é %d.\n\n", A, B, x1);
    return x1;
}

// Função para calcular (base^exp) % mod de forma eficiente
int powMod(int base, int exp, int mod) {
    long long res = 1;        // Resultado (usamos long long para evitar overflow)
    long long b = base % mod; // Base reduzida módulo mod
    
    // Algoritmo de exponenciação por quadrados (exponenciação binária)
    while (exp > 0) {
        if (exp % 2 == 1)     // Se o expoente é ímpar
            res = (res * b) % mod;  // Multiplica o resultado pela base atual
        
        b = (b * b) % mod;    // Eleva a base ao quadrado
        exp >>= 1;            // Divide o expoente por 2 (deslocamento à direita)
    }
    return (int)res;  // Retorna o resultado como int
}

int main() {
    // Configura o console para UTF-8 no Windows (para caracteres especiais)
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    // Declaração das variáveis que o usuário vai informar
    int H, G, Zn, x, n1;

    // Entrada de dados do usuário
    printf("Insira H: ");
    scanf("%d", &H);
    printf("Insira G: ");
    scanf("%d", &G);
    printf("Insira Zn: ");
    scanf("%d", &Zn);
    printf("Insira x: ");
    scanf("%d", &x);
    printf("Insira n1: ");
    scanf("%d", &n1);
    printf("\n");

    // Cálculo do inverso modular de G em Zn
    int inverso = inversoModular(G, Zn);
    
    // Cálculo: a = (H * inverso) % Zn
    int a = (H * inverso) % Zn;

    // Explicação do cálculo intermediário
    printf("Fazendo a multiplicação modular: %d * %d mod %d === %d\n", H, inverso, Zn, a);
    printf(" Sendo %d o inverso de %d.\n", inverso, G);

    // Cálculo final: a^x mod n1
    int resultado = powMod(a, x, n1);
    printf("Valor final da congruência: %d\n", resultado);

    return 0;
}
/*

1)

saída com os valores: 

H: 7, G: 3, Zn: 11, x: 10, n1: 13

-----------------------------------------

RESULTADO:

Algoritmo de Euclides: 3 mod 11 = 3
Algoritmo de Euclides: 11 mod 3 = 2
Algoritmo de Euclides: 3 mod 2 = 1
Algoritmo de Euclides: 2 mod 1 = 0

Substituindo, temos que o inverso de 3 em 11 é 4.

Fazendo a multiplicação modular: 7 * 4 mod 11 === 6
 Sendo 4 o inverso de 3.
Valor final da congruência: 4
===============================================================================

2)

=================
RESPOSTAS V OU F:
=================

1. V  
2. F  
3. V  
4. F  
5. F  
6. V  
7. F

==============================
    Explicação do (V ou F)
==============================

1. (V): A função inversoModular usa o algoritmo estendido de Euclides (com coeficientes x0/x1 para os coeficientes de Bézout, onde ax + my = 1),
retornando x1 como o inverso de a mod m, desde que gcd(a, m) = 1.

2. (F): O algoritmo estendido só garante um inverso se gcd(G, Zn) = 1 (condição implícita para existência do inverso modular).
Se gcd ≠ 1, o loop termina com m ≠ 0 ou x1 inválido, mas o código não verifica isso explicitamente (ele assume coprimos e pode retornar um valor incorreto sem erro).

3. (V): Na aritmética modular, dividir H por G é multiplicar H pelo inverso modular de G (pois H / G ≡ H * G^{-1} mod Zn), e o % Zn garante o resto.

4. (F): O `powMod` usa exponenciação binária (quadrado-e-multiplicar) sem redução de expoente. 
Não há verificação se n1 é primo nem aplicação de Fermat (a^{p-1} ≡ 1 mod p para redução x mod (p-1)).

5. (F): Ela usa otimização: exponenciação binária (while exp > 0, verifica bit ímpar com exp % 2,
quadrado de base e shift exp >>= 1), que é O(log x) em vez de O(x) multiplicações diretas.

6. (V): No final tem um if (x1 < 0) que soma m0 (o módulo original), pra deixar o inverso positivo e no range certo [0, m-1]. 
Para não dar problema.

7. (F): Não há cálculo de φ(n1) em lugar nenhum no código. O `powMod` não usa Euler/Fermat para redução (não há % φ(n1)), 
independentemente de n1 ser primo ou não, ele sempre faz exponenciação binária direta.
*/












