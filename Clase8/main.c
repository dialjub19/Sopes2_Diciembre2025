#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <linux/unistd.h>
#include <stdbool.h>

#define MY_ENCRYPT 548

void encryptAnalizer(){
    char file_input[256] = {0}, file_output[256] = {0}, key[256] = {0};
    int threads_numbers = 0;
    char command[256];
    bool run = true;

    while(run){
        printf("\nIngrese un parametro (-p, -o, -k, -j o run para ejecutar): ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "-p") == 0) {
            printf("Archivo de entrada: ");
            fgets(file_input, sizeof(file_input), stdin);
            file_input[strcspn(file_input, "\n")] = 0;

        } else if (strcmp(command, "-o") == 0) {
            printf("Archivo de salida: ");
            fgets(file_output, sizeof(file_output), stdin);
            file_output[strcspn(file_output, "\n")] = 0;

        } else if (strcmp(command, "-k") == 0) {
            printf("Clave: ");
            fgets(key, sizeof(key), stdin);
            key[strcspn(key, "\n")] = 0;

        } else if (strcmp(command, "-j") == 0) {
            printf("Número de hilos: ");
            scanf("%d", &threads_numbers);
            getchar();

        } else if (strcmp(command, "run") == 0) {

            if (strlen(file_input) == 0 || strlen(file_output) == 0 || strlen(key) == 0 || threads_numbers == 0) {
                printf("\nFaltan parametros obligatorios ...\n");
                continue;
            }

            long result = syscall(MY_ENCRYPT, file_input, file_output, key, threads_numbers);
            if (result >= 0)
                printf("Archivo encriptado exitosamente\n");
            else
                printf("Ocurrió un error\n");

            return;  
        } else {
            printf("Comando no reconocido\n");
        }
    }
}

void analizer() {
    char command[256];
    bool run = true;

    while (run) {
        printf("*****************************************************************\n");
        printf("\n****  Multithreading # Encrypt  ****\n");
        printf("1. Encriptar\n");
        printf("2. Salir\n");
        printf("*****************************************************************\n");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "1") == 0) {
            encryptAnalizer();
        } else if (strcmp(command, "2") == 0) {
            printf("Hasta luego !\n");
            return;
        }else {
            printf("Comando - %s - no reconocido, vuelva a intentarlo", command);
        }
    }
}

int main() {
    analizer();
    return 0;
}