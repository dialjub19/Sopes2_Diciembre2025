#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define SYS_CPU_USAGE 551


int main(){
  uptimeFunc();
  int cpu_usage = 0;
    
    // Llamamos a la syscall
    long res = syscall(SYS_CPU_USAGE, &cpu_usage);
    
    if (res == 0) {
        printf("CPU Usage: %d.%02d%%\n", cpu_usage / 100, cpu_usage % 100);
    } else {
        perror("Error en syscall");
    }
    
 
  return 0;
}