#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fields.h"
#include "dllist.h"

typedef struct {
    char ch;
    int count;
} OperationResult;

void yaz(char ch, int count, Dllist *output);
void sil(char ch, int count, Dllist *output);

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Kullanım: %s <giriş_dosyası> <çıkış_dosyası>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    FILE *output_file = fopen(argv[2], "w");

    if (input_file == NULL || output_file == NULL) {
        perror("Dosya açılamadı");
        return 1;
    }

    IS is = new_inputstruct(argv[1]);
    Dllist output = new_dllist();

    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, input_file)) != -1) {
        char *token;
        token = strtok(line, " ");

        while (token != NULL) {
            if (strncmp(token, "yaz:", 4) == 0) {
    token = strtok(NULL, " ");
    while (token != NULL) {
        int count = atoi(token);
        token = strtok(NULL, " ");
        char ch;
        if (strcmp(token, "\\b") == 0) {
            ch = ' ';
        } else if (strcmp(token, "\\n") == 0) {
            ch = '\n';
        } else {
            ch = token[0];
        }
        yaz(ch, count, &output);
        token = strtok(NULL, " ");
    }

            } else if (strncmp(token, "sil:", 4) == 0) {
                token = strtok(NULL, " ");
                if (token != NULL) {
                    int sil_count = atoi(token); // Silinecek adet
                    token = strtok(NULL, " ");
                    if (token != NULL) {
                        char sil_ch = token[0]; // Silinecek karakter
                        // Silme işlemi
                        sil(sil_ch, sil_count, &output);
                    }
                }
            }
            token = strtok(NULL, " ");
        }
    }

    // Write the output to the output file
    Dllist node;
    fprintf(output_file, "Output: ");
    dll_traverse(node, output) {
        OperationResult *result = (OperationResult *)jval_v(node->val);
        for (int i = 0; i < result->count; i++) {
            fprintf(output_file, "%c", result->ch);
        }
    }
    fprintf(output_file, "\n");

    // Print the output to the console
    printf("Output: ");
    dll_traverse(node, output) {
        OperationResult *result = (OperationResult *)jval_v(node->val);
        for (int i = 0; i < result->count; i++) {
            printf("%c", result->ch);
        }
    }
    printf("\n");

    // Free allocated memory and close files
    jettison_inputstruct(is);
    fclose(input_file);
    fclose(output_file);
    free_dllist(output);
    if (line)
        free(line);

    return 0;
}

void yaz(char ch, int count, Dllist *output) {
    OperationResult *result = (OperationResult *)malloc(sizeof(OperationResult));
    result->ch = ch;
    result->count = count;
    dll_append(*output, new_jval_v((void *)result));
}

void sil(char ch, int count, Dllist *output) {
    // Silme işlemi için karakter ve adet kontrolü yapılacak
    if (count <= 0) {
        printf("Geçersiz silme adedi: %d\n", count);
        return; // Geçersiz silme adedi, işlem yapma
    }


    // Silme işlemi için karakterlerin ve adetlerin bulunduğu çıktı listesini oluşturalım
    Dllist node = dll_last(*output); // Son düğümden başla

    // Düğümü sona git ve geriye doğru tarayarak istenilen karakteri bul
    while (node != NULL) {
        OperationResult *result = (OperationResult *)jval_v(node->val);
        if (result->ch == ch) {
            if (result->count <= count) {
                // Karakterin tamamı silinecek, düğümü çıkart
                Dllist temp = node;
                node = node->blink; // Geri git
                dll_delete_node(temp); // Düğümü sil
                free(result); // Belleği serbest bırak
            } else {
                // Belirli bir sayıda karakter silinecek
                result->count -= count;
            }
            return;
        }
        node = node->blink; // Önceki düğüme ilerle
    }
    printf("Karakter '%c' bulunamadı\n", ch);
}
