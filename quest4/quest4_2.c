/*
QUESTÃO 4 PARTE 2 (EXTRA)
Aluno: Gabriel Goldenberg Moita
Matrícula: 211061752

==========================================
    RESPOSTAS V ou F NO quest4_VF!
==========================================

Explicação do código:
*/

#include <stdio.h> 
/*
 * Função: inversoModular
 * 
 * Propósito didático: Esta função implementa o Algoritmo Estendido de Euclides para calcular o 
 * inverso modular de 'a' módulo 'm'. Em termos simples: encontra um número 'i' tal que 
 * a * i ≡ 1 (mod m), ou seja, a * i - k * m = 1 para algum k inteiro (identidade de Bézout).
 * 
 * Pré-condição: Deve existir inverso, i.e., MDC(a, m) = 1 (coprimo). Caso contrário, retorna 
 * valor inválido (não verifica explicitamente aqui, mas assume coprimos para didática).
 * 
 * Fluxo geral (explicação passo a passo):
 * 1. Inicializa variáveis: m0 guarda m original; x0/x1 são coeficientes iniciais (x1=1 para a, x0=0 para m).
 * 2. Loop: Simula Euclides (atualiza a e m com divisão/modulo) enquanto rastreia coeficientes x (back-substitution).
 * 3. Ao final, x1 é o coeficiente de a (inverso); ajusta se negativo para [0, m-1].
 * 
 * Exemplo didático: Inverso de 3 mod 11.
 * - MDC(3,11): 11=3*3+2, 3=2*1+1, 2=1*2+0 → MDC=1.
 * - Coeficientes: Reconstrói 1 = 3*x + 11*y → x=4 (3*4=12≡1 mod 11).
 * 
 * Retorno: O inverso modular i (0 ≤ i < m).
 * 
 * Observação: Para fins didáticos, em um código completo, chame mdcComPassos(a,m) antes para mostrar MDC.
 */
int inversoModular(int a, int m) {
    int m0 = m, t, q;  /* m0: Cópia de m para ajuste final (m muda no loop).
                        * t: Temporária para trocas (evita perda de valores).
                        * q: Cociente da divisão (a / m). */
    
    int x0 = 0, x1 = 1;  /* Coeficientes de Bézout iniciais:
                          * x0: Coeficiente para m (inicia 0, pois 0*m + 1*a = a).
                          * x1: Coeficiente para a (inicia 1). 
                          * No final, a*x1 + m*x0 = MDC (esperado=1). */
    
    /* Loop principal: Estende Euclides para rastrear coeficientes.
     * Didática: Paralelo ao Euclides simples (a, m → m, a%m), mas atualiza x's 
     * via recursão: Se MDC(a, m) = MDC(m, r), então x_novo = x_prev - q * x_curr. */
    while (m != 0) {  /* Continua até m=0 (MDC encontrado em 'a'). */
        q = a / m;    /* Cociente: Parte inteira de a / m (ex: 11/3=3). */
        
        t = m;        /* Salva m atual em t (para trocar com a). */
        m = a % m;    /* Novo resto: r = a mod m (atualiza m para resto). */
        a = t;        /* a vira o divisor anterior (Euclides: MDC(a,m) = MDC(m,r)). */
        
        /* Atualiza coeficientes x: Mantém a relação a*x1 + m*x0 = constante (MDC).
         * Didática: x0_novo = x1 - q * x0 (coef. para novo 'm').
         * x1 vira o anterior (coef. para novo 'a'). */
        t = x0;       /* Salva x0 em t. */
        x0 = x1 - q * x0;  /* Novo x0: Ajuste pela recursão de Bézout. */
        x1 = t;       /* x1 = x0 anterior. */
    }
    
    /* Ao sair do loop: 'a' é o MDC, x1 é o coeficiente para o a original (inverso se MDC=1). */
    if (x1 < 0) {      /* Inverso deve ser não-negativo (convenção modular). */
        x1 += m0;      /* Ajusta: x1 + m ≡ x1 mod m (soma 1*m para positivo). 
                        * Didática: Garante 0 ≤ inverso < m. Ex: Se x1=-7 mod 11, -7+11=4. */
    }
    
    /* Print didático: Explica o resultado (em código completo, adicione printf aqui). */
    /* printf("\nSubstituindo, temos que o inverso de %d em %d é %d.\n\n", a_original, m_original, x1); */
    
    return x1;         /* Retorna o inverso modular. */
}



