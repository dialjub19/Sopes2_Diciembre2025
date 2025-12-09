import threading
import time
import random

# --- CONFIGURACIÓN ---
CAPACIDAD_ESTACIONAMIENTO = 5
CANTIDAD_AUTOS = 12

# Creamos un semáforo con 5 "fichas" disponibles
# Esto permitirá que 5 hilos ejecuten la zona crítica AL MISMO TIEMPO
semaforo_estacionamiento = threading.Semaphore(CAPACIDAD_ESTACIONAMIENTO)

def auto_entra(id_auto):
    print(f"🚗 Auto {id_auto} llegó a la barrera.")
    
    # 1. ACQUIRE: Intentar entrar
    # Si hay espacio (contador > 0), entra y resta 1.
    # Si NO hay espacio (contador == 0), espera aquí.
    semaforo_estacionamiento.acquire()
    
    print(f"✅ Auto {id_auto} ENTRÓ y aparcó. (Ocupando espacio)")
    
    # Simula el tiempo que tarda en hacer sus compras (entre 1 y 3 segundos)
    tiempo_estacionado = random.randint(1, 3)
    time.sleep(tiempo_estacionado)
    
    print(f"💨 Auto {id_auto} SALIÓ liberando espacio tras {tiempo_estacionado}s.")
    
    # 2. RELEASE: Salir
    # Suma 1 al contador. Si había alguien esperando en la barrera, entra.
    semaforo_estacionamiento.release()

if __name__ == "__main__":
    print(f"--- INICIANDO ESTACIONAMIENTO (Capacidad: {CAPACIDAD_ESTACIONAMIENTO}) ---")
    
    hilos = []
    
    # Creamos 12 autos
    for i in range(1, CANTIDAD_AUTOS + 1):
        t = threading.Thread(target=auto_entra, args=(i,))
        hilos.append(t)
        t.start()
        # Pequeña pausa para que no lleguen todos en el milisegundo exacto 0
        time.sleep(0.2) 
        
    # Esperamos a que todos terminen su día
    for t in hilos:
        t.join()
        
    print("\n🏁 Todos los autos se han ido. Estacionamiento vacío.")