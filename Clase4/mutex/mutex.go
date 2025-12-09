package main

import (
	"fmt"
	"sync"
	"time"
)

// --- CONFIGURACIÓN ---
const N = 1000 // Cantidad de depósitos por gorutina

// Variable compartida (La cuenta bancaria)
var cuenta int

// Candado (Mutex) para la versión segura
var mu sync.Mutex

// WaitGroup: Es la forma en que Go espera a que terminen las gorutinas
var wg sync.WaitGroup

// ---------------------------------------------------------
// ESCENARIO 1: INSEGURO (Race Condition)
// ---------------------------------------------------------
func depositarInseguro() {
	// Avisamos al WaitGroup que terminamos cuando salga de la función
	defer wg.Done()

	for i := 0; i < N; i++ {
		// 1. Leer valor
		temp := cuenta

		// 2. SABOTAJE: Dormimos una micro-fracción para forzar el error
		// Esto permite que la otra gorutina entre y lea el mismo valor viejo
		time.Sleep(time.Microsecond)

		// 3. Escribir valor
		cuenta = temp + 1
	}
}

// ---------------------------------------------------------
// ESCENARIO 2: SEGURO (Con Mutex)
// ---------------------------------------------------------
func depositarSeguro() {
	defer wg.Done()

	for i := 0; i < N; i++ {
		// --- ZONA CRÍTICA EMPIEZA ---
		// "Lock()" bloquea el acceso. Si otra gorutina llega, tiene que esperar.
		mu.Lock()

		temp := cuenta
		time.Sleep(time.Microsecond) // Aunque durmamos, tenemos la llave. Nadie entra.
		cuenta = temp + 1

		// "Unlock()" libera la llave para el siguiente
		mu.Unlock()
		// --- ZONA CRÍTICA TERMINA ---
	}
}

// ---------------------------------------------------------
// FUNCIÓN AUXILIAR PARA CORRER PRUEBAS
// ---------------------------------------------------------
func correrPrueba(titulo string, funcionOperacion func()) {
	// Resetear la cuenta a 0
	cuenta = 0
	fmt.Println("\n🧪 EJECUTANDO:", titulo)

	// Decimos al WaitGroup que vamos a lanzar 2 gorutinas
	wg.Add(2)

	// Lanzamos las gorutinas (Hilos ligeros)
	go funcionOperacion() // Gorutina A
	go funcionOperacion() // Gorutina B

	// Esperamos a que ambas terminen (el contador del WG llegue a 0)
	wg.Wait()

	// Resultados
	esperado := N * 2
	resultado := cuenta

	if resultado == esperado {
		fmt.Printf("✅ ÉXITO: Resultado %d (Perfecto)\n", resultado)
	} else {
		perdida := esperado - resultado
		fmt.Printf("❌ FALLO:  Resultado %d (Se perdieron %d operaciones)\n", resultado, perdida)
	}
}

func main() {
	fmt.Printf("Objetivo: Llegar a %d (1000 sumas por gorutina)\n", N*2)

	// 1. Prueba Rota
	correrPrueba("Depósito INSEGURO (Race Condition)", depositarInseguro)

	// 2. Prueba Arreglada
	correrPrueba("Depósito SEGURO (Con Mutex)", depositarSeguro)
}