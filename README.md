# Sistema de Análise de Dados de Vendas em C

Este projeto foi desenvolvido em Linguagem C com o objetivo de processar e analisar dados de vendas a partir de arquivos externos, simulando um cenário simples de controle e análise de informações comerciais.

O sistema realiza a leitura de arquivos contendo dados de vendas e produtos, permitindo calcular faturamento, identificar produtos mais vendidos, realizar buscas e aplicar diferentes algoritmos de ordenação.

## Funcionalidades

* Leitura de dados a partir de arquivos externos
* Cadastro e processamento de vendas
* Busca linear
* Busca binária
* Ordenação por ID utilizando diferentes algoritmos:

  * Bubble Sort
  * Insertion Sort
  * Selection Sort
  * Quick Sort
  * Merge Sort
* Cálculo do faturamento total
* Identificação do produto mais vendido
* Menu interativo para navegação do usuário
* Tratamento de erros na abertura dos arquivos

## Tecnologias utilizadas

* Linguagem C
* Manipulação de arquivos
* Algoritmos de busca
* Algoritmos de ordenação
* Estruturas de dados com arrays
* Programação modular

## Estrutura dos arquivos
```text
sistema-analise-vendas-c/
│
├── main.c
├── vendas.txt
├── produtos.csv
└── README.md
```
## Arquivos de entrada

O sistema utiliza dois arquivos principais:

### vendas.txt

Arquivo responsável por armazenar os dados das vendas.

Exemplo de estrutura:

```text
1 101 2 49.90
2 102 1 89.90
3 103 5 19.90
```
Cada linha representa uma venda contendo:

ID da venda | ID do produto | Quantidade | Preço unitário

### produtos.csv

Arquivo responsável por armazenar o cadastro dos produtos.

Exemplo de estrutura:

```text
101,Teclado Mecânico
102,Mouse Gamer
103,Cabo HDMI
```

Cada linha representa um produto contendo:

```text
ID do produto | Descrição do produto
```

## Como executar o projeto

### Opção 1: Compilando pelo terminal com GCC

1. Certifique-se de ter o compilador GCC instalado.

2. Abra o terminal na pasta do projeto.

3. Compile o arquivo principal:

```bash
gcc main.c -o sistema
```

4. Execute o programa:

No Windows:

sistema.exe

No Linux ou macOS:

./sistema

## Observação importante

Os arquivos vendas.txt e produtos.csv devem estar na mesma pasta do arquivo executável para que o programa consiga realizar a leitura corretamente.

Caso algum arquivo não seja encontrado, o sistema exibirá uma mensagem de erro informando que não foi possível abrir o arquivo.

## Conceitos aplicados

Durante o desenvolvimento deste projeto, foram aplicados conceitos importantes de programação, como:

* Lógica de programação
* Manipulação de arquivos em C
* Arrays
* Funções
* Modularização de código
* Busca linear e binária
* Algoritmos clássicos de ordenação
* Tratamento de erros

## Objetivo acadêmico

O projeto foi desenvolvido como parte dos meus estudos em Ciência da Computação, com foco em consolidar conhecimentos fundamentais de algoritmos, estruturas de dados e programação em Linguagem C.

Além de implementar as funcionalidades propostas, o objetivo principal foi compreender a lógica por trás de cada etapa do sistema, desde a leitura dos dados até a aplicação dos algoritmos de busca e ordenação.

## Autor

Fernando Galante

Estudante de Ciência da Computação, em constante evolução na área de tecnologia e desenvolvimento de software.
