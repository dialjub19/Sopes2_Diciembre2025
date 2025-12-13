# Clase 8 — Documentación: Syscall de Encriptación Multihilo

En esta clase se implementó una syscall avanzada que encripta archivos utilizando múltiples hilos en el kernel para paralelizar la operación.

## Resumen rápido

- **Objetivo**: Crear una syscall que encripta archivos usando XOR con una clave, aprovechando múltiples hilos del kernel para paralelizar el proceso.
- **Archivo kernel**: `linux-6.12.61/kernel/encrypt.c`
- **Tabla de syscalls**: `linux-6.12.61/arch/x86/entry/syscalls/syscall_64.tbl` (número `552`)
- **Programa de usuario**: `main.c` (en la raíz del proyecto)
- **Números de hilos**: Configurable por el usuario

---

## Archivos relevantes

- `linux-6.12.61/kernel/encrypt.c`: Implementa la lógica de la syscall `my_encrypt`.
- `linux-6.12.61/arch/x86/entry/syscalls/syscall_64.tbl`: Debe contener la entrada `552 common encryp_syscall encryp_syscall`.
- `main.c`: Programa de usuario interactivo que invoca la syscall `552`.

### Detalles de la syscall

- **Nombre en código**: `my_encrypt` (definida con `SYSCALL_DEFINE4`)
- **Prototipo**: `int my_encrypt(const char __user *input_filepath, const char __user *output_filepath, const char __user *key_filepath, int thread_count)`
- **Comportamiento**: Lee un archivo de entrada, lo encripta usando XOR con una clave, y lo guarda en un archivo de salida, utilizando múltiples hilos del kernel para paralelizar la operación.

---

## 🔐 Algoritmo de Encriptación: XOR

La encriptación utiliza el operador **XOR (^)** que funciona así:

| Operación | Resultado |
| --------- | --------- |
| `0 XOR 0` | 0         |
| `0 XOR 1` | 1         |
| `1 XOR 0` | 1         |
| `1 XOR 1` | 0         |

**Propiedad importante**: XOR es reversible. Si tienes:

- `dato_encriptado = dato_original XOR clave`
- Entonces: `dato_original = dato_encriptado XOR clave`

---

## 📋 Flujo General de la Syscall

```
┌─────────────────────────────────────────┐
│  Usuario llama syscall(552, ...)        │
└──────────────┬──────────────────────────┘
               │
        ┌──────▼───────┐
        │ Copiar rutas │
        │  del usuario │
        │ al kernel    │
        └──────┬───────┘
               │
     ┌─────────▼──────────┐
     │ Abrir archivos:    │
     │ • Entrada          │
     │ • Salida           │
     │ • Clave            │
     └─────────┬──────────┘
               │
     ┌─────────▼─────────────┐
     │ Leer clave a RAM      │
     │ Leer entrada a RAM    │
     └─────────┬─────────────┘
               │
     ┌─────────▼────────────────────┐
     │ Dividir archivo en N           │
     │ fragmentos (N = thread_count)  │
     └─────────┬────────────────────┘
               │
     ┌─────────▼──────────────────┐
     │ Crear N hilos del kernel   │
     │ Cada uno procesa su parte  │
     │ (operación XOR paralela)   │
     └─────────┬──────────────────┘
               │
     ┌─────────▼──────────────────┐
     │ Esperar a todos los hilos  │
     │ (sincronización)           │
     └─────────┬──────────────────┘
               │
     ┌─────────▼───────────────────┐
     │ Escribir buffer encriptado  │
     │ al archivo de salida        │
     └─────────┬───────────────────┘
               │
     ┌─────────▼────────────────┐
     │ Liberar memoria y        │
     │ cerrar archivos          │
     └─────────┬────────────────┘
               │
        ┌──────▼────────────┐
        │ Retornar resultado│
        │ al usuario        │
        └───────────────────┘
```

---

## 🔧 Implementación del Kernel (`encrypt.c`)

### Estructuras de datos

**DataFragment**: Contiene la sección del archivo que cada hilo procesa.

```c
typedef struct {
    unsigned char *buffer;        // Puntero a los datos completos
    size_t data_size;             // Tamaño total
    unsigned char *encryption_key;// Clave de encriptación
    size_t key_length;            // Largo de la clave
    size_t start_idx;             // Inicio del fragmento para este hilo
    size_t end_idx;               // Fin del fragmento para este hilo
} DataFragment;
```

**task_params**: Coordina cada hilo individual.

```c
struct task_params {
    DataFragment data_fragment;
    struct completion completed_event; // Señal de finalización
};
```

### Funciones principales

#### 1. `perform_xor_operation()`

Función ejecutada por cada hilo del kernel:

```c
int perform_xor_operation(void *arg) {
    struct task_params *params = (struct task_params *)arg;
    DataFragment *fragment = &params->data_fragment;

    // Procesa solo la sección asignada a este hilo
    for (i = fragment->start_idx; i < fragment->end_idx; i++) {
        // XOR cada byte con un byte de la clave (ciclando si la clave es corta)
        fragment->buffer[i] ^= fragment->encryption_key[i % fragment->key_length];
    }

    // Avisa al hilo principal que terminó
    complete(&params->completed_event);
    return 0;
}
```

#### 2. `handle_file_encryption()`

Función principal que orquesta el proceso:

1. **Abrir archivos** usando `filp_open()` en el kernel
2. **Leer clave** a memoria del kernel con `kernel_read()`
3. **Leer archivo de entrada** a memoria del kernel
4. **Dividir el archivo** en `thread_count` fragmentos
5. **Crear hilos** con `kthread_run()` pasando la función `perform_xor_operation()`
6. **Esperar sincronización** con `wait_for_completion()`
7. **Escribir resultado** con `kernel_write()`
8. **Liberar memoria** con `kfree()` y cerrar archivos

#### 3. `SYSCALL_DEFINE4()`

Define la interfaz de la syscall visible desde espacio de usuario:

```c
SYSCALL_DEFINE4(my_encrypt,
    const char __user *, input_filepath,
    const char __user *, output_filepath,
    const char __user *, key_filepath,
    int, thread_count)
```

- Copia las rutas del usuario al kernel de forma segura con `strndup_user()`
- Llama a `handle_file_encryption()` con los datos del kernel
- Libera memoria y retorna el resultado

### Conceptos clave

- **kmalloc / kfree**: Reservar/liberar memoria en el kernel
- **kernel_read / kernel_write**: Leer/escribir archivos desde el kernel
- **kthread_run**: Crear y ejecutar un hilo del kernel
- **completion**: Mecanismo de sincronización para esperar hilos
- **strndup_user**: Copiar cadenas de usuario a kernel de forma segura

---

## 💻 Uso desde espacio de usuario

El programa `main.c` implementa un menú interactivo:

```c
#include <sys/syscall.h>

#define MY_ENCRYPT 552

long result = syscall(MY_ENCRYPT, input_path, output_path, key_path, num_threads);
```

### Parámetros

| Parámetro         | Tipo   | Descripción                            |
| ----------------- | ------ | -------------------------------------- |
| `input_filepath`  | char\* | Ruta del archivo a encriptar           |
| `output_filepath` | char\* | Ruta del archivo encriptado            |
| `key_filepath`    | char\* | Ruta del archivo que contiene la clave |
| `thread_count`    | int    | Número de hilos para paralelizar       |

### Programa interactivo

El programa `main.c` pide al usuario:

```
-p : Ruta del archivo de entrada
-o : Ruta del archivo de salida
-k : Archivo con la clave
-j : Número de hilos
run : Ejecutar la encriptación
```

### Compilación y ejecución

```bash
# Compilar
gcc -o encrypt main.c

# Ejecutar
./encrypt
```

### Flujo de uso

```
1. Encriptar
2. Salir

> 1
Ingrese un parametro (-p, -o, -k, -j o run para ejecutar): -p
Archivo de entrada: /home/user/documento.txt

Ingrese un parametro (-p, -o, -k, -j o run para ejecutar): -o
Archivo de salida: /home/user/documento.txt.encrypted

Ingrese un parametro (-p, -o, -k, -j o run para ejecutar): -k
Clave: /home/user/clave.key

Ingrese un parametro (-p, -o, -k, -j o run para ejecutar): -j
Número de hilos: 4

Ingrese un parametro (-p, -o, -k, -j o run para ejecutar): run
Archivo encriptado exitosamente
```

---

## ⚙️ Configuración del Kernel

### 1. Crear/verificar `encrypt.c`

Asegúrate de que existe `linux-6.12.61/kernel/encrypt.c` con la implementación completa.

### 2. Actualizar `kernel/Makefile`

```makefile
obj-y += encrypt.o
```

### 3. Actualizar `syscall_64.tbl`

Agregar la entrada (número 552):

```
552 common encryp_syscall encryp_syscall
```

---

## 🏗️ Compilación e Instalación

### Paso 1: Compilar el kernel

Desde el directorio raíz del kernel (`linux-6.12.61/`):

```bash
fakeroot make -j#nucleos
```

Reemplaza `#nucleos` por el número de cores de tu CPU.

### Paso 2: Verificar compilación

```bash
echo $?
# Debe mostrar "0" (sin errores)
```

### Paso 3: Instalar módulos e kernel

```bash
sudo make modules_install
sudo make install
```

### Paso 4: Actualizar GRUB

```bash
sudo update-grub
```

### Paso 5: Reiniciar

```bash
sudo reboot
```

### Paso 6: Verificar kernel

```bash
uname -r
```

Debe mostrar la versión compilada (ej: `6.12.61`).

---

## 🧪 Ejemplo práctico

### Crear archivos de prueba

```bash
# Crear un archivo de prueba
echo "Hola, este es un mensaje secreto!" > mensaje.txt

# Crear un archivo de clave
echo "mi_clave_super_secreta" > clave.key
```

### Encriptar

```bash
./encrypt
# Seleccionar opción 1
# Ingresar parámetros como se describe arriba
```

### Verificar

```bash
# Ver el archivo original
cat mensaje.txt
# Output: Hola, este es un mensaje secreto!

# Ver el archivo encriptado (será binario/ilegible)
cat mensaje.txt.encrypted
# Output: [caracteres ilegibles]

# Para desencriptar, ejecutar nuevamente con:
# -p: mensaje.txt.encrypted
# -o: mensaje.txt.decrypted
# -k: clave.key
# El archivo decrypted será idéntico al original
```

---

## 📊 Paralelización de Hilos

Si tu archivo tiene 1000 bytes y usas 4 hilos:

| Hilo | Rango    | Bytes |
| ---- | -------- | ----- |
| 0    | 0-249    | 250   |
| 1    | 250-499  | 250   |
| 2    | 500-749  | 250   |
| 3    | 750-1000 | 250   |

Cada hilo aplica XOR a su rango de forma **independiente y paralela**, mejorando el rendimiento en sistemas multi-core.

---

## 🐛 Solución de problemas

| Error                      | Causa                  | Solución                                  |
| -------------------------- | ---------------------- | ----------------------------------------- |
| "Archivo no encontrado"    | Ruta incorrecta        | Verificar que los archivos existan        |
| "Error al obtener syscall" | Kernel no recompilado  | Verificar que el kernel nuevo esté activo |
| "Permiso denegado"         | Permisos insuficientes | Usar `sudo` si es necesario               |
| "Error de memoria"         | Archivo muy grande     | Usar más hilos o aumentar RAM             |

---

## 📝 Notas importantes

- La encriptación XOR **es reversible**: aplicar XOR dos veces con la misma clave recupera el contenido original.
- El uso de **múltiples hilos** mejora el rendimiento en archivos grandes en sistemas multi-core.
- Los **hilos del kernel** son más eficientes que hilos en espacio de usuario para operaciones I/O intensivas.
- La **clave se repite** si es más corta que el archivo (usando operador módulo `%`).
- Este es un ejemplo **educativo**; para uso en producción, usar algoritmos criptográficos estándares (AES, RSA, etc.).

```

```
