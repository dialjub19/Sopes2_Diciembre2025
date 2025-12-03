#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void* tortuga(void* arg) {
    char* nombre = (char*)arg;
    int avance = 0;

    while (avance < 20) {
        avance += rand() % 3;
        printf("%s avanzó a %d\n", nombre, avance);
        usleep(500000);
    }

    printf("🐢 %s llegó a la meta!\n", nombre);
    return NULL;
}

int main() {
    pthread_t t1, t2;

    srand(time(NULL));

    pthread_create(&t1, NULL, tortuga, "Tortuga A");
    pthread_create(&t2, NULL, tortuga, "Tortuga B");

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("🏁 Carrera terminada\n");
    return 0;
}


/* 
int pthread_create(
    pthread_t *thread,        // identificador del hilo
    const pthread_attr_t *attr, // atributos (normalmente NULL)
    void *(*start_routine)(void *), // función que ejecuta el hilo
    void *arg                 // argumento que se pasa a la función
);
*/