#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Para a fórmula de grayscale (precisa linkar com -lm)

// Definindo as implementações para as imagens
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define MAX 1000

typedef struct edge {
  int u, v, w;
} edge;

typedef struct edge_list {
  edge data[MAX];
  int n;
} edge_list;

edge_list elist;

int Graph[MAX][MAX], n;
edge_list spanlist;

void kruskalAlgo();
int find(int belongs[], int vertexno);
void applyUnion(int belongs[], int c1, int c2);
void sort();
void print();

// Applying Krushkal Algo
void kruskalAlgo() {
  int belongs[MAX], i, j, cno1, cno2;
  elist.n = 0;

  for (i = 1; i < n; i++)
    for (j = 0; j < i; j++) {
      if (Graph[i][j] != 0) {
        elist.data[elist.n].u = i;
        elist.data[elist.n].v = j;
        elist.data[elist.n].w = Graph[i][j];
        elist.n++;
      }
    }

  sort();

  for (i = 0; i < n; i++)
    belongs[i] = i;

  spanlist.n = 0;

  for (i = 0; i < elist.n; i++) {
    cno1 = find(belongs, elist.data[i].u);
    cno2 = find(belongs, elist.data[i].v);

    if (cno1 != cno2) {
      spanlist.data[spanlist.n] = elist.data[i];
      spanlist.n = spanlist.n + 1;
      applyUnion(belongs, cno1, cno2);
    }
  }
}

int find(int belongs[], int vertexno) {
  return (belongs[vertexno]);
}

void applyUnion(int belongs[], int c1, int c2) {
  int i;

  for (i = 0; i < n; i++)
    if (belongs[i] == c2)
      belongs[i] = c1;
}

// Sorting algo
void sort() {
  int i, j;
  edge temp;

  for (i = 1; i < elist.n; i++)
    for (j = 0; j < elist.n - 1; j++)
      if (elist.data[j].w > elist.data[j + 1].w) {
        temp = elist.data[j];
        elist.data[j] = elist.data[j + 1];
        elist.data[j + 1] = temp;
      }
}

// Printing the result
void print() {
  int i, cost = 0;

  for (i = 0; i < spanlist.n; i++) {
    printf("\n%d - %d : %d", spanlist.data[i].u, spanlist.data[i].v, spanlist.data[i].w);
    cost = cost + spanlist.data[i].w;
  }

  printf("\nSpanning tree cost: %d", cost);
}

int main() {
    int width, height, channels;

    // 1. Carregar a imagem
    // O último argumento '0' força a carregar com o número original de canais
    // Se quiser forçar 3 canais (RGB), use '3'
    unsigned char *img = stbi_load("imagem.jpg", &width, &height, &channels, 0);

    if (img == NULL) {
        printf("Erro ao carregar a imagem.\n");
        return 1;
    }
    printf("Imagem carregada: %d x %d, %d canais\n", width, height, channels);

    // 2. Alocar memória para a nova imagem (em escala de cinza, 1 canal)
    size_t img_size = width * height;
    unsigned char *gray_img = (unsigned char *)malloc(img_size);
    if (gray_img == NULL) {
        printf("Erro ao alocar memória para imagem em escala de cinza.\n");
        stbi_image_free(img);
        return 1;
    }

    // 3. Processar a Imagem (Converter para Escala de Cinza)
    // Usamos a fórmula de luminância: Y = 0.299*R + 0.587*G + 0.114*B
    unsigned char *p = img;
    unsigned char *pg = gray_img;
    unsigned char *save = pg;

    for (int i = 0; i < img_size; i++) {
        unsigned char r = *p++; // Pega o R e avança o ponteiro
        unsigned char g = *p++; // Pega o G e avança o ponteiro
        unsigned char b = *p++; // Pega o B e avança o ponteiro
        
        // Se a imagem tiver canal Alfa (transparência), pulamos ele
        if (channels == 4) {
            p++; 
        }

        // Calcula o valor de cinza
        unsigned char gray = (unsigned char)(0.299 * r + 0.587 * g + 0.114 * b);
        *pg++ = gray; // Salva o pixel cinza
    }

    // Salvar a nova imagem
    // stbi_write_png(nome, largura, altura, canais_da_nova_imagem, dados, stride_em_bytes)
    if (stbi_write_png("imagem_cinza.png", width, height, 1, gray_img, width)) {
        printf("Imagem em escala de cinza salva com sucesso!\n");
    } else {
        printf("Erro ao salvar a imagem.\n");
    }

    int u, v;

    //4. Modular o Grafo
    for (int i = 0; i<width; i++){
        u = *pg;
        pg++;
        for (int j = 0; j>height; j++){
            // Mapeando as arestas da mesma linha
            v = *pg;
            pg++;
            Graph[i][j] = u - v;
            u = v;
        }
    }

    // Liberar a memória
    stbi_image_free(img);
    free(gray_img);

    return 0;
}