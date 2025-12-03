# Guía para trabajar con módulos de kernel en Linux

Este documento explica cómo cargar, crear, eliminar y verificar un módulo de kernel en Linux, así como los requisitos previos necesarios.

## Requisitos previos

Antes de trabajar con módulos de kernel, asegúrate de tener instalado lo siguiente:

- **Kernel headers**: Los encabezados del kernel correspondientes a tu versión de Linux.
- **Herramientas de compilación**: `gcc`, `make` y `build-essential` (en distribuciones basadas en Debian/Ubuntu).
- **Herramientas para manejo de módulos**: `insmod`, `rmmod`, `lsmod`, `modinfo` (normalmente incluidas en el paquete `kmod`).

### Instalación de dependencias (ejemplo en Ubuntu/Debian)

Verificacion de c/c++ instalados en el sistemas

```sh
sudo apt-get/dnf install gcc

# ver versión
gcc --version
```

```sh
sudo apt update
sudo apt-get install make
sudo apt-get install build-essential
```

## 1. Crear un módulo de kernel

1. Escribe el código fuente del módulo, por ejemplo en `hello.c`.
2. Crea un `Makefile` para compilar el módulo:

```makefile
obj-m += hello.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

3. Compila el módulo:

```sh
make
```

Esto generará un archivo `hello.ko`.

## 2. Cargar un módulo de kernel

Para cargar el módulo:

```sh
sudo insmod hello.ko
```

O usando `modprobe` (si el módulo está en la ruta de módulos):

```sh
sudo modprobe hello
```

## 3. Verificar un módulo cargado

Para verificar que el módulo está cargado:

```sh
lsmod | grep hello
```

Para obtener información detallada:

```sh
modinfo hello.ko
```

Para verificar su contenido podemos verificar los logs del kernel:
dmesg: Significa "diagnostic message" y muestra el buffer de mensajes del kernel.
| : Es un "tubo" que pasa la salida de un comando como entrada a otro.
tail -n 20: Muestra las últimas 20 líneas de la entrada.

```sh
sudo dmesg | tail -n 20
```

## 4. Eliminar un módulo de kernel

Para eliminar el módulo:

```sh
sudo rmmod hello
```

Verifica que se haya eliminado:

```sh
lsmod | grep hello
```

---

**Nota:** Debes tener privilegios de superusuario para cargar o eliminar módulos del kernel.

