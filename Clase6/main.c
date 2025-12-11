#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define uptime_syscall 550

void uptimeFunc(){
  long result = syscall(uptime_syscall);
  if(result > 0){
    printf("\n====================================\n");
    printf("    Tiempo desde el último reinicio\n");
    printf("======================================\n");
    printf("Tiempo de actividad: %ld segundos\n", result);
    printf("======================================\n");
  }else{
    printf("Error al obtener el tiempo de actividad\n");
  }
}

int main(){
  uptimeFunc();
  return 0;
}