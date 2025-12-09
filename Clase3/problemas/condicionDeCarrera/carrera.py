import threading
import time

# --- CONFIGURACIÓN ---
# Cantidad de depósitos que hará CADA hilo.
# Necesitamos un número grande para aumentar la probabilidad de que choquen.
N = 1000  # 1 millón

# Variable compartida (Recurso Crítico)
cuenta_bancaria = 0

def depositar_dinero(hilo_nombre):
    global cuenta_bancaria
    print(f"[{hilo_nombre}] 🟢 Iniciando {N} depósitos...")
    
    for _ in range(N):
      # 1. Leer (copia local)
      copia_local = cuenta_bancaria
      
      # 2. SABOTAJE: Forzamos un cambio de contexto aquí.
        # Esto le dice al procesador: "Pausa este hilo y deja pasar al otro".
      time.sleep(0.0001)
        # AQUÍ OCURRE EL ERROR INVISIBLE:
        # La instrucción `cuenta_bancaria = copia_local + 1` parece una sola línea,
        # pero para la CPU son 3 pasos separados:
        # 1. LEER valor actual.
        # 2. SUMAR 1.
        # 3. GUARDAR nuevo valor.
        # Si el hilo es interrumpido entre el paso 1 y 3, el dato se pierde.
      
      # 3. Escribir (guardamos el valor viejo + 1)
      cuenta_bancaria = copia_local + 1
        
    print(f"[{hilo_nombre}] 🔴 Terminó su trabajo.")

def ejecutar_experimento():
    global cuenta_bancaria
    cuenta_bancaria = 0  # Resetear cuenta
    
    print(f"--- INICIANDO SIMULACIÓN DE CARRERA ---")
    print(f"Objetivo esperado: {N * 2} (Si todo fuera perfecto)")
    print(f"Valor inicial: {cuenta_bancaria}\n")

    # Crear dos hilos que apuntan a la misma función
    hilo1 = threading.Thread(target=depositar_dinero, args=("HILO-1",))
    hilo2 = threading.Thread(target=depositar_dinero, args=("HILO-2",))

    # Iniciar cronómetro
    inicio = time.time()

    # Arrancar los hilos casi simultáneamente
    hilo1.start()
    hilo2.start()

    # .join() hace que el programa principal espere a que terminen los hilos
    hilo1.join()
    hilo2.join()
    
    fin = time.time()

    # --- RESULTADOS ---
    esperado = N * 2
    real = cuenta_bancaria
    perdida = esperado - real
    
    print("\n" + "="*40)
    print(f"💰 RESULTADO FINAL EN CUENTA: {real}")
    print(f"🏆 RESULTADO ESPERADO:      {esperado}")
    print("="*40)
    
    if real < esperado:
        print(f"⚠️  ¡CONDICIÓN DE CARRERA DETECTADA!")
        print(f"❌ Se perdieron {perdida} operaciones.")
        print(f"📉 Precisión: {(real/esperado)*100:.4f}%")
        print("El sistema operativo interrumpió a los hilos en el momento equivocado.")
    else:
        print("✅ ¡Increíble! Tuviste suerte y no hubo colisiones (raro).")
        print("Prueba aumentar N y ejecuta de nuevo.")

if __name__ == "__main__":
    # Ejecutamos el experimento varias veces para ver la variación
    ejecutar_experimento()