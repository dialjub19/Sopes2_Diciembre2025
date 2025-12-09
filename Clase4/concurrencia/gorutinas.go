package main

import (
    "time"
    "fmt"
)


func hello() {
    fmt.Println("¡Hola desde goroutine!")
}


func main() {
    go hello()            // Lanza la goroutine con la palabra reservada "go"

    // Aquí podrías hacer otras cosas mientras hello() se ejecuta en paralelo
    fmt.Println("Hola desde main")
    
    /* 
        Para evitar que el programa termine antes de que la goroutine complete su ejecución,
        podrías usar un mecanismo de sincronización como un WaitGroup o simplemente esperar un poco.
        Aquí, simplemente usamos un Sleep para dar tiempo a la goroutine a ejecutarse.
     */

    time.Sleep(5 * time.Second) // Descomentar si se quiere esperar un segundo
}