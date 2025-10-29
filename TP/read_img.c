#include <stdio.h>
#include <stdlib.h>
#include <math.h> // Para a fórmula de grayscale (precisa linkar com -lm)

// Define as implementações (faça isso em APENAS UM arquivo .c)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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
        *pg++ = gray; // Salva o pixel cinza e avança o ponteiro
    }

    //4. 

    // Salvar a nova imagem
    // stbi_write_png(nome, largura, altura, canais_da_nova_imagem, dados, stride_em_bytes)
    if (stbi_write_png("imagem_cinza.png", width, height, 1, gray_img, width)) {
        printf("Imagem em escala de cinza salva com sucesso!\n");
    } else {
        printf("Erro ao salvar a imagem.\n");
    }

    // Liberar a memória
    stbi_image_free(img);
    free(gray_img);

    return 0;
}