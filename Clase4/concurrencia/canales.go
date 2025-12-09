package main

import "fmt"

func enviarNumero(ch chan int) {
	fmt.Println("📤 Enviando número...")
	ch <- 7 															// Enviar el número 7 al canal
	fmt.Println("✅ Número enviado")
}

func main() {
	ch := make(chan int) 									// Crear canal sin búfer, make es una función que se usa para crear canales

	go enviarNumero(ch) 									// Lanzar goroutine que envía el canal por parametro

	num := <-ch 													// Recibir número desde el canal
	fmt.Println("📥 Número recibido:", num)
}