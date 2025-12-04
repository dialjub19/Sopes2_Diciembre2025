#include <linux/init.h>          // Macros __init y __exit
#include <linux/proc_fs.h>       // Funciones para crear archivos en /proc
#include <linux/seq_file.h>      // Lectura secuencial de archivos en /proc
#include <linux/module.h>        // Funciones para crear un módulo
#include <linux/kernel.h>        // Funciones para imprimir mensajes en el kernel

/*
    ¿Qué son los macros?
    Los macros son una forma de definir funciones o constantes en tiempo de preprocesamiento.
    __init y __exit indican al kernel qué funciones se deben llamar al cargar o descargar el módulo.
*/

// Información del módulo
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hola mundo");
MODULE_AUTHOR("Juarez");

// Función que se invoca al leer /proc/hello
// Muestra un mensaje personalizado
static int mostrar_info(struct seq_file *m, void *v) {
    seq_printf(m, "Si sale Sopes2 Guys!\n");  // Mensaje personalizado
    return 0;
}

// Función que se llama al abrir /proc/hello
static int abrir(struct inode *inode, struct file *file) {
    return single_open(file, mostrar_info, NULL);  // Asocia la lectura secuencial con mostrar_info
}

// Estructura que define las operaciones sobre /proc/hello
static const struct proc_ops ops = {
    .proc_open = abrir,             // Se llama al abrir el archivo
    .proc_read = seq_read,          // Función de lectura secuencial
    .proc_release = single_release  // Se llama al cerrar el archivo
};

// Función que se ejecuta al cargar el módulo con insmod
static int __init hello_init(void) {
    proc_create("hello", 0, NULL, &ops);  // Crea /proc/hello
    printk(KERN_INFO "Si sale Sopes2 Guys!\n");  // Mensaje en el log del kernel
    return 0;
}

// Función que se ejecuta al quitar el módulo con rmmod
static void __exit hello_exit(void) {
    remove_proc_entry("hello", NULL);  // Elimina /proc/hello
    printk(KERN_INFO "Adios, Sopes2 sale sola con el aux!\n");  // Mensaje en el log al remover
}

// Registro de funciones de inicio y salida del módulo
module_init(hello_init);
module_exit(hello_exit);