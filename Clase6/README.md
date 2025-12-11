# Guía de Creación de una Llamada al Sistema Personalizada

## 📚 ¿Qué es una Syscall?

Una **syscall (llamada al sistema)** es un mecanismo que permite a los programas en modo usuario solicitar servicios del kernel. Actúan como una interfaz entre el espacio de usuario y el espacio del kernel, permitiendo que las aplicaciones accedan a recursos del sistema como archivos, memoria, procesos, etc.

En este proyecto, crearemos una syscall personalizada llamada `uptime_syscall` que retorna el tiempo de actividad del sistema desde el último reinicio.

---

## 🔧 Carpetas que se deben Modificar

### 1. **linux-6.12.61/kernel/**

**Archivo:** `syscall_uptime.c`

**¿Por qué?** Aquí se define la lógica de la syscall. El kernel necesita la implementación de la función que será ejecutada cuando se realice la llamada.

```c
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/timekeeping.h>

SYSCALL_DEFINE0(uptime_syscall){
    unsigned long uptime = ktime_get_boottime_seconds();
    return uptime;
}
```

**Explicación:**

- `SYSCALL_DEFINE0()`: Define una syscall sin parámetros
- `ktime_get_boottime_seconds()`: Función del kernel que obtiene los segundos desde el boot
- Retorna el tiempo de actividad en segundos

### 2. **linux-6.12.61/kernel/Makefile**

**¿Por qué?** El Makefile controla qué archivos se compilan. Debemos agregar `syscall_uptime.o` para que el compilador incluya nuestro archivo.

**Modificación necesaria:**

```makefile
# Agregar la línea:
obj-y += syscall_uptime.o
```

### 3. **linux-6.12.61/arch/x86/entry/syscalls/syscall_64.tbl**

**¿Por qué?** Esta tabla mapea el número de la syscall con su nombre y función. El kernel necesita saber qué número de syscall corresponde a nuestra función.

**Modificación necesaria:**

```
# Agregar al final de la tabla:
550    common    uptime_syscall        sys_uptime_syscall
```

Donde:

- `550`: Número único de la syscall
- `common`: Disponible en arquitectura x86_64 común
- `uptime_syscall`: Nombre de la syscall
- `sys_uptime_syscall`: Nombre de la función en el kernel

---

## 🏗️ Compilación del Kernel

> Reemplaza `#nucleos` por el número de hilos/cores de tu CPU

```bash
fakeroot make -j#nucleos
```

## Verificación del proceso de compilación

```bash
// Si muestra un "0" quiere decir que no hubo errores y la compilación fue exitosa.
echo $?
```

### Instalar módulos

```bash
sudo make modules_install
```

### Instalar el kernel

```bash
sudo make install
```

---

## 🖥️ Configuración de GRUB

```bash
cd /etc/default
sudo nano grub
```

Modificar las siguientes líneas en el archivo de grub, por defecto tiene esto valores:

```bash
GRUB_DEFAULT=0
GRUB_TIMEOUT_STYLE=hidden
GRUB_TIMEOUT=0
```

Modificamos con los siguientes valores:

```bash
GRUB_DEFAULT=save
GRUB_TIMEOUT_STYLE=menu
GRUB_TIMEOUT=10
```

Guardar con:

```
Ctrl + O
Ctrl + X
```

### Actualizar GRUB

```bash
sudo update-grub
```

### Reiniciar

```bash
sudo reboot
```

### Seleccionar kernel compilado

Aunque no se muestre el Kernel instalado no quiere decir que algo haya salido mal, ya que al iniciar el SO podemos ingresar a las opciones avanzadas presionando "shift+esc" el cual nos despliega el grub y ahi podemos seleccionar el la version de Kernel instalada, tambien se puede evitar hacer este proceso modificando archivos dentro de SO.

![error kernel1](./img/kernel1.png)
![error kernel2](./img/kernel2.png)

---

## 💻 Uso de la Syscall Personalizada

Una vez compilado e instalado el kernel, podemos usar la syscall desde nuestro programa en C. A continuación se muestra cómo hacerlo:

### main.c

```c
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
```

### Explicación:

- `#include <sys/syscall.h>`: Incluye las definiciones necesarias para hacer syscalls
- `#define uptime_syscall 550`: Define el número de la syscall (debe coincidir con syscall_64.tbl)
- `syscall(uptime_syscall)`: Realiza la llamada al sistema pasando el número de syscall
- La función retorna un `long` con el tiempo en segundos

### Compilación y Ejecución:

```bash
# Compilar
gcc -o uptime main.c

# Ejecutar
./uptime
```

### Salida esperada:

```
====================================
    Tiempo desde el último reinicio
======================================
Tiempo de actividad: XXXX segundos
======================================
```

---

## 📝 Resumen del Proceso

| Paso | Acción                   | Archivo                                  |
| ---- | ------------------------ | ---------------------------------------- |
| 1    | Crear la función syscall | `kernel/syscall_uptime.c`                |
| 2    | Registrar en el Makefile | `kernel/Makefile`                        |
| 3    | Mapear número a función  | `arch/x86/entry/syscalls/syscall_64.tbl` |
| 4    | Compilar el kernel       | `fakeroot make -j#nucleos`               |
| 5    | Instalar módulos         | `sudo make modules_install`              |
| 6    | Instalar el kernel       | `sudo make install`                      |
| 7    | Actualizar GRUB          | `sudo update-grub`                       |
| 8    | Reiniciar                | `sudo reboot`                            |
| 9    | Usar desde programa C    | `main.c` con `syscall()`                 |
