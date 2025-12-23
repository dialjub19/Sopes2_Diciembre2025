# Clase 10: API REST con Crow Framework en C++

## Descripción General

Este proyecto implementa una API REST usando **Crow**, un framework C++ moderno y ligero para crear servidores HTTP. Crow permite construir APIs de alto rendimiento sin dependencias pesadas.

---

## Requisitos Previos

Antes de instalar Crow, asegúrate de tener instalados los siguientes componentes:

### Instalación de Dependencias (Una sola línea)

```bash
sudo apt install g++ cmake make libasio-dev
```

Este comando instala:

- **g++**: Compilador C++
- **cmake**: Sistema de construcción
- **make**: Herramienta de compilación
- **libasio-dev**: Librería ASIO (necesaria para Crow)

### Verificación de requisitos

```bash
g++ --version
cmake --version
make --version
```

---

## Instalación de Crow

### Paso 1: Instalar las dependencias

```bash
sudo apt install g++ cmake make libasio-dev
```

### Paso 2: Clonar el repositorio de Crow

```bash
git clone https://github.com/CrowCpp/Crow.git
cd Crow
```

### Paso 3: Crear el directorio de build

```bash
cmake -B build .
cd build
```

### Paso 4: Compilar sin ejemplos ni tests

```bash
cmake .. -DCROW_BUILD_EXAMPLES=OFF -DCROW_BUILD_TESTS=OFF
```

### Paso 5: Instalar Crow

```bash
sudo make install
```

Esto instalará Crow en `/usr/local/include/crow_all.h` (archivo header único) y otras configuraciones necesarias.

### Paso 6: Verificar la instalación

```bash
ls /usr/local/include/crow_all.h
# O buscar todos los archivos de Crow
find /usr/local -name "*crow*"
```

---

## Estructura del Proyecto

```
Clase10/
├── README.md           # Este archivo
├── api.cpp             # API con middleware CORS
├── sys.cpp             # API para sistema/syscalls
└── Makefile            # (Opcional) Para compilar fácilmente
```

---

## Compilación de Proyectos

### Opción 1: Usando g++ directamente (RECOMENDADO)

```bash
# Compilación básica
g++ api.cpp

# Ejecutar el archivo a.out que se genera
./a.out

```

### Opción 2: Usando un Makefile

Crea un archivo `Makefile` en tu directorio:

```makefile
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -I/usr/local/include
LDFLAGS = -lpthread

# Targets
TARGETS = api sys

all: $(TARGETS)

api: api.cpp
	$(CXX) $(CXXFLAGS) api.cpp -o api $(LDFLAGS)

sys: sys.cpp
	$(CXX) $(CXXFLAGS) sys.cpp -o sys $(LDFLAGS)

clean:
	rm -f $(TARGETS)

run-api:
	sudo ./api

run-sys:
	sudo ./sys

.PHONY: all clean run-api run-sys
```

Luego compila con:

```bash
make          # Compila todo
make api      # Compila solo api
make clean    # Limpia los binarios
```

### Opción 3: Usando CMake

Crea un `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.13)
project(CrowAPI)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(crow_all REQUIRED)

add_executable(api api.cpp)
target_link_libraries(api PRIVATE crow_all pthread)

add_executable(sys sys.cpp)
target_link_libraries(sys PRIVATE crow_all pthread)
```

Compila con:

```bash
mkdir build
cd build
cmake ..
make
```

---

## Ejecución

### API Básica (api.cpp)

```bash
# Compilar
g++ api.cpp -o api -std=c++17 -lpthread

# Ejecutar (puede requerir sudo según los puertos)
sudo ./api

# O sin sudo si usas puerto > 1024
./api
```

**Endpoint disponible:**

- `GET http://localhost:18080/` → Devuelve "Hello world from C++"

### Sistema/Syscalls (sys.cpp)

```bash
# Compilar
g++ sys.cpp -o sys -std=c++17 -lpthread

# Ejecutar
sudo ./sys
```

**Endpoint disponible:**

- `GET http://localhost:18080/stats` → Devuelve estadísticas del CPU en JSON

---

## Ejemplos de Uso

### Prueba con curl

````bash
# Endpoint básico
curl http://localhost:18080/

# Endpoint de estadísticas (en otra terminal)
curl http://localhost:18080/stats

# GET con JSON
curl -X GET http://localhost:18080/stats | jq .

---

## Características Principales de Crow

### 1. **Routing Sencillo**

```cpp
CROW_ROUTE(app, "/ruta").methods("GET"_method)
  ([]() { return "Respuesta"; });
````

### 2. **Parámetros en URL**

```cpp
CROW_ROUTE(app, "/usuario/<int>")
  ([](int id) { return crow::response(200, "ID: " + std::to_string(id)); });
```

### 3. **JSON en Crow**

```cpp
CROW_ROUTE(app, "/json")
  ([]() {
    crow::json::wvalue response;
    response["nombre"] = "Diego";
    response["edad"] = 25;
    return response;
  });
```

### 4. **Middleware (como CORS en api.cpp)**

```cpp
struct MiMiddleware {
    struct context {};
    void before_handle(crow::request& req, crow::response& res, context&) { }
    void after_handle(crow::request&, crow::response& res, context&) { }
};

crow::App<MiMiddleware> app;
```

### 5. **Métodos HTTP**

```cpp
CROW_ROUTE(app, "/recurso").methods("GET"_method, "POST"_method)
  ([](const crow::request& req) {
    if (req.method == crow::HTTPMethod::Get) { }
    else if (req.method == crow::HTTPMethod::Post) { }
  });
```

---

## Solución de Problemas

### Error: "crow.h: No such file or directory"

- **Causa:** Crow no está instalado correctamente
- **Solución:**

```bash
# Verifica que esté instalado
ls /usr/local/include/crow_all.h

# Si no existe, reinstala Crow
cd ~/Crow/build
sudo make install
```

### Error: "undefined reference to pthread"

- **Solución:** Añade `-lpthread` al compilar:

```bash
g++ archivo.cpp -o salida -lpthread -std=c++17
```

### Puerto 18080 ya en uso

```bash
# Busca el proceso usando el puerto
lsof -i :18080

# Mata el proceso
kill -9 <PID>

# O cambia el puerto en el código:
app.port(8081).multithreaded().run();
```

### Necesita permiso de sudo

- Si usas puertos < 1024, necesitas `sudo`
- Si usas puertos >= 1024, puedes ejecutar sin `sudo`

---

## Documentación Oficial

- **GitHub Crow:** https://github.com/CrowCpp/Crow
- **Documentación:** https://crowcpp.org
- **Ejemplos:** https://github.com/CrowCpp/Crow/tree/master/examples

---

## Referencias Útiles

- **C++ 17 Standard:** https://en.cppreference.com/
- **HTTP Methods:** https://developer.mozilla.org/es/docs/Web/HTTP/Methods
- **JSON en Crow:** https://crowcpp.org/master/guides/json/
- **Multithreading:** https://en.cppreference.com/w/cpp/thread
