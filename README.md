# Métodos de resolução para o Problema do Caixeiro Viajante

Este repositório contém as implementações do meu projeto de Iniciação Científica desenvolvido no Instituto de Ciências Matemáticas e de Computação (ICMC-USP), sob a orientação da professora Marina Andretta.

## Sobre o Projeto
O objetivo central é estudar e implementar métodos de resolução para o Problema do Caixeiro Viajante (PCV), comparando seus desempenhos em instâncias da literatura. O projeto abrange:

* **Modelos Exatos:** Formulações clássicas de Otimização Linear Inteira Mista, com ênfase nos modelos DFJ e MTZ, resolvidos via solvers.
* **Heurísticas:** Implementação de heurísticas construtivas, como o Vizinho Mais Próximo, e de melhoramento, como a técnica de busca local 2-Opt.
* **Algoritmos de Aproximação:** Implementação de um algoritmo de 2-aproximação para o PCV métrico, baseado em Árvore Geradora Mínima e Circuito Euleriano.

## Metodologia e Avaliação
Os algoritmos são avaliados com base no tempo computacional e na qualidade da solução. O desempenho das heurísticas e do algoritmo de aproximação é medido através do cálculo do gap percentual de erro em relação à solução ótima exata fornecida pelo solver.

## Estrutura do Repositório
* `/src`: Arquivos de código fonte em C++.
* `/instances`: Matrizes de teste com complexidade variada.

## Como compilar e executar (Linux)
```bash
g++ src/main.cpp -o main
./main

