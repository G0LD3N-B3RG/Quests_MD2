#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Códigos de cores ANSI */
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"
#define RESET   "\033[0m"

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
long meu_gcd(long a, long b) {
    long original_a = a, original_b = b;
    printf(CYAN "\n--- Calculando MDC(%ld, %ld) ---\n" RESET, original_a, original_b);
    a = labs(a); b = labs(b);  // DECISÃO: Valores absolutos (teorema)
    while (b != 0) {
        long q = a / b;  // COCIENTE
        long r = a % b;  // RESTO (MÓDULO!)
        printf(WHITE "   %4ld = %2ld x %4ld + %4ld\n" RESET, a, q, b, r);
        a = b;
        b = r;
    }
    printf(GREEN "\n    MDC = %ld\n\n" RESET, a);
    return a;
}

/* ============================================================================
   DECISÃO 2: TESTE DE PRIMALIDADE OPTIMIZADO
   JUSTIFICATIVA: Miller-Rabin simplificado + 6k±1 = O(√n) rápido
   MÓDULO: Elimina 2,3 e pares - foca candidatos primos
   ============================================================================ */
int e_primo(long num) {
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
long inverso_modular(long a, long m) {
    printf(CYAN "\n--- Algoritmo Estendido de Euclides ---\n\n" RESET);
    printf(YELLOW "    Inverso de %ld mod %ld\n" RESET, a, m);
    printf(WHITE "       q     a     m\n");
    printf("      --- ------- ---\n" RESET);
    
    long m0 = m, y = 0, x = 1;
    if (m == 1) return 0;
    while (a > 1) {
        long q = a / m;           // COCIENTE
        long t = m;               // TROCA
        m = a % m;                // MÓDULO
        a = t;
        printf(WHITE "      %2ld   %4ld  %4ld\n" RESET, q, a, m);
        t = y;
        y = x - q * y;            // DECISÃO: Coeficientes Bezout
        x = t;
    }
    if (x < 0) x += m0;           // POSITIVO MOD φ
    printf(GREEN "\n    INVERSO = %ld\n\n" RESET, x);
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
    printf(CYAN "\n--- %ld^%ld mod %ld ---\n" RESET, base, exp, mod);
    
    const char* theorem = "";
    long reduced_exp = exp;
    long gcd_base_mod = meu_gcd(base, mod);
    
    // DECISÃO: EULER PADRÃO RSA (n composto!)
    if (gcd_base_mod == 1) {
        theorem = "EULER";                    // JUSTIFICATIVA: RSA usa φ(n)
        reduced_exp %= phi;                   // exp = exp mod φ(n)
    } else {
        theorem = "Euclides";                 // Sem redução
    }
    
    printf(MAGENTA "   Teorema: %s" RESET, theorem);
    if (reduced_exp != exp) printf(MAGENTA " (exp: %ld -> %ld)" RESET, exp, reduced_exp);
    printf("\n");
    printf(WHITE "   exp  res   base\n");
    printf("   --- ------ ------\n" RESET);
    
    long result = 1;
    base = base % mod;                        // MÓDULO INICIAL
    printf(WHITE "   %3ld  %4ld  %4ld\n" RESET, reduced_exp, result, base);
    
    while (reduced_exp > 0) {
        if (reduced_exp % 2 == 1) 
            result = (result * base) % mod;   // MÓDULO A CADA PASSO!
        base = (base * base) % mod;           // QUADRADO MODULAR
        reduced_exp /= 2;
        printf(WHITE "   %3ld  %4ld  %4ld\n" RESET, reduced_exp, result, base);
    }
    
    printf(GREEN "\n   RESULTADO = %ld (%c)\n\n\n" RESET, result, num_to_char(result));
    return result;
}

/* ============================================================================
   DECISÃO 6: POLLARD RHO - FATORIZAÇÃO RÁPIDA
   JUSTIFICATIVA: O(√p) esperado vs O(√n) teste completo
   MÓDULO: Detecta ciclo: gcd(|x-y|, n) = p
   ============================================================================ */
long pollard_rho(long n) {
    printf(YELLOW "___________________________________________\n");
    printf("\n--- Fatoracao rho de Pollard para %ld ---\n" RESET, n);
    if (n % 2 == 0) return 2;  // DECISÃO: Par trivial
    
    long x = 2, y = 2, d = 1;
    int iter = 0;
    printf(WHITE "   Iter  x     y     d\n");
    printf("   --- ------ ------ ----\n" RESET);
    
    while (d == 1) {
        iter++;
        x = (x * x + 1) % n;          // PASSO LENTO (MÓDULO!)
        y = (y * y + 1) % n;          // PASSO RÁPIDO 1
        y = (y * y + 1) % n;          // PASSO RÁPIDO 2
        long diff = labs(x - y);
        d = meu_gcd(diff, n);          // MÓDULO DETECTA CICLO!
    }
    printf(YELLOW "    ___________\n");
    printf(GREEN "    FATOR = %ld\n\n\n" RESET, d);
    return d;
}

// ============================================================================
// INTERFACE E MAIN COM VALIDAÇÕES EXATAS
// ============================================================================

void print_menu() {
    printf("\n");
    printf(BLUE "================================================\n");
    printf("    SISTEMA RSA COMPLETO COM POLLARD RHO\n");
    printf("================================================\n" RESET);
    printf("ESCOLHA UMA OPCAO:\n");
    printf(GREEN "  1 - TESTE RAPIDO (391 323 HELLO)\n");
    printf("  2 - DIGITAR MANUAL\n");
    printf("  3 - AJUDA - Regras\n");
    printf(RED "  0 - SAIR\n" RESET);
    printf(BLUE "================================================\n" RESET);
    printf("Digite: ");
}

void print_help() {
    printf(YELLOW "\nREGRAS RSA SIMPLES:\n");
    printf("========================================\n" RESET);
    printf(GREEN "OK: N1, N2 entre 100 e 9999\n");
    printf("OK: N1, N2 COMPOSTOS (2 primos)\n");
    printf("OK: Exemplo: 391 323\n" RESET);
    printf(RED "ERRO: 397 323 (397 primo)\n" RESET);
    printf(YELLOW "========================================\n" RESET);
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
            printf(RED "\n*** ERRO: Digite APENAS 0, 1, 2 ou 3! ***\n" RESET);
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
            printf(GREEN "\n*** TESTE RAPIDO CARREGADO! ***\n" RESET);
        } else if (op == 2) {
            printf("N1 (100-9999): "); scanf("%ld", &N1);
            printf("N2 (100-9999): "); scanf("%ld", &N2);
            getchar();
            printf("Mensagem: "); fgets(msg, sizeof(msg), stdin);
            msg[strcspn(msg, "\n")] = 0;
        }
        
        // VALIDAÇÃO 2: INTERVALO EXATO
        if (N1 < 100 || N1 > 9999) {
            printf(RED "\n*** ERRO: N1 (%ld) deve estar entre 100 e 9999! ***\n" RESET, N1);
            printf("Pressione ENTER para tentar novamente...\n"); getchar(); continue;
        }
        if (N2 < 100 || N2 > 9999) {
            printf(RED "\n*** ERRO: N2 (%ld) deve estar entre 100 e 9999! ***\n" RESET, N2);
            printf("Pressione ENTER para tentar novamente...\n"); getchar(); continue;
        }
        
        printf(GREEN "\n*** INICIANDO RSA! ***\n\n" RESET);
        printf(MAGENTA "===============================================\n");
        printf("             ETAPA 1: FATORACAO\n");
        printf("===============================================\n\n" RESET);

        
        printf(CYAN "### N1 = %ld ###\n" RESET, N1);

        long p = pollard_rho(N1);  // p1 de N1 = p1×q1
        
        printf(CYAN "### N2 = %ld ###\n" RESET, N2);
        
        long q = pollard_rho(N2);  // p2 de N2 = p2×q2
        
        // VALIDAÇÃO 3: COMPOSTO EXATO
        if (e_primo(N1)) {
            printf(RED "\n*** ERRO: N1 (%ld) e PRIMO! Deve ser COMPOSTO! ***\n" RESET, N1);
            printf("Pressione ENTER para tentar novamente...\n"); getchar(); continue;
        }
        if (e_primo(N2)) {
            printf(RED "\n*** ERRO: N2 (%ld) e PRIMO! Deve ser COMPOSTO! ***\n" RESET, N2);
            printf("Pressione ENTER para tentar novamente...\n"); getchar(); continue;
        }
        
        // VALIDAÇÃO 4: DISTINTO EXATO
        if (p == q) {
            printf(RED "\n*** ERRO: n = %ld x %ld (PRIMOS IGUAIS)! ***\n" RESET, p, q);
            printf("Pressione ENTER para tentar novamente...\n"); getchar(); continue;
        }
        
        /* ============================================================================
           RSA CORE: n = p*q, φ(n) = (p-1)(q-1)
           JUSTIFICATIVA: Teorema Euler garante inverso único
           ============================================================================ */

        printf(YELLOW "__________________________\n");
        printf("FATORES:\n" RESET);
        printf(GREEN "  N1 = %ld = %ld x %ld\n", N1, p, N1/p);
        printf("  N2 = %ld = %ld x %ld\n\n" RESET, N2, q, N2/q);
        
        
        printf(MAGENTA "===============================================\n");
        printf("           ETAPA 2: CHAVES RSA\n");
        printf("===============================================\n\n" RESET);

        long n = p * q;                           // MÓDULO PÚBLICO
        long phi = (p - 1) * (q - 1);             // EULER TOTIENT

        
        printf(CYAN "--- Calculo do Modulo e Totiente:\n\n" RESET);
        printf(WHITE "    Modulo n = p x q = %ld x %ld = %ld\n", p, q, n);
        printf("    Totiente phi(n) = (p-1) x (q-1) = (%ld-1) x (%ld-1) = %ld x %ld = %ld\n" RESET, p, q, p-1, q-1, phi);

        long e = 2;
        
        printf(YELLOW "__________________________________________________________________________________________\n");
        printf("\n--- Expoente Publico E -> mdc(E, phi=%ld) = 1:\n" RESET, phi);
        

        while (e < n && meu_gcd(e, phi) != 1) e++; // e coprimo φ
        
        long d = inverso_modular(e, phi);            // d*e ≡ 1 (mod φ)

        printf(YELLOW "_____________________________\n");
        printf(GREEN "\nCHAVES:\n");
        printf("  PUBLICA:  (n=%ld, e=%ld)\n", n, e);
        printf("  PRIVADA:  (n=%ld, d=%ld)\n\n" RESET, n, d);
        
        // CRIPTOGRAFIA: C = M^e mod n
        printf(MAGENTA "===============================================\n");
        printf("      ETAPA 3: CRIPTOGRAFIA\n");
        printf("===============================================\n" RESET);
        
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
        printf(CYAN "\nPRE-CODIFICACAO:\n" RESET);
        printf(WHITE "  Original:  %s\n", msg);
        printf("  Numerica:  %s\n\n" RESET, num_str);
        
        long cipher[1000];
        int num_blocks = strlen(num_str) / 2;
        
        printf(YELLOW "___________________________________\n");
        printf("CRIPTOGRAFIA: (%d blocos):\n\n" RESET, num_blocks);
        
        
        for (int i = 0; i < num_blocks; i++) {
            char block[3] = {num_str[2*i], num_str[2*i+1], 0};
            long M = atol(block);
            printf(WHITE "(Bloco %2d) \n" RESET, i+1);
            long C = mod_pow(M, e, n, phi);   // C = M^e mod n
            cipher[i] = C;
        }
        printf("\n");
        
        // DESCRIPTOGRAFIA: M = C^d mod n
        char dec_num_str[2001] = {0};
        printf(YELLOW "___________________________________\n");
        printf("DESCRIPTOGRAFIA: (%d blocos):\n\n" RESET, num_blocks);
      
        for (int i = 0; i < num_blocks; i++) {
            long C = cipher[i];
            printf(WHITE "(Bloco %2d) \n" RESET, i+1);
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

        printf(YELLOW "___________________________________\n");
        printf("RESULTADO FINAL:\n" RESET);
        printf(WHITE "  Original:   %s\n", msg);
        printf("  Decifrada:  %s\n\n" RESET, dec_msg);
        
        char orig_processed[1001] = {0};
        for (int i = 0; msg[i]; i++) {
            char ch = toupper(msg[i]);
            if (isspace(msg[i]) || isalpha(ch)) strncat(orig_processed, &ch, 1);
        }
        
        printf(MAGENTA "===============================================\n");
        printf("               VERIFICACAO FINAL\n");
        printf("===============================================\n" RESET);
        if (strcmp(dec_msg, orig_processed) == 0) {
            printf(GREEN "      *** SUCESSO! Mensagem identica! ***\n" RESET);
        } else {
            printf(RED "       *** ERRO na descriptografia! ***\n" RESET);
        }
        printf(MAGENTA "===============================================\n" RESET);
        
        printf("\nPressione ENTER para MENU...");
        getchar();
    }
    printf(GREEN "\n*** OBRIGADO! ***\n" RESET);
    return 0;
}