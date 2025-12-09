import time
import os
from concurrent.futures import ProcessPoolExecutor
import sys

# --- 1. Herramientas de Visualización (Colores) ---
def obtener_color(pid):
    """Asigna un color basado en el ID del proceso."""
    colores = [
        "\033[91m", # Rojo
        "\033[92m", # Verde
        "\033[93m", # Amarillo
        "\033[94m", # Azul
        "\033[95m", # Magenta
        "\033[96m", # Cian
    ]
    # Usamos el módulo para elegir siempre el mismo color para el mismo PID
    indice = pid % len(colores)
    return colores[indice]

def log_con_color(texto):
    """Imprime texto con el color asociado al proceso actual."""
    pid = os.getpid()
    color = obtener_color(pid)
    reset = "\033[0m"
    # Imprimimos directamente a stdout para evitar buffers
    sys.stdout.write(f"{color}[PID: {pid}] {texto}{reset}\n")

# --- 2. La Tarea Pesada ---
def tarea_pesada(n):
    # Visualización: Avisar que empezamos
    log_con_color(f"⚙️ Calculando suma de cuadrados hasta {n}...")
    
    # Carga de CPU (Matemáticas)
    total = 0
    for i in range(n):
        total += i * i
        
    return total

# --- 3. Ejecutor Secuencial ---
def correr_secuencial(lista_datos):
    print("\n" + "="*50)
    print(" 🐢 INICIANDO EJECUCIÓN SECUENCIAL (1 Hilo)")
    print("="*50)
    
    inicio = time.time()
    
    # Bucle simple: Espera a que termine uno para empezar el otro
    for dato in lista_datos:
        tarea_pesada(dato)
        
    fin = time.time()
    return fin - inicio

# --- 4. Ejecutor Paralelo ---
def correr_paralelo(lista_datos):
    nucleos = os.cpu_count()
    print("\n" + "="*50)
    print(f" 🐇 INICIANDO EJECUCIÓN PARALELA ({nucleos} Núcleos)")
    print("="*50)
    
    inicio = time.time()
    
    # El Pool crea procesos hijos y reparte la lista
    with ProcessPoolExecutor(max_workers=nucleos) as executor:
        list(executor.map(tarea_pesada, lista_datos))
        
    fin = time.time()
    return fin - inicio

# --- 5. Bloque Principal ---
if __name__ == '__main__':
    # Configuración del experimento
    # N: Qué tan difícil es cada tarea (10 millones de iteraciones)
    N = 10**7 
    # Cantidad de tareas (8 tareas para asegurar que se usen varios núcleos)
    CANTIDAD_TAREAS = 8
    datos = [N] * CANTIDAD_TAREAS

    print(f"EXPERIMENTO: Procesar {CANTIDAD_TAREAS} tareas pesadas.")
    
    # 1. Ejecución Lenta
    tiempo_sec = correr_secuencial(datos)
    print(f"\n⏱️  Tiempo Secuencial: {tiempo_sec:.2f} segundos")

    # Pausa dramática para leer la consola
    time.sleep(1)

    # 2. Ejecución Rápida
    tiempo_par = correr_paralelo(datos)
    print(f"\n⏱️  Tiempo Paralelo:   {tiempo_par:.2f} segundos")

    # 3. Resultados
    print("\n" + "-"*30)
    if tiempo_par < tiempo_sec:
        speedup = tiempo_sec / tiempo_par
        print(f"🚀 CONCLUSIÓN: El modo paralelo fue {speedup:.1f}x veces más rápido.")
    else:
        print("CONCLUSIÓN: No hubo mejora (revisa si tu CPU tiene pocos núcleos).")
    print("-"*30)