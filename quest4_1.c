/*
QUESTÃO 4 PARTE 1 (EXTRA)
Aluno: Gabriel Goldenberg Moita
Matrícula: 211061752

====================================================
    RESPOSTA NO COMENTÁRIO AO FINAL DO CÓDIGO!!
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
        resto = a % b;
        printf("Algoritmo de Euclides: %d mod %d = %d\n", a, b, resto);
        a = b;
        b = resto;
    }
    return a;
}

int inversoModular(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;
    int A = a, B = m;

    mdcComPassos(a,m);

    while (m != 0) {
        q = a / m;
        t = m;
        m = a % m;
        a = t;

        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }
    if (x1 < 0) {
        x1 += m0;
    }
    printf("\nSubstituindo, temos que o inverso de %d em %d é %d.\n\n", A, B, x1);
    return x1;
}

int powMod(int base, int exp, int mod) {
    long long res = 1;
    long long b = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            res = (res * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (int)res;
}

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    int H, G, Zn, x, n1;

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

    int inverso = inversoModular(G,Zn);
    int a = (H * inverso) % Zn;

    printf("Fazendo a multiplicação modular: %d * %d mod %d === %d\n", H, inverso, Zn, a);
    printf(" Sendo %d o inverso de %d.\n", inverso, G);

    int resultado = powMod(a,x,n1);
    printf("Valor final da congruência: %d\n", resultado);

    return 0;
}

/*
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
*/










