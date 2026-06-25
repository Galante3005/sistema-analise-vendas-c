/*
 * Universidade Presbiteriana Mackenzie
 * Algoritmos e Programacao II - Projeto 2 
 *
 * Nome: Fernando Galante Marcal da Silva   RA: 10753524
 * Nome: Matheus Camilo Marques Messias     RA: 10753500
 * Nome: Nicolas de Sousa Brandt           RA: 10754422
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 1000

/* FUNCOES DE CARGA DE ARQUIVOS */

int carregarVendas(const char nomeArquivo[], int ids[], int produtoIds[],
                   int quantidades[], float precos[], int max)
{
    FILE *fp = fopen(nomeArquivo, "r");
    if (fp == NULL)
        return -1;

    int count = 0;
    while (count < max &&
           fscanf(fp, "%d %d %d %f",
                  &ids[count], &produtoIds[count],
                  &quantidades[count], &precos[count]) == 4)
    {
        count++;
    }

    fclose(fp);
    return count;
}

int carregarProdutos(const char nomeArquivo[], int produtoIds[],
                     char descricoes[][50], int max)
{
    FILE *fp = fopen(nomeArquivo, "r");
    if (fp == NULL)
        return -1;

    int count = 0;
    char linha[100];

    while (count < max && fgets(linha, sizeof(linha), fp) != NULL) {
        /* Remove '\n' do final, se houver */
        int len = (int)strlen(linha);
        if (len > 0 && linha[len - 1] == '\n')
            linha[len - 1] = '\0';

        /* Localiza o ';' separador */
        char *sep = strchr(linha, ';');
        if (sep == NULL)
            continue;

        *sep = '\0';                       /* termina a parte do id   */
        produtoIds[count] = atoi(linha);   /* converte id             */

        /* Copia descricao (ate 49 chars) */
        strncpy(descricoes[count], sep + 1, 49);
        descricoes[count][49] = '\0';

        count++;
    }

    fclose(fp);
    return count;
}

/* FUNCOES DE BUSCA */

int buscaLinear(int ids[], int n, int idProcurado)
{
    for (int i = 0; i < n; i++)
        if (ids[i] == idProcurado)
            return i;
    return -1;
}

int buscaBinaria(int ids[], int n, int idProcurado)
{
    int inicio = 0, fim = n - 1;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        if (ids[meio] == idProcurado)
            return meio;
        else if (ids[meio] < idProcurado)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }
    return -1;
}

/* FUNCOES AUXILIARES DE TROCA (suporte para as ordenações) */

static void trocarInt(int *a, int *b)       { int t = *a; *a = *b; *b = t; }
static void trocarFloat(float *a, float *b) { float t = *a; *a = *b; *b = t; }

static void trocarRegistros(int ids[], int produtoIds[], int quantidades[],
                             float precos[], int i, int j)
{
    trocarInt(&ids[i],         &ids[j]);
    trocarInt(&produtoIds[i],  &produtoIds[j]);
    trocarInt(&quantidades[i], &quantidades[j]);
    trocarFloat(&precos[i],    &precos[j]);
}

/* ALGORITMOS DE ORDENACAO */

void bubbleSortVendaPorId(int ids[], int produtoIds[], int quantidades[],
                          float precos[], int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - 1 - i; j++)
            if (ids[j] > ids[j + 1])
                trocarRegistros(ids, produtoIds, quantidades, precos, j, j + 1);
}

void insertionSortVendaPorId(int ids[], int produtoIds[], int quantidades[],
                             float precos[], int n)
{
    for (int i = 1; i < n; i++) {
        int   keyId  = ids[i];
        int   keyPid = produtoIds[i];
        int   keyQtd = quantidades[i];
        float keyPrc = precos[i];
        int j = i - 1;

        while (j >= 0 && ids[j] > keyId) {
            ids[j + 1]         = ids[j];
            produtoIds[j + 1]  = produtoIds[j];
            quantidades[j + 1] = quantidades[j];
            precos[j + 1]      = precos[j];
            j--;
        }

        ids[j + 1]         = keyId;
        produtoIds[j + 1]  = keyPid;
        quantidades[j + 1] = keyQtd;
        precos[j + 1]      = keyPrc;
    }
}

void selectionSortPorId(int ids[], int produtoIds[], int quantidades[],
                        float precos[], int n)
{
    for (int i = 0; i < n - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < n; j++)
            if (ids[j] < ids[minIdx])
                minIdx = j;

        if (minIdx != i)
            trocarRegistros(ids, produtoIds, quantidades, precos, i, minIdx);
    }
}

/* Quick Sort - particao com pivo no ultimo elemento */
static int particionar(int ids[], int produtoIds[], int quantidades[],
                        float precos[], int inicio, int fim)
{
    int pivo = ids[fim];
    int i    = inicio - 1;

    for (int j = inicio; j < fim; j++) {
        if (ids[j] <= pivo) {
            i++;
            trocarRegistros(ids, produtoIds, quantidades, precos, i, j);
        }
    }
    trocarRegistros(ids, produtoIds, quantidades, precos, i + 1, fim);
    return i + 1;
}

void quickSortPorId(int ids[], int produtoIds[], int quantidades[],
                    float precos[], int inicio, int fim)
{
    if (inicio < fim) {
        int p = particionar(ids, produtoIds, quantidades, precos, inicio, fim);
        quickSortPorId(ids, produtoIds, quantidades, precos, inicio, p - 1);
        quickSortPorId(ids, produtoIds, quantidades, precos, p + 1, fim);
    }
}

/* Merge Sort - funcao auxiliar de mesclagem */
static void merge(int ids[], int produtoIds[], int quantidades[],
                  float precos[], int inicio, int meio, int fim)
{
    int n1 = meio - inicio + 1;
    int n2 = fim  - meio;

    /* Arrays temporarios (necessários para a troca) */
    
    int   tIds[MAX], tPid[MAX], tQtd[MAX];
    float tPrc[MAX];

    for (int i = 0; i < n1; i++) {
        tIds[i] = ids[inicio + i];
        tPid[i] = produtoIds[inicio + i];
        tQtd[i] = quantidades[inicio + i];
        tPrc[i] = precos[inicio + i];
    }
    for (int j = 0; j < n2; j++) {
        tIds[n1 + j] = ids[meio + 1 + j];
        tPid[n1 + j] = produtoIds[meio + 1 + j];
        tQtd[n1 + j] = quantidades[meio + 1 + j];
        tPrc[n1 + j] = precos[meio + 1 + j];
    }

    int i = 0, j = n1, k = inicio;
    while (i < n1 && j < n1 + n2) {
        if (tIds[i] <= tIds[j]) {
            ids[k]         = tIds[i];
            produtoIds[k]  = tPid[i];
            quantidades[k] = tQtd[i];
            precos[k]      = tPrc[i];
            i++;
        } else {
            ids[k]         = tIds[j];
            produtoIds[k]  = tPid[j];
            quantidades[k] = tQtd[j];
            precos[k]      = tPrc[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        ids[k]         = tIds[i];
        produtoIds[k]  = tPid[i];
        quantidades[k] = tQtd[i];
        precos[k]      = tPrc[i];
        i++; k++;
    }
    while (j < n1 + n2) {
        ids[k]         = tIds[j];
        produtoIds[k]  = tPid[j];
        quantidades[k] = tQtd[j];
        precos[k]      = tPrc[j];
        j++; k++;
    }
}

void mergeSortPorId(int ids[], int produtoIds[], int quantidades[],
                    float precos[], int inicio, int fim)
{
    if (inicio < fim) {
        int meio = (inicio + fim) / 2;
        mergeSortPorId(ids, produtoIds, quantidades, precos, inicio, meio);
        mergeSortPorId(ids, produtoIds, quantidades, precos, meio + 1, fim);
        merge(ids, produtoIds, quantidades, precos, inicio, meio, fim);
    }
}

/* FUNCOES DE ANALISE (Menu do usuário) */

float calcularFaturamentoTotal(int quantidades[], float precos[], int n)
{
    float total = 0.0f;
    for (int i = 0; i < n; i++)
        total += quantidades[i] * precos[i];
    return total;
}

int encontrarProdutoMaisVendido(int produtoIds[], int quantidades[], int n)
{
    /* Coleta ids unicos */
    int unicos[MAX];
    int totais[MAX];
    int nUnicos = 0;

    for (int i = 0; i < n; i++) {
        int pid   = produtoIds[i];
        int achou = 0;
        for (int j = 0; j < nUnicos; j++) {
            if (unicos[j] == pid) {
                totais[j] += quantidades[i];
                achou = 1;
                break;
            }
        }
        if (!achou) {
            unicos[nUnicos] = pid;
            totais[nUnicos] = quantidades[i];
            nUnicos++;
        }
    }

    /* Encontra o maximo. Em caso de empate, menor id ganha */
    int melhorIdx = 0;
    for (int j = 1; j < nUnicos; j++) {
        if (totais[j] > totais[melhorIdx] ||
           (totais[j] == totais[melhorIdx] && unicos[j] < unicos[melhorIdx]))
        {
            melhorIdx = j;
        }
    }
    return unicos[melhorIdx];
}

/* FUNCOES AUXILIARES DA INTERFACE */

/* Retorna a descricao de um produto pelo id; ou "Desconhecido" */
static const char *buscarDescricao(int pid,
                                   int produtoIdsCatalogo[],
                                   char descricoes[][50],
                                   int nProdutos)
{
    for (int i = 0; i < nProdutos; i++)
        if (produtoIdsCatalogo[i] == pid)
            return descricoes[i];
    return "Desconhecido";
}

/* Imprime uma linha da tabela de vendas */
static void imprimirVenda(int i,
                          int ids[], int produtoIds[],
                          int quantidades[], float precos[],
                          int produtoIdsCatalogo[],
                          char descricoes[][50], int nProdutos)
{
    const char *desc = buscarDescricao(produtoIds[i],
                                       produtoIdsCatalogo, descricoes, nProdutos);
    float total = quantidades[i] * precos[i];
    printf("%-8d | %-7d | %-20s | %-10d | %-14.2f | %.2f\n",
           ids[i], produtoIds[i], desc, quantidades[i], precos[i], total);
}

/* MAIN (armazenando o menu do usuário) */

int main(void)
{
    /* --- Arrays de vendas --- */
    int   ids[MAX];
    int   produtoIds[MAX];
    int   quantidades[MAX];
    float precos[MAX];
    int   nVendas = 0;

    /* --- Arrays de produtos --- */
    int  produtoIdsCatalogo[MAX];
    char descricoes[MAX][50];
    int  nProdutos = 0;

    /* --- Carrega arquivos --- */
    nVendas = carregarVendas("vendas.txt", ids, produtoIds, quantidades, precos, MAX);
    if (nVendas == -1) {
        printf("Erro ao abrir o arquivo vendas.txt.\nEncerrando o programa.\n");
        return 1;
    }

    nProdutos = carregarProdutos("produtos.csv", produtoIdsCatalogo, descricoes, MAX);
    if (nProdutos == -1) {
        printf("Erro ao abrir o arquivo produtos.csv.\nEncerrando o programa.\n");
        return 1;
    }

    int ordenado = 0;   
    int opcao    = 0;

    /* ---- Loop do menu ---- */
    do {
        printf("\n===== SISTEMA DE ANALISE DE VENDAS =====\n");
        printf("1. Mostrar vendas ordenadas por ID\n");
        printf("2. Buscar venda por ID\n");
        printf("3. Calcular faturamento total\n");
        printf("4. Identificar produto mais vendido\n");
        printf("5. Sair da aplicacao\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &opcao);

        /* OPCAO 1 - Mostrar vendas ordenadas */
        if (opcao == 1) {

            if (!ordenado) {
                int alg = 0;
                printf("\nEscolha o algoritmo de ordenacao:\n");
                printf("1. Bubble Sort\n");
                printf("2. Insertion Sort\n");
                printf("3. Selection Sort\n");
                printf("4. Quick Sort\n");
                printf("5. Merge Sort\n");
                printf("\nOpcao: ");
                scanf("%d", &alg);

                switch (alg) {
                    case 1:
                        bubbleSortVendaPorId(ids, produtoIds, quantidades, precos, nVendas);
                        break;
                    case 2:
                        insertionSortVendaPorId(ids, produtoIds, quantidades, precos, nVendas);
                        break;
                    case 3:
                        selectionSortPorId(ids, produtoIds, quantidades, precos, nVendas);
                        break;
                    case 4:
                        quickSortPorId(ids, produtoIds, quantidades, precos, 0, nVendas - 1);
                        break;
                    case 5:
                        mergeSortPorId(ids, produtoIds, quantidades, precos, 0, nVendas - 1);
                        break;
                    default:
                        printf("Opcao invalida. Tente novamente.\n");
                        continue;
                }
                ordenado = 1;
            }

            printf("\nVendas ordenadas por ID:\n\n");
            printf("%-8s | %-7s | %-20s | %-10s | %-14s | %s\n",
                   "ID Venda", "Produto", "Descricao", "Quantidade",
                   "Preco Unitario", "Total");
            printf("----------------------------------------------------------------------------\n");

            for (int i = 0; i < nVendas; i++)
                imprimirVenda(i, ids, produtoIds, quantidades, precos,
                              produtoIdsCatalogo, descricoes, nProdutos);

        /* OPCAO 2 - Buscar venda por ID */
        } else if (opcao == 2) {

            int idBusca = 0;
            printf("\nDigite o ID da venda: ");
            scanf("%d", &idBusca);

            int algBusca = 0;
            printf("\nEscolha o algoritmo de busca:\n");
            printf("1. Busca linear\n");
            printf("2. Busca binaria\n");
            printf("\nOpcao: ");
            scanf("%d", &algBusca);

            int idx = -1;

            if (algBusca == 1) {
                idx = buscaLinear(ids, nVendas, idBusca);

            } else if (algBusca == 2) {
                /* Garante ordenacao antes da busca binaria */
                if (!ordenado) {
                    quickSortPorId(ids, produtoIds, quantidades, precos,
                                   0, nVendas - 1);
                    ordenado = 0;
                }
                idx = buscaBinaria(ids, nVendas, idBusca);

            } else {
                printf("Opcao invalida. Tente novamente.\n");
                continue;
            }

            if (idx == -1) {
                printf("\nVenda nao encontrada.\n");
            } else {
                const char *desc = buscarDescricao(produtoIds[idx],
                                                   produtoIdsCatalogo,
                                                   descricoes, nProdutos);
                float totalVenda = quantidades[idx] * precos[idx];

                printf("\nVenda encontrada:\n\n");
                printf("ID Venda: %d\n",        ids[idx]);
                printf("Produto: %d\n",          produtoIds[idx]);
                printf("Descricao: %s\n",        desc);
                printf("Quantidade: %d\n",       quantidades[idx]);
                printf("Preco unitario: %.2f\n", precos[idx]);
                printf("Total da venda: %.2f\n", totalVenda);
            }

        /* OPCAO 3 - Faturamento total */
        } else if (opcao == 3) {

            float fat = calcularFaturamentoTotal(quantidades, precos, nVendas);
            printf("\nFaturamento total: R$ %.2f\n", fat);

        /* OPCAO 4 - Produto mais vendido */
        } else if (opcao == 4) {

            int pidMais = encontrarProdutoMaisVendido(produtoIds, quantidades, nVendas);

            /* Calcula quantidade total desse produto */
            int qtdTotal = 0;
            for (int i = 0; i < nVendas; i++)
                if (produtoIds[i] == pidMais)
                    qtdTotal += quantidades[i];

            const char *desc = buscarDescricao(pidMais,
                                               produtoIdsCatalogo,
                                               descricoes, nProdutos);

            printf("\nProduto mais vendido:\n\n");
            printf("Produto: %d\n",                pidMais);
            printf("Descricao: %s\n",              desc);
            printf("Quantidade total vendida: %d\n", qtdTotal);

        /* OPCAO 5 - Sair */
        } else if (opcao == 5) {

            printf("\nEncerrando o programa.\n");

        } else {
            printf("\nOpcao invalida. Tente novamente.\n");
        }

    } while (opcao != 5);

    return 0;
}
