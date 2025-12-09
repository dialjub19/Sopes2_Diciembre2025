package main

import (
	"fmt"
	"math/rand"
	"time"
)

/* 
	En este ejemplo, se simula una carrera entre tres corredores: una tortuga, una liebre y un perro.
	El objetivo es demostrar cómo las goroutines pueden ejecutarse en paralelo y cómo se pueden utilizar canales para comunicar el resultado de la carrera.
	1. Cada corredor es una goroutine que imprime su nombre y duerme un tiempo aleatorio.
	2. El tiempo de sueño simula el tiempo que tarda cada corredor en completar su parte de la carrera.
	3. Al finalizar, cada corredor envía su nombre a través de un canal.

*/

func corredor(nombre string, c chan string) {
	for i := 0; i < 10; i++ {
		fmt.Println(nombre)
		time.Sleep(time.Duration(rand.Intn(100)) * time.Millisecond)
	}
	c <- nombre
}

func main() {
	rand.Seed(time.Now().UnixNano())

	c := make(chan string)
	go corredor("🐢 Tortuga", c)
	go corredor("🐇 Liebre", c)
	go corredor("🐶 Perro", c)

	ganador := <-c
	fmt.Println("¡Ganador:", ganador, "!")
}