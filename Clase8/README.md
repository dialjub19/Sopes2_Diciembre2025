# Clase 7 — Documentación: Syscall de Uso de CPU

En esta clase se implementó y probó una syscall que mide el uso de CPU del sistema.

## Resumen rápido

- Objetivo: Añadir una syscall que calcula el uso de CPU durante un intervalo (100 ms) y devuelve el valor en centésimas de porcentaje (por ejemplo, 1234 = 12.34%).
- Archivo kernel: linux-6.12.61/kernel/cpu_usage.c
- Tabla de syscalls: linux-6.12.61/arch/x86/entry/syscalls/syscall_64.tbl (número `550`)
- Programa de usuario de ejemplo: `main.c` (en la raíz del proyecto)

---

## Archivos relevantes

- `linux-6.12.61/kernel/cpu_usage.c`: Implementa la lógica de la syscall `cpu_info`.
- `linux-6.12.61/arch/x86/entry/syscalls/syscall_64.tbl`: Debe contener la entrada `550 common cpu_usage_syscall cpu_usage_syscall`.
- `main.c`: Programa de usuario que invoca la syscall `550` y muestra el resultado.

### Detalles de la syscall

- Nombre en código: `cpu_info` (definida con `SYSCALL_DEFINE1`)
- Prototipo (implícito): `int cpu_info(int __user *cpu_usage_out)`
- Comportamiento: bloqueante ~100 ms para muestreo; escribe en la dirección de usuario el valor entero (centésimas de %).

### Implementación del kernel (`cpu_usage.c`)

El archivo `linux-6.12.61/kernel/cpu_usage.c` contiene:

1. **`read_cpu_times()`**: Lee los tiempos acumulados de todos los CPUs del sistema, sumando estados (usuario, sistema, idle, etc.).
2. **`get_cpu_percent_x100()`**: Toma dos muestras de CPU con intervalo de 100 ms y calcula el porcentaje de uso multiplicado por 100 (centésimas).
3. **`SYSCALL_DEFINE1(cpu_info, ...)`**: Define la syscall con un parámetro que apunta a un int en espacio de usuario.

Características:

- Usa `for_each_online_cpu()` para iterar núcleos activos.
- Accede a `kcpustat_cpu()` para estadísticas de cada núcleo.
- Valida punteros con `put_user()` para escritura segura en espacio de usuario.
- Devuelve `-EINVAL` si el puntero es NULL, `-EFAULT` si hay error al escribir.

### Entrada en `syscall_64.tbl`

El archivo debe contener:

```
550 common cpu_usage_syscall cpu_usage_syscall
```

---

## Uso desde espacio de usuario

Ejemplo de `main.c` incluido en el repositorio (usa `SYS_CPU_USAGE = 550`):

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

#define SYS_CPU_USAGE 550

int main(void) {
    int cpu_usage = 0;
    long res = syscall(SYS_CPU_USAGE, &cpu_usage);

    if (res == 0) {
        printf("CPU Usage: %d.%02d%%\n", cpu_usage / 100, cpu_usage % 100);
    } else {
        perror("Error en syscall");
        return 1;
    }

    return 0;
}
```

### Compilar y ejecutar

```bash
gcc -o cpu_usage_example main.c
./cpu_usage_example
```

### Salida esperada

```
CPU Usage: 12.34%
```

---

## Pasos para compilar e instalar el kernel (resumen)

1. **Verificar archivo fuente**: `linux-6.12.61/kernel/cpu_usage.c` debe estar presente y contener la implementación de `cpu_info`.

2. **Configurar Makefile** (si es necesario):

   ```bash
   # En linux-6.12.61/kernel/Makefile, asegúrese de incluir:
   obj-y += cpu_usage.o
   ```

3. **Confirmar entrada en tabla de syscalls**:

   ```
   # En linux-6.12.61/arch/x86/entry/syscalls/syscall_64.tbl:
   550 common cpu_usage_syscall cpu_usage_syscall
   ```

4. **Compilar el kernel** (desde directorio raíz del kernel):

   ```bash
   fakeroot make -j#nucleos
   ```

   Reemplaza `#nucleos` por el número de hilos/cores disponibles en tu CPU.

5. **Verificar compilación**:

   ```bash
   echo $?
   # Debería mostrar "0" (sin errores)
   ```

6. **Instalar módulos e instalar el kernel**:

   ```bash
   sudo make modules_install
   sudo make install
   ```

7. **Actualizar GRUB** (si es necesario):

   ```bash
   sudo update-grub
   ```

8. **Reiniciar** (si es necesario):
   ```bash
   sudo reboot
   ```

---

## Notas y recomendaciones

- El valor devuelto por la syscall está en centésimas de porcentaje: `value / 100` = porcentaje con 2 decimales.
- La syscall **bloquea ~100 ms** para medir el uso; no usarla en contextos donde ese bloqueo sea crítico.
- Si al ejecutar desde usuario se obtiene error `EFAULT` o `EINVAL`:

  - Verificar que el kernel con la syscall esté arrancado.
  - Confirmar permisos de ejecución en el binario del programa de usuario.
  - Asegurar que la dirección del puntero sea válida (no NULL).

    ## Pasos para compilar e instalar el kernel (resumen)

    1. Añadir/editar `linux-6.12.61/kernel/cpu_usage.c` (ya incluido en el repo).
    2. Asegurar que `linux-6.12.61/kernel/Makefile` compile el nuevo archivo (si aplica, añadir `obj-y += cpu_usage.o`).
    3. Confirmar la entrada en `arch/x86/entry/syscalls/syscall_64.tbl` (número `550`).
    4. Compilar el kernel:

    ```bash
    fakeroot make -j#nucleos
    ```

    5. Instalar módulos e instalar el kernel:

    ```bash
    sudo make modules_install
    sudo make install
    ```

    6. Actualizar GRUB y reiniciar si es necesario:

    ```bash
    sudo update-grub
    sudo reboot
    ```

    ***

    ## Notas y recomendaciones

    - El valor devuelto por la syscall está en centésimas de porcentaje (`value / 100` = porcentaje con 2 decimales).
    - La syscall bloquea ~100 ms para medir el uso; no usarla en contextos donde ese bloqueo sea crítico.
    - Si al ejecutar el ejemplo desde usuario se obtiene `EFAULT` o error, verificar permisos y que el kernel correcto esté arrancado.

    ```

    ```

```

```
