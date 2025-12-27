# Clase 12: API con Autenticación PAM y Aplicación Web Angular

## Descripción General

En esta clase se desarrolló una solución completa de autenticación que integra:

- **API en C++** usando Crow framework con autenticación PAM (Pluggable Authentication Modules)
- **Aplicación web Angular** con servicios para consumo de API y gestión de sesiones

---

## 📚 Componentes

### 1. API en C++ (`Api/api.cpp`)

#### ¿Qué es PAM?

PAM (Pluggable Authentication Modules) es un módulo de Linux que proporciona un marco flexible para la autenticación. En lugar de que cada aplicación maneje directamente las credenciales, PAM actúa como intermediario, permitiendo usar las credenciales del sistema Linux.

#### Características de la API

**Función de Conversación PAM (`pam_conv_cb`)**

```cpp
static int pam_conv_cb(int num_msg, const struct pam_message **msg,
                       struct pam_response **resp, void *appdata_ptr)
```

- Maneja la comunicación bidireccional entre PAM y la aplicación
- Responde a diferentes tipos de mensajes PAM (prompts de contraseña, mensajes de error, etc.)
- Recibe la contraseña a través del parámetro `appdata_ptr`

**Función de Autenticación (`pam_authenticate_user`)**

```cpp
static bool pam_authenticate_user(const std::string& username,
                                  const std::string& password,
                                  std::string* error_out = nullptr)
```

- Inicia una sesión PAM con el servicio "login"
- Autentica al usuario contra el sistema operativo
- Valida la gestión de cuentas
- Retorna `true` si la autenticación es exitosa, `false` en caso contrario
- Captura mensajes de error de PAM

**Manejo CORS**

```cpp
struct CORS
```

- Permite peticiones desde navegadores web
- Maneja preflight requests (OPTIONS)
- Añade headers de CORS a todas las respuestas

**Endpoints**

1. **GET `/`**

   - Respuesta simple de prueba
   - Retorna: `"Hello world from C++"`

2. **POST `/auth`**
   - Realiza la autenticación del usuario
   - Body esperado:
     ```json
     {
       "username": "nombre_usuario",
       "password": "contraseña"
     }
     ```
   - Respuesta exitosa (200):
     ```json
     {
       "ok": true,
       "username": "nombre_usuario"
     }
     ```
   - Respuesta fallida (401):
     ```json
     {
       "ok": false,
       "error": "mensaje_de_error_pam"
     }
     ```

**Compilación**

```bash
g++ api.cpp -o api -lpthread -lpam -lpam_misc
```

- `-lpthread`: Soporte multihilo
- `-lpam`: Librería PAM
- `-lpam_misc`: Utilidades adicionales de PAM

**Ejecución**

```bash
./api
# Servidor escucha en puerto 18080
```

---

### 2. Aplicación Angular (`appWeb/`)

#### Servicios Implementados

##### **2.1 Auth Service** (`src/app/auth/auth.ts`)

Servicio encargado de comunicarse con la API de autenticación.

**Interfaz `AuthLogin`**

```typescript
export interface AuthLogin {
  username: string;
  password: string;
}
```

- Define la estructura de datos para enviar al endpoint `/auth`

**Interfaz `ResponseLogin`**

```typescript
export interface ResponseLogin {
  ok: boolean;
  role: string;
  username: string;
}
```

- Define la respuesta esperada de la API

**Método `authLogin()`**

```typescript
async authLogin(login: AuthLogin): Promise<ResponseLogin>
```

- Realiza una petición POST a `http://localhost:18080/auth`
- Utiliza `HttpClient` de Angular
- Retorna una promesa con la respuesta
- Usa `lastValueFrom` para convertir Observable a Promise

##### **2.2 Storage Service** (`src/app/shared/service/storage.ts`)

Servicio para gestionar datos del usuario en localStorage.

**Señal `user`**

```typescript
user = signal<ResponseLogin | null>(this.readUser());
```

- Almacena el usuario actual en memoria
- Se sincroniza con localStorage al inicio

**Computed `isLoggedIn`**

```typescript
isLoggedIn = computed(() => this.user !== null);
```

- Indicador automático de si el usuario está autenticado

**Método `setUser(user: ResponseLogin)`**

```typescript
setUser(user: ResponseLogin)
```

- Guarda el usuario en localStorage (JSON)
- Actualiza la señal en memoria
- Se ejecuta después de login exitoso

**Método `clearUser()`**

```typescript
clearUser();
```

- Elimina solo el usuario actual de localStorage
- Resetea la señal user a null

**Método `deleteAllStorege()`**

```typescript
deleteAllStorege();
```

- Limpia todo localStorage
- Resetea la señal user a null

---

#### Componentes

##### **2.3 Login Component** (`src/app/auth/login/login.ts`)

Componente principal de autenticación.

**Funcionalidades:**

- Formulario reactivo con validación
  - `username`: requerido
  - `password`: requerido
- Integración con Auth Service para realizar login
- Integración con Storage Service para guardar sesión
- Navegación automática al dashboard después de login exitoso
- Manejo de errores:
  - 401: Usuario o contraseña incorrectos
  - Otros errores: mensaje de error inesperado
- Toggle de visibilidad de contraseña

**Método `login()`**

```typescript
async login()
```

- Valida el formulario
- Llama a `authService.authLogin()` con credenciales
- Guarda la respuesta en localStorage usando Storage Service
- Navega a `auth/workspace`
- Maneja excepciones HTTP

##### **2.4 Dashboard Component** (`src/app/workspace/dashboard/dashboard.ts`)

Componente principal después de autenticación.

- Componente básico que servirá como punto de entrada a la aplicación
- Estructura lista para agregar funcionalidades de workspace

---

#### Rutas (`src/app/app.routes.ts`)

```typescript
const routes: Routes = [
  {
    path: "login",
    component: Login, // Ruta de login
  },
  {
    path: "auth",
    loadChildren: () =>
      import("./workspace/dashboard/workspace.route").then((m) => m.routes), // Lazy loading del módulo auth
  },
  { path: "", redirectTo: "login", pathMatch: "full" }, // Ruta por defecto
  { path: "**", redirectTo: "login" }, // Rutas no encontradas
];
```

**Estructura de Rutas:**

- `/login` → Componente Login
- `/auth/workspace` → Dashboard (cargado con lazy loading)
- `/` → Redirige a `/login`
- `/*` → Redirige a `/login`

**Rutas Hijas** (`src/app/workspace/dashboard/workspace.route.ts`)

```typescript
const routes: Routes = [
  {
    path: "workspace",
    component: Dashboard,
  },
];
```

---

## 🔄 Flujo de Autenticación

```
1. Usuario ingresa credenciales en formulario Login
          ↓
2. Componente Login valida el formulario
          ↓
3. Llama a Auth Service (authLogin)
          ↓
4. Auth Service envía POST a http://localhost:18080/auth
          ↓
5. API C++ recibe solicitud
          ↓
6. API utiliza PAM para validar credenciales del sistema
          ↓
7. Si es exitoso → Retorna {ok: true, username: X}
   Si falla → Retorna {ok: false, error: "mensaje"}
          ↓
8. Login Component recibe respuesta
          ↓
9. Si exitoso:
   - Storage Service guarda usuario en localStorage
   - Se navega a /auth/workspace
   Si fallido:
   - Muestra alerta con error
```

---

## 📝 Flujo de Datos

### Login Flow:

```
Login.html
    ↓
Login Component (form submission)
    ↓
Auth Service.authLogin()
    ↓
HTTP POST → API C++ (/auth)
    ↓
Storage Service.setUser() + Router.navigate()
    ↓
Dashboard Component
```

### State Management:

```
Storage Service
    ├── user: Signal<ResponseLogin | null>
    ├── isLoggedIn: Computed boolean
    ├── localStorage: Persistencia
```

---

## 🛠️ Requisitos del Sistema

### Para la API C++:

- GCC/G++ compilador
- Librerías PAM (`libpam0g-dev` en Debian/Ubuntu)
- Puerto 18080 disponible

### Para Angular:

- Node.js + npm
- Angular 17+ (standalone components)
- Puerto 4200 disponible (desarrollo)

---

## 🚀 Uso

### Ejecutar API:

```bash
cd Api
g++ api.cpp -o api -lpthread -lpam -lpam_misc
./api
```

### Ejecutar aplicación Angular:

```bash
cd appWeb
npm install
ng serve
```

Acceder a `http://localhost:4200`

### Probar autenticación:

1. Usar credenciales de un usuario del sistema Linux
2. Ejemplo: usuario `root` con su contraseña

---

## 🔐 Consideraciones de Seguridad

- **PAM:** Valida contra credenciales del sistema operativo
- **CORS:** Abierto en desarrollo (restringir en producción)
- **localStorage:** Datos persistentes (token/sesión en futuras mejoras)
- **HTTPS:** Recomendado en producción
