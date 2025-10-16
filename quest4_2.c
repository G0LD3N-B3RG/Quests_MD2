/*
QUESTÃO 4 PARTE 2 (EXTRA)
Aluno: Gabriel Goldenberg Moita
Matrícula: 211061752

====================================================
    RESPOSTA NO COMENTÁRIO AO FINAL DO CÓDIGO!!
====================================================
*/

#include <stdio.h> 

/*
 * Programa para Cálculo de Divisão Modular e Potência Modular
 * 
 * Este programa demonstra conceitos de aritmética modular:
 * 1. Cálculo do MDC (Maior Divisor Comum) via Euclides, com passos visíveis.
 * 2. Inverso modular usando o algoritmo estendido de Euclides.
 * 3. Divisão modular: H / G mod Zn = H * (G^{-1}) mod Zn.
 * 4. Potência modular: a^x mod n1, usando exponenciação binária (otimizada).
 * 
 * Conceitos chave:
 * - MDC: Útil para verificar se inverso existe (só se MDC(G, Zn) = 1).
 * - Inverso modular: Número i tal que G * i ≡ 1 mod Zn.
 * - Potência modular: Evita overflow com % mod a cada passo.
 * 
 * Entrada: H, G, Zn, x, n1 (via scanf).
 * Saída: Passos do MDC, inverso, divisão e potência final.
 */

#ifdef _WIN32
#include <windows.h>
#endif

/*
 * Função: mdcComPassos
 * 
 * Calcula o MDC de a e b usando o Algoritmo de Euclides (iterativo).
 * Didática: Mostra cada passo da divisão (modulo) para entender como o resto
 * diminui até 0. O MDC é o último não-zero (propriedade: MDC(a, b) = MDC(b, a%b)).
 * 
 * Parâmetros:
 * - a, b: Números inteiros positivos (assume a >= b para simplicidade).
 * 
 * Retorno: O MDC de a e b.
 * 
 * Exemplo: MDC(3,11) → 11 mod 3=2, 3 mod 2=1, 2 mod 1=0 → MDC=1.
 */
int mdcComPassos(int a, int b) {
    int resto;  // Variável temporária para armazenar o resto da divisão
    while (b != 0) {  // Loop continua enquanto o divisor não for 0 (condição de parada do Euclides)
        resto = a % b;  // Calcula o resto: a = q*b + resto, com 0 <= resto < b
        printf("Algoritmo de Euclides: %d mod %d = %d\n", a, b, resto);  // Mostra o passo (didático!)
        a = b;          // Atualiza: agora MDC(a, b) = MDC(b, resto)
        b = resto;      // Próximo divisor é o resto anterior
    }
    return a;           // Quando b=0, a é o MDC
}

/*
 * Função: inversoModular
 * 
 * Calcula o inverso modular de a mod m usando o Algoritmo Estendido de Euclides.
 * Didática: Além do MDC, constrói coeficientes x e y tal que a*x + m*y = MDC.
 * Se MDC=1, x é o inverso (a*x ≡ 1 mod m). Assume coprimos (MDC=1); senão, falha.
 * 
 * Parâmetros:
 * - a: Número base (o que queremos inverter).
 * - m: Módulo (Zn no main).
 * 
 * Retorno: O inverso i tal que a*i ≡ 1 mod m (positivo, [0, m-1]).
 * 
 * Exemplo: Inverso de 3 mod 11 → 4 (3*4=12 ≡ 1 mod 11).
 * Passos: Chama mdcComPassos para mostrar Euclides, depois constrói x1.
 */
int inversoModular(int a, int m) {
    int m0 = m, t, q;  // m0: cópia de m (para ajuste final); t,q: temporários
    int x0 = 0, x1 = 1;  // Coeficientes iniciais de Bézout: x0 para m, x1 para a
    int A = a, B = m;    // Cópias para print (valores originais)

    mdcComPassos(a, m);  // Mostra passos do Euclides simples (MDC)

    // Loop principal: Estende Euclides para achar coeficientes (back-substitution)
    while (m != 0) {     // Similar ao Euclides, mas rastreia x's
        q = a / m;       // Cociente da divisão
        t = m;           // Salva m atual
        m = a % m;       // Novo resto
        a = t;           // Atualiza a para m anterior

        // Atualiza coeficientes: x's seguem a recursão x = x_prev - q * x_curr
        t = x0;          // Troca temporária
        x0 = x1 - q * x0;  // Novo x0
        x1 = t;          // x1 vira o anterior
    }
    // Agora, a é o MDC, x1 é o coeficiente para o original a (inverso se MDC=1)
    if (x1 < 0) {        // Inverso deve ser positivo
        x1 += m0;        // Ajusta: x1 + k*m ≡ x1 mod m, escolhe k=1 para positivo
    }
    // Print didático: Explica o resultado
    printf("\nSubstituindo, temos que o inverso de %d em %d é %d.\n\n", A, B, x1);
    return x1;           // Retorna o inverso
}

/*
 * Função: powMod
 * 
 * Calcula base^exp mod mod usando Exponenciação Binária (Quadrado-e-Multiplicar).
 * Didática: Otimiza potências grandes evitando overflow (multiplica mod mod sempre).
 * Não usa Fermat/Euler aqui — só binária pura (O(log exp) multiplicações).
 * 
 * Parâmetros:
 * - base: Base da potência.
 * - exp: Expoente (x no main).
 * - mod: Módulo (n1 no main).
 * 
 * Retorno: (base^exp) % mod como int.
 * 
 * Exemplo: 6^10 mod 13 → Usa bits de 10 (1010 binário) para quadrados seletivos.
 */
#include <stdio.h>  // Biblioteca padrão para entrada/saída (printf, scanf)

/*
 * Programa para Cálculo de Divisão Modular e Potência Modular
 * 
 * Este programa demonstra conceitos de aritmética modular:
 * 1. Cálculo do MDC (Maior Divisor Comum) via Euclides, com passos visíveis.
 * 2. Inverso modular usando o algoritmo estendido de Euclides.
 * 3. Divisão modular: H / G mod Zn = H * (G^{-1}) mod Zn.
 * 4. Potência modular: a^x mod n1, usando exponenciação binária (otimizada).
 * 
 * Conceitos chave:
 * - MDC: Útil para verificar se inverso existe (só se MDC(G, Zn) = 1).
 * - Inverso modular: Número i tal que G * i ≡ 1 mod Zn.
 * - Potência modular: Evita overflow com % mod a cada passo.
 * 
 * Entrada: H, G, Zn, x, n1 (via scanf).
 * Saída: Passos do MDC, inverso, divisão e potência final.
 * 
 * Dica didática: Rode com H=7, G=3, Zn=11, x=10, n1=13 para ver o exemplo.
 * Saída esperada: Inverso=4, a=6, resultado=4.
 */

// Suporte para Windows: Define codificação UTF-8 no console (opcional, para acentos)
#ifdef _WIN32
#include <windows.h>  // Biblioteca Windows para SetConsoleOutputCP
#endif

/*
 * Função: mdcComPassos
 * 
 * Calcula o MDC de a e b usando o Algoritmo de Euclides (iterativo).
 * Didática: Mostra cada passo da divisão (modulo) para entender como o resto
 * diminui até 0. O MDC é o último não-zero (propriedade: MDC(a, b) = MDC(b, a%b)).
 * 
 * Parâmetros:
 * - a, b: Números inteiros positivos (assume a >= b para simplicidade).
 * 
 * Retorno: O MDC de a e b.
 * 
 * Exemplo: MDC(3,11) → 11 mod 3=2, 3 mod 2=1, 2 mod 1=0 → MDC=1.
 */
int mdcComPassos(int a, int b) {
    int resto;  // Variável temporária para armazenar o resto da divisão
    while (b != 0) {  // Loop continua enquanto o divisor não for 0 (condição de parada do Euclides)
        resto = a % b;  // Calcula o resto: a = q*b + resto, com 0 <= resto < b
        printf("Algoritmo de Euclides: %d mod %d = %d\n", a, b, resto);  // Mostra o passo (didático!)
        a = b;          // Atualiza: agora MDC(a, b) = MDC(b, resto)
        b = resto;      // Próximo divisor é o resto anterior
    }
    return a;           // Quando b=0, a é o MDC
}

/*
 * Função: inversoModular
 * 
 * Calcula o inverso modular de a mod m usando o Algoritmo Estendido de Euclides.
 * Didática: Além do MDC, constrói coeficientes x e y tal que a*x + m*y = MDC.
 * Se MDC=1, x é o inverso (a*x ≡ 1 mod m). Assume coprimos (MDC=1); senão, falha.
 * 
 * Parâmetros:
 * - a: Número base (o que queremos inverter).
 * - m: Módulo (Zn no main).
 * 
 * Retorno: O inverso i tal que a*i ≡ 1 mod m (positivo, [0, m-1]).
 * 
 * Exemplo: Inverso de 3 mod 11 → 4 (3*4=12 ≡ 1 mod 11).
 * Passos: Chama mdcComPassos para mostrar Euclides, depois constrói x1.
 */
int inversoModular(int a, int m) {
    int m0 = m, t, q;  // m0: cópia de m (para ajuste final); t,q: temporários
    int x0 = 0, x1 = 1;  // Coeficientes iniciais de Bézout: x0 para m, x1 para a
    int A = a, B = m;    // Cópias para print (valores originais)

    mdcComPassos(a, m);  // Mostra passos do Euclides simples (MDC)

    // Loop principal: Estende Euclides para achar coeficientes (back-substitution)
    while (m != 0) {     // Similar ao Euclides, mas rastreia x's
        q = a / m;       // Cociente da divisão
        t = m;           // Salva m atual
        m = a % m;       // Novo resto
        a = t;           // Atualiza a para m anterior

        // Atualiza coeficientes: x's seguem a recursão x = x_prev - q * x_curr
        t = x0;          // Troca temporária
        x0 = x1 - q * x0;  // Novo x0
        x1 = t;          // x1 vira o anterior
    }
    // Agora, a é o MDC, x1 é o coeficiente para o original a (inverso se MDC=1)
    if (x1 < 0) {        // Inverso deve ser positivo
        x1 += m0;        // Ajusta: x1 + k*m ≡ x1 mod m, escolhe k=1 para positivo
    }
    // Print didático: Explica o resultado
    printf("\nSubstituindo, temos que o inverso de %d em %d é %d.\n\n", A, B, x1);
    return x1;           // Retorna o inverso
}

/*
 * Função: powMod
 * 
 * Calcula base^exp mod mod usando Exponenciação Binária (Quadrado-e-Multiplicar).
 * Didática: Otimiza potências grandes evitando overflow (multiplica mod mod sempre).
 * Não usa Fermat/Euler aqui — só binária pura (O(log exp) multiplicações).
 * 
 * Parâmetros:
 * - base: Base da potência.
 * - exp: Expoente (x no main).
 * - mod: Módulo (n1 no main).
 * 
 * Retorno: (base^exp) % mod como int.
 * 
 * Exemplo: 6^10 mod 13 → Usa bits de 10 (1010 binário) para quadrados seletivos.
 */
int powMod(int base, int exp, int mod) {
    long long res = 1;   // Resultado inicial (qualquer coisa^0 = 1)
    long long b = base % mod;  // Base inicial mod mod (reduz logo de cara)
    while (exp > 0) {    // Loop até expoente zerar (binário: divide por 2)
        if (exp % 2 == 1) {  // Se bit atual é 1 (ímpar), multiplica no resultado
            res = (res * b) % mod;  // % mod evita overflow (long long ajuda)
        }
        b = (b * b) % mod;   // Sempre quadrado a base (prepara próximo bit)
        exp >>= 1;           // Divide exp por 2 (shift right, próximo bit)
    }
    return (int)res;     // Cast para int (seguro, pois < mod)
}

/*
 * Função: main
 * 
 * Ponto de entrada: Lê inputs, chama funções e mostra resultados.
 * Didática: Demonstra fluxo completo: inverso → divisão modular → potência.
 * Suporte Windows para UTF-8 (acentos em prints).
 * 
 * Fluxo:
 * 1. Lê H, G, Zn, x, n1.
 * 2. Calcula inverso de G mod Zn.
 * 3. a = H * inverso mod Zn (divisão modular).
 * 4. resultado = a^x mod n1.
 * 5. Prints explicativos.
 */
int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);  // Configura console para UTF-8 (Windows)
#endif

    int H, G, Zn, x, n1;  // Variáveis de entrada: H (dividendo), G (divisor), Zn (mod div), x (exp), n1 (mod pot)

    // Leitura interativa com prompts (didático para usuário)
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
    printf("\n");  // Quebra de linha para clareza

    // Passo 1: Calcula inverso de G mod Zn (chama função, que mostra passos)
    int inverso = inversoModular(G, Zn);

    // Passo 2: Divisão modular: H / G = H * G^{-1} mod Zn
    int a = (H * inverso) % Zn;  // Multiplicação seguida de % Zn (resíduo)

    // Prints didáticos para o passo da divisão
    printf("Fazendo a multiplicação modular: %d * %d mod %d === %d\n", H, inverso, Zn, a);
    printf(" Sendo %d o inverso de %d.\n", inverso, G);

    // Passo 3: Potência modular: a^x mod n1
    int resultado = powMod(a, x, n1);  // Chama função otimizada

    // Print final: Resultado da congruência completa
    printf("Valor final da congruência: %d\n", resultado);

    return 0;  // Sucesso: Programa termina sem erros
}

/*
==========
RESPOSTAS:
==========

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










