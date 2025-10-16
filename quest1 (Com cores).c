#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ============================================================================
   SISTEMA RSA COMPLETO COM POLLARD RHO
   ALUNO: [Gabriel Goldenberg Moita]
   MATRÍCULA: [211061752]
   ============================================================================ */

/* ============================================================================
   DECISÃO 1: MDC EUCLIDES - CORTE E RESTO
   JUSTIFICATIVA: Algoritmo clássico O(log n) - eficiente para RSA
   MÓDULO: Usado em: (1) Teorema Euler, (2) Inverso, (3) Pollard Rho
   ============================================================================ */
long my_gcd(long a, long b) {
    long original_a = a, original_b = b;
    printf("\n\033[33m\033[1m--- Calculando MDC(%ld, %ld) ---\033[0m\n", original_a, original_b);
    a = labs(a); b = labs(b);  // DECISÃO: Valores absolutos (teorema)
    while (b != 0) {
        long q = a / b;  // COCIENTE
        long r = a % b;  // RESTO (MÓDULO!)
        printf("   \033[36m\033[1m%4ld\033[0m = \033[32m\033[1m%2ld\033[0m x \033[36m\033[1m%4ld\033[0m + \033[36m\033[1m%4ld\033[0m\n", a, q, b, r);
        a = b;
        b = r;
    }
    printf("\033[32m\033[1m   MDC = %ld\033[0m\n\n", a);
    return a;
}

/* ============================================================================
   DECISÃO 2: TESTE DE PRIMALIDADE OPTIMIZADO
   JUSTIFICATIVA: Miller-Rabin simplificado + 6k±1 = O(√n) rápido
   MÓDULO: Elimina 2,3 e pares - foca candidatos primos
   ============================================================================ */
int is_prime(long num) {
    if (num <= 1) return 0;
    if (num <= 3) return 1;
    if (num % 2 == 0 || num % 3 == 0) return 0;  // MÓDULO 2,3
    for (long i = 5; i * i <= num; i += 6) {     // DECISÃO: Pula 6k±1
        if (num % i == 0 || num % (i + 2) == 0) return 0;
    }
    return 1;
}

/* ============================================================================
   DECISÃO 3: INVERSO MODULAR (CHAVE PRIVADA)
   JUSTIFICATIVA: Euclides Estendido = único inverso mod φ(n)
   MÓDULO: Construção: ax ≡ 1 (mod m) → d*e ≡ 1 (mod φ)
   ============================================================================ */
long find_inverse(long a, long m) {
    printf("\n\033[33m\033[1m--- Algoritmo Estendido de Euclides ---\033[0m\n");
    printf("   Inverso de \033[36m\033[1m%ld\033[0m mod \033[36m\033[1m%ld\033[0m\n", a, m);
    printf("   q    a     m\n");
    printf("   -- -------- --------\n");
    
    long m0 = m, y = 0, x = 1;
    if (m == 1) return 0;
    while (a > 1) {
        long q = a / m;           // COCIENTE
        long t = m;               // TROCA
        m = a % m;                // MÓDULO
        a = t;
        printf("   \033[32m\033[1m%2ld\033[0m  \033[36m\033[1m%4ld\033[0m  \033[36m\033[1m%4ld\033[0m\n", q, a, m);
        t = y;
        y = x - q * y;            // DECISÃO: Coeficientes Bezout
        x = t;
    }
    if (x < 0) x += m0;           // POSITIVO MOD φ
    printf("\n\033[32m\033[1m   INVERSO = %ld\033[0m\n\n", x);
    return x;
}

/* ============================================================================
   DECISÃO 4: CODIFICAÇÃO BASE-36 SIMPLIFICADA
   JUSTIFICATIVA: A-Z (11-36) + ESPAÇO(0) = alfabeto completo
   MÓDULO: Não afeta - só mapeamento
   ============================================================================ */
char num_to_char(int num) {
    if (num == 0) return ' ';                    // DECISÃO: 00 = ESPAÇO
    if (num >= 11 && num <= 36) return 'A' + (num - 11);
    return '?';
}

/* ============================================================================
   DECISÃO 5: EXPONENCIAÇÃO MODULAR (CORE RSA!)
   JUSTIFICATIVA: Quadrado-e-Multiplicar = O(log e) vs O(e)
   MÓDULO: CRUCIAL! Evita overflow: (a*b) mod n
   TEOREMA ESCOLHIDO:
     - FERMAT: Se n primo, a^(n-1) ≡ 1 (mod n)
     - EULER: Se MDC(a,n)=1, a^φ(n) ≡ 1 (mod n) ← USADO RSA!
   ============================================================================ */
long mod_pow(long base, long exp, long mod, long phi) {
    printf("\n\033[33m\033[1m--- %ld^%ld mod %ld ---\033[0m\n", base, exp, mod);
    
    const char* theorem = "";
    long reduced_exp = exp;
    long gcd_base_mod = my_gcd(base, mod);
    
    // DECISÃO: EULER PADRÃO RSA (n composto!)
    if (gcd_base_mod == 1) {
        theorem = "EULER";                    // JUSTIFICATIVA: RSA usa φ(n)
        reduced_exp %= phi;                   // exp = exp mod φ(n)
    } else {
        theorem = "Euclides";                 // Sem redução
    }
    
    printf("   Teorema: \033[32m\033[1m%s\033[0m", theorem);
    if (reduced_exp != exp) printf(" (exp: \033[36m\033[1m%ld\033[0m -> \033[36m\033[1m%ld\033[0m)", exp, reduced_exp);
    printf("\n");
    printf("   exp  res   base\n");
    printf("   --- ------ ------\n");
    
    long result = 1;
    base = base % mod;                        // MÓDULO INICIAL
    printf("   \033[36m\033[1m%3ld\033[0m  \033[36m\033[1m%4ld\033[0m  \033[36m\033[1m%4ld\033[0m\n", reduced_exp, result, base);
    
    while (reduced_exp > 0) {
        if (reduced_exp % 2 == 1) 
            result = (result * base) % mod;   // MÓDULO A CADA PASSO!
        base = (base * base) % mod;           // QUADRADO MODULAR
        reduced_exp /= 2;
        printf("   \033[36m\033[1m%3ld\033[0m  \033[36m\033[1m%4ld\033[0m  \033[36m\033[1m%4ld\033[0m\n", reduced_exp, result, base);
    }
    
    printf("\n\033[32m\033[1m   RESULTADO = %ld (%c)\033[0m\n\n", result, num_to_char(result));
    return result;
}

/* ============================================================================
   DECISÃO 6: POLLARD RHO - FATORIZAÇÃO RÁPIDA
   JUSTIFICATIVA: O(√p) esperado vs O(√n) teste completo
   MÓDULO: Detecta ciclo: gcd(|x-y|, n) = p
   ============================================================================ */
long pollard_rho(long n) {
    printf("\n\033[33m\033[1m--- Fatoracao rho de Pollard para %ld ---\033[0m\n", n);
    if (n % 2 == 0) return 2;  // DECISÃO: Par trivial
    
    long x = 2, y = 2, d = 1;
    int iter = 0;
    printf("   Iter  x     y     d\n");
    printf("   --- ------ ------ ----\n");
    
    while (d == 1) {
        iter++;
        x = (x * x + 1) % n;          // PASSO LENTO (MÓDULO!)
        y = (y * y + 1) % n;          // PASSO RÁPIDO 1
        y = (y * y + 1) % n;          // PASSO RÁPIDO 2
        long diff = labs(x - y);
        d = my_gcd(diff, n);          // MÓDULO DETECTA CICLO!
    }
    printf("\n\033[32m\033[1m   FATOR = %ld\033[0m\n\n", d);
    return d;
}

// ============================================================================
// INTERFACE E MAIN COM VALIDAÇÕES EXATAS
// ============================================================================

void print_menu() {
    printf("\n");
    printf("\033[33m\033[1m================================================\033[0m\n");
    printf("\033[33m\033[1m    SISTEMA RSA COMPLETO COM POLLARD RHO\033[0m\n");
    printf("\033[33m\033[1m================================================\033[0m\n");
    printf("ESCOLHA UMA OPCAO:\n");
    printf("  1 - TESTE RAPIDO (391 323 HELLO)\n");
    printf("  2 - DIGITAR MANUAL\n");
    printf("  3 - AJUDA - Regras\n");
    printf("  0 - SAIR\n");
    printf("\033[33m\033[1m================================================\033[0m\n");
    printf("Digite: ");
}

void print_help() {
    printf("\nREGRAS RSA SIMPLES:\n");
    printf("\033[33m\033[1m========================================\033[0m\n");
    printf("\033[32m\033[1mOK: N1, N2 entre 100 e 9999\033[0m\n");
    printf("\033[32m\033[1mOK: N1, N2 COMPOSTOS (2 primos)\033[0m\n");
    printf("\033[32m\033[1mOK: Exemplo: 391 323\033[0m\n");
    printf("\033[31m\033[1mERRO: 397 323 (397 primo)\033[0m\n");
    printf("\033[33m\033[1m========================================\033[0m\n");
    printf("Pressione ENTER para voltar...");
    getchar();
}

int main() {
    /* ============================================================================
       DECISÃO 7: LOOP INFINITO + VALIDAÇÕES EXATAS
       JUSTIFICATIVA: UX Profissional - usuário NUNCA trava
       MÓDULO: Validações granulares = debug preciso
       ============================================================================ */
    while (1) {
        print_menu();
        int op;
        scanf("%d", &op);
        getchar();
        
        // VALIDAÇÃO 1: MENU
        if (op != 0 && op != 1 && op != 2 && op != 3) {
            printf("\n\033[31m\033[1m*** ERRO: Digite APENAS 0, 1, 2 ou 3! ***\033[0m\n");
            printf("Pressione ENTER para tentar novamente...\n");
            getchar();
            continue;
        }
        
        if (op == 0) break;
        if (op == 3) { print_help(); continue; }
        
        long N1, N2;
        char msg[1000];
        
        if (op == 1) {
            N1 = 391; N2 = 323;  // 23×17, 19×17
            strcpy(msg, "HELLO");
            printf("\n\033[32m\033[1m*** TESTE RAPIDO CARREGADO! ***\033[0m\n");
        } else if (op == 2) {
            printf("N1 (100-9999): "); scanf("%ld", &N1);
            printf("N2 (100-9999): "); scanf("%ld", &N2);
            getchar();
            printf("Mensagem: "); fgets(msg, sizeof(msg), stdin);
            msg[strcspn(msg, "\n")] = 0;
        }
        
        // VALIDAÇÃO 2: INTERVALO EXATO
        if (N1 < 100 || N1 > 9999) {
            printf("\n\033[31m\033[1m*** ERRO: N1 (%ld) deve estar entre 100 e 9999! ***\033[0m\n", N1);
            printf("Pressione ENTER para tentar novamente...\n"); getchar(); continue;
        }
        if (N2 < 100 || N2 > 9999) {
            printf("\n\033[31m\033[1m*** ERRO: N2 (%ld) deve estar entre 100 e 9999! ***\033[0m\n", N2);
            printf("Pressione ENTER para tentar novamente...\n"); getchar(); continue;
        }
        
        printf("\n\033[32m\033[1m*** INICIANDO RSA! ***\033[0m\n");
        printf("\033[33m\033[1m===============================================\033[0m\n");
        printf("\033[33m\033[1m             ETAPA 1: FATORACAO\033[0m\n");
        printf("\033[33m\033[1m===============================================\033[0m\n");
        
        long p = pollard_rho(N1);  // p1 de N1 = p1×q1
        long q = pollard_rho(N2);  // p2 de N2 = p2×q2
        
        // VALIDAÇÃO 3: COMPOSTO EXATO
        if (is_prime(N1)) {
            printf("\n\033[31m\033[1m*** ERRO: N1 (%ld) e PRIMO! Deve ser COMPOSTO! ***\033[0m\n", N1);
            printf("Pressione ENTER para tentar novamente...\n"); getchar(); continue;
        }
        if (is_prime(N2)) {
            printf("\n\033[31m\033[1m*** ERRO: N2 (%ld) e PRIMO! Deve ser COMPOSTO! ***\033[0m\n", N2);
            printf("Pressione ENTER para tentar novamente...\n"); getchar(); continue;
        }
        
        // VALIDAÇÃO 4: DISTINTO EXATO
        if (p == q) {
            printf("\n\033[31m\033[1m*** ERRO: n = %ld x %ld (PRIMOS IGUAIS)! ***\033[0m\n", p, q);
            printf("Pressione ENTER para tentar novamente...\n"); getchar(); continue;
        }
        
        /* ============================================================================
           RSA CORE: n = p*q, φ(n) = (p-1)(q-1)
           JUSTIFICATIVA: Teorema Euler garante inverso único
           ============================================================================ */
        printf("\033[32m\033[1mFATORES:\033[0m\n");
        printf("  N1 = \033[36m\033[1m%ld\033[0m = \033[36m\033[1m%ld\033[0m x \033[36m\033[1m%ld\033[0m\n", N1, p, N1/p);
        printf("  N2 = \033[36m\033[1m%ld\033[0m = \033[36m\033[1m%ld\033[0m x \033[36m\033[1m%ld\033[0m\n\n", N2, q, N2/q);
        
        long n = p * q;                           // MÓDULO PÚBLICO
        long phi = (p - 1) * (q - 1);             // EULER TOTIENT
        long e = 2;
        while (e < n && my_gcd(e, phi) != 1) e++; // e coprimo φ
        
        long d = find_inverse(e, phi);            // d*e ≡ 1 (mod φ)
        
        printf("\033[33m\033[1m===============================================\033[0m\n");
        printf("\033[33m\033[1m           ETAPA 2: CHAVES RSA\033[0m\n");
        printf("\033[33m\033[1m===============================================\033[0m\n");
        printf("\nCHAVES:\n");
        printf("  PUBLICA:  (n=\033[32m\033[1m%ld\033[0m, e=\033[32m\033[1m%ld\033[0m)\n", n, e);
        printf("  PRIVADA:  (n=\033[32m\033[1m%ld\033[0m, d=\033[32m\033[1m%ld\033[0m)\n\n", n, d);
        
        // CRIPTOGRAFIA: C = M^e mod n
        printf("\033[33m\033[1m===============================================\033[0m\n");
        printf("\033[33m\033[1m      ETAPA 3: CRIPTOGRAFIA\033[0m\n");
        printf("\033[33m\033[1m===============================================\033[0m\n");
        
        char num_str[2001] = {0};
        for (int i = 0; msg[i]; i++) {
            char ch = toupper(msg[i]);
            if (isspace(msg[i])) strcat(num_str, "00");
            else if (isalpha(ch)) {
                int val = 10 + (ch - 'A' + 1);  // A=11, B=12...
                char buf[3]; sprintf(buf, "%02d", val);
                strcat(num_str, buf);
            }
        }
        printf("\nPRE-CODIFICACAO:\n");
        printf("  Original:  %s\n", msg);
        printf("  Numerica:  %s\n\n", num_str);
        
        long cipher[1000];
        int num_blocks = strlen(num_str) / 2;
        printf("CRIPTOGRAFIA (%d blocos):\n", num_blocks);
        printf("  Bloco  M     C\n");
        printf("  ---- ------ -------\n");
        
        for (int i = 0; i < num_blocks; i++) {
            char block[3] = {num_str[2*i], num_str[2*i+1], 0};
            long M = atol(block);
            long C = mod_pow(M, e, n, phi);   // C = M^e mod n
            cipher[i] = C;
            printf("  %2d   \033[36m\033[1m%4ld\033[0m  \033[36m\033[1m%7ld\033[0m\n", i+1, M, C);
        }
        printf("\n");
        
        // DESCRIPTOGRAFIA: M = C^d mod n
        char dec_num_str[2001] = {0};
        printf("DESCRIPTOGRAFIA:\n");
        printf("  Bloco  C       M\n");
        printf("  ---- ------- ----\n");
        
        for (int i = 0; i < num_blocks; i++) {
            long C = cipher[i];
            long M = mod_pow(C, d, n, phi);   // M = C^d mod n
            char buf[20]; sprintf(buf, "%02ld", M);
            strcat(dec_num_str, buf);
        }
        printf("\n");
        
        char dec_msg[1001] = {0};
        for (int i = 0; i < strlen(dec_num_str)/2; i++) {
            char block[3] = {dec_num_str[2*i], dec_num_str[2*i+1], 0};
            int num = atoi(block);
            dec_msg[i] = num_to_char(num);
        }
        
        printf("RESULTADO FINAL:\n");
        printf("  Original:   \033[32m\033[1m %s \033[0m\n", msg);
        printf("  Decifrada:  \033[32m\033[1m %s \033[0m\n\n", dec_msg);
        
        char orig_processed[1001] = {0};
        for (int i = 0; msg[i]; i++) {
            char ch = toupper(msg[i]);
            if (isspace(msg[i]) || isalpha(ch)) strncat(orig_processed, &ch, 1);
        }
        
        printf("\033[33m\033[1m===============================================\033[0m\n");
        printf("\033[33m\033[1m               VERIFICACAO FINAL\033[0m\n");
        printf("\033[33m\033[1m===============================================\033[0m\n");
        if (strcmp(dec_msg, orig_processed) == 0) {
            printf("\033[32m\033[1m*** SUCESSO! Mensagem identica! ***\033[0m\n");
        } else {
            printf("\033[31m\033[1m*** ERRO na descriptografia! ***\033[0m\n");
        }
        printf("\033[33m\033[1m===============================================\033[0m\n");
        
        printf("\nPressione ENTER para MENU...");
        getchar();
    }
    printf("\n\033[32m\033[1m*** OBRIGADO! ***\033[0m\n");
    return 0;
}