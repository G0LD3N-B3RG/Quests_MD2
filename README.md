# Projeto: Soluções de Questões de Matemática Discreta e Criptografia

## Autor
- **Nome**: Gabriel Goldenberg Moita
- **Matrícula**: 211061752

## Descrição Geral
Este repositório contém soluções implementadas em C para questões de disciplinas relacionadas a Matemática Discreta, Teoria dos Números e Criptografia. Cada questão é resolvida com código funcional, comentários didáticos e validações para facilitar a execução e compreensão. O foco é em algoritmos como MDC, inverso modular, potência modular e implementações RSA com Pollard Rho.

Para compilar e executar:
- Use GCC: `gcc -o programa arquivo.c`
- Execute: `./programa`
- Testes incluem exemplos hardcoded para verificação.

## Estrutura do Repositório
- **quest1.c**: [Breve descrição, se aplicável – ex: Chaves Periódicas com MMC]
- **quest2.c**: [Breve descrição]
- **quest3.c**: [Breve descrição – ex: Razão de Eficiência com fatoração prima]
- **quest4/**: Diretório para a Questão 4 (bônus), dividida em duas partes para modularidade:
  - **quest4_1.c**: Implementa divisão modular (H ≡ G mod Zn) usando inverso modular via Euclides estendido. Inclui passos didáticos do MDC e prints de cálculos.
  - **quest4_2.c**: Computa a^x mod n1 com potência modular otimizada (binária). Integra com quest4_1 para fluxo completo, aplicando Fermat/Euler implicitamente via redução (se coprimo).
- **Outros arquivos**: README.md (este), testes ou exemplos.

## Questão 4: Detalhes da Divisão
A **Questão 4 (bônus – 0,5 pontos)** é dividida em **quest4_1** e **quest4_2** para separar responsabilidades e facilitar depuração:
- **quest4_1**: Foca na divisão modular H ≡ G (mod Zn). Calcula o inverso de G mod Zn (se existe, i.e., MDC=1) e multiplica por H, mostrando passos do Euclides.
- **quest4_2**: Aplica potência modular no resultado (a^x mod n1), usando exponenciação binária para eficiência. Não aplica Fermat/Euler explicitamente (sem redução de expoente), mas o método é compatível.

Exemplo de uso (Questão 4):  
Entrada: H=7, G=3, Zn=11, x=10, n1=13  
Saída esperada: Inverso=4, a=6 (7/3 mod 11), resultado=4 (6^10 mod 13=4).

## Como Contribuir ou Testar
1. Clone o repo: `git clone [URL]`
2. Compile arquivos individuais.
3. Rode com inputs de teste para validar.
4. Issues/PRs bem-vindos para melhorias!

## Licença
MIT License – Livre para uso educacional.