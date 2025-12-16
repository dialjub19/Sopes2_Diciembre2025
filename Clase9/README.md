# Clase 9 — Documentación: PAM (Pluggable Authentication Modules)

**¿Qué es PAM?**

PAM (Pluggable Authentication Modules) es una arquitectura flexible que proporciona una forma modular y estandarizada de realizar la autenticación, autorización y gestión de sesiones en sistemas Unix/Linux. PAM separa la lógica de autenticación de las aplicaciones, permitiendo que servicios como `sshd`, `login`, `sudo`, `gdm`, `vsftpd`, entre otros, deleguen las tareas de autenticación a módulos configurables.

**¿Para qué se usa PAM?**

- Autenticación de usuarios (contraseñas, biometría, tokens, certificados).
- Gestión de cuentas (controlar si una cuenta está permitida acceder: expiración, horarios).
- Gestión de contraseñas (políticas de cambio, complejidad, actualización).
- Gestión de sesión (acciones antes/después del login, montado de recursos, limpieza).

**¿Dónde se usa PAM?**

PAM se utiliza en la mayoría de los servicios que requieren autenticación en un sistema Linux, por ejemplo:

- `sshd` (acceso remoto)
- `login` / `getty` (consola)
- `sudo` (elevación de privilegios)
- `gdm`, `lightdm` (gestores de pantalla/entorno gráfico)
- `vsftpd`, `proftpd` (servicios FTP)
- `sssd`, servicios LDAP/AD cuando integran autenticación local

**Arquitectura y tipos de módulos**

Los módulos PAM se organizan en pilas por servicio y por tipo de control. Los tipos principales son:

- `auth`: manejo de autenticación (login, verificación de credenciales).
- `account`: verificación de la cuenta (expirada, bloqueada, políticas de acceso).
- `password`: operaciones relacionadas con cambio de contraseña.
- `session`: tareas que se ejecutan al iniciar/cerrar sesión (montar directorios, establecer límites).

Cada entrada en la configuración PAM tiene la forma:

```
<tipo> <control> <módulo> <opciones>
```

Ejemplo:

```
auth required pam_unix.so try_first_pass
account required pam_unix.so
password required pam_unix.so use_authtok
session required pam_unix.so
```

Donde `control` puede ser `required`, `requisite`, `sufficient`, `optional`, o usar reglas de control más avanzadas.

**Archivos de configuración**

- `/etc/pam.d/`: Directorio con archivos por servicio (por ejemplo `/etc/pam.d/sshd`, `/etc/pam.d/sudo`).
- `/etc/pam.conf`: Archivo de configuración global (menos usado hoy; la convención moderna es `/etc/pam.d/`).

Cada archivo en `/etc/pam.d/` define la pila PAM para el servicio específico.

**Ejemplo: permitir autenticación por contraseña y luego por OTP (conceptual)**

En `/etc/pam.d/sshd`:

```
# Autenticación: primero pam_unix, si falla pedir OTP
auth required pam_unix.so try_first_pass
auth required pam_google_authenticator.so nullok

# Control de cuenta
account required pam_unix.so

# Sesión
session required pam_unix.so
```

**Integrar un módulo PAM personalizado**

1. Escribir el módulo como una biblioteca compartida siguiendo la API de PAM (`pam_sm_authenticate`, `pam_sm_setcred`, etc.).
2. Compilar el módulo y colocarlo en `/lib/security/` o `/lib64/security/`.
3. Añadir la línea correspondiente en el archivo de servicio en `/etc/pam.d/`.
4. Probar cuidadosamente con un servicio de bajo riesgo antes de ponerlo en producción.

Ejemplo mínimo de prototipo (conceptual, no completo):

```c
#include <security/pam_modules.h>
#include <security/pam_ext.h>

int pam_sm_authenticate(pam_handle_t *pamh, int flags, int argc, const char **argv) {
    // Lógica de autenticación
    return PAM_SUCCESS; // o PAM_AUTH_ERR
}
```

**Comprobación y pruebas**

- Usar `pamtester` (si está disponible) para probar entradas PAM sin afectar servicios reales.
- Probar con un servicio no crítico (por ejemplo, un servicio de prueba que lance `pam_authenticate`).
- Revisar `/var/log/auth.log` o `journalctl -t sshd` para ver mensajes de autentificación y errores.

Ejemplo de uso básico de `pamtester` (si instalado):

```bash
pamtester sshd username authenticate
```

**Buenas prácticas y seguridad**

- Hacer copias de seguridad de los archivos en `/etc/pam.d/` antes de modificarlos.
- Probar cambios en un entorno de laboratorio o VM.
- Evitar dejar configuraciones que permitan autenticación sin verificación (`sufficient` mal usada).
- Limitar el uso de módulos que ejecuten código de usuario sin validación.
- Registrar eventos de autenticación y monitorear intentos fallidos (usar `pam_faillock` o `pam_tally2`).
- Mantener módulos y dependencias actualizados.

**Casos de uso típicos**

- Requerir MFA (contraseña + OTP) para `ssh`.
- Forzar expiración de contraseñas y políticas de complejidad.
- Integrar autenticación contra directorios externos (LDAP, Kerberos) mediante módulos PAM.
- Ejecutar tareas de sesión (montar home en red, crear directorios temporales).

**Riesgos y mitigaciones**

- Una mala configuración de PAM puede dejar inaccesibles servicios esenciales (por ejemplo `sudo` o `sshd`).
  - Mitigación: mantener una sesión abierta o consola de rescate, y probar cambios gradualmente.
- Módulos de terceros no confiables pueden comprometer la seguridad del sistema.
  - Mitigación: auditar y preferir soluciones mantenidas por la comunidad/distribución.

**Recursos y lecturas recomendadas**

- Manual de PAM: `man pam`, `man pam.conf`, `man pam.d`.
- ArchWiki — PAM: https://wiki.archlinux.org/title/PAM
- Linux-PAM project: http://www.linux-pam.org/

---

## 💻 Cómo Compilar y Ejecutar `main.c`

El archivo `main.c` en Clase9 contiene un programa práctico que demuestra cómo usar PAM desde una aplicación en C. Este programa realiza un flujo completo de autenticación de usuarios.

### Paso 1: Instalar Dependencias

Antes de compilar, necesitas tener las librerías de desarrollo de PAM instaladas:

**En Debian/Ubuntu:**

```bash
sudo apt-get update
sudo apt-get install libpam0g-dev
```

### Paso 2: Compilar el Programa

Navega al directorio de Clase9:

```bash
cd ~/Sopes2_Diciembre2025/Clase9
```

Compila el programa con el siguiente comando:

```bash
gcc -o pam_test main.c -lpam -lpam_misc
```

**Explicación de las flags:**

- `gcc`: Compilador C de GNU
- `-o pam_test`: Nombre del ejecutable de salida
- `main.c`: Archivo fuente
- `-lpam`: Enlaza contra la librería PAM
- `-lpam_misc`: Enlaza contra utilidades adicionales de PAM

**Resultado esperado:**
Si no hay errores, se creará un archivo ejecutable llamado `pam_test`.

### Paso 3: Ejecutar el Programa

```bash
./pam_test
```

El programa te solicitará:

```
Usuario:
```

Ingresa un nombre de usuario válido en tu sistema (por ejemplo, `diego`, `root`, etc.):

```bash
Usuario: diego
Contraseña:
```

Luego ingresa la contraseña (no se mostrará en pantalla):

```bash
Contraseña: mi_contraseña_secreta
```

### Resultados Posibles

#### ✅ Autenticación Exitosa

Si las credenciales son correctas:

```
Autenticación Completa ✅
```

#### ❌ Credenciales Incorrectas

Si la contraseña es incorrecta:

```
Usuario o contraseña incorrectos ❌: Authentication failure
```

#### ❌ Usuario No Existe

Si el usuario no existe en el sistema:

```
Usuario o contraseña incorrectos ❌: User not known to the underlying authentication module
```

---

## 🔍 Estructura y Flujo de `main.c`

### Función Principal: `pam_conv_callback()`

Esta es la función de **conversación** que PAM llama para comunicarse con la aplicación:

```c
static int pam_conv_callback(int num_msg, const struct pam_message **msg,
                              struct pam_response **resp, void *appdata_ptr)
```

**¿Qué hace?**

1. Recibe mensajes de PAM (solicitudes de contraseña, usuario, etc.)
2. Extrae la contraseña del parámetro `appdata_ptr`
3. Devuelve las respuestas en la estructura `resp`
4. Maneja errores y libera memoria si es necesario

**Tipos de mensajes:**

- `PAM_PROMPT_ECHO_OFF`: Entrada secreta (contraseña) - no se muestra en pantalla
- `PAM_PROMPT_ECHO_ON`: Entrada visible (usuario, email, etc.)
- Otros tipos se tratan como error

### Función Principal: `main()`

El flujo principal es:

**1. Entrada de credenciales:**

```c
char username[64];
char password[64];
scanf("%63s", username);
scanf("%63s", password);
```

**2. Configurar PAM:**

```c
struct pam_conv conversation = {pam_conv_callback, (void *)password};
pam_handle_t *pam_handler = NULL;
```

**3. Iniciar sesión PAM:**

```c
int pam_result = pam_start("login", username, &conversation, &pam_handler);
```

- Usa el servicio `"login"` (busca configuración en `/etc/pam.d/login`)
- Almacena el estado en `pam_handler`

**4. Autenticar:**

```c
pam_result = pam_authenticate(pam_handler, 0);
```

- PAM llama a `pam_conv_callback()` para obtener la contraseña
- Valida contra `/etc/shadow`

**5. Verificar cuenta:**

```c
if (pam_result == PAM_SUCCESS) {
    pam_result = pam_acct_mgmt(pam_handler, 0);
}
```

- Verifica expiración de contraseña
- Comprueba bloqueos de usuario
- Valida horarios permitidos

**6. Cerrar sesión PAM:**

```c
pam_end(pam_handler, pam_result);
```

- **Muy importante:** libera toda la memoria reservada por PAM

---

## 🧪 Casos de Prueba

### Prueba 1: Usuario válido con contraseña correcta

```bash
$ ./pam_test
Usuario: diego
Contraseña: micontraseña123
Autenticación Completa ✅
```

### Prueba 2: Usuario válido con contraseña incorrecta

```bash
$ ./pam_test
Usuario: diego
Contraseña: contraseña_falsa
Usuario o contraseña incorrectos ❌: Authentication failure
```

### Prueba 3: Usuario que no existe

```bash
$ ./pam_test
Usuario: usuario_fantasma
Contraseña: cualquier_contraseña
Usuario o contraseña incorrectos ❌: User not known to the underlying authentication module
```

---

## 🔧 Verificar Configuración PAM

Antes de ejecutar, verifica la configuración de PAM para el servicio `login`:

```bash
cat /etc/pam.d/login
```

Deberías ver algo como:

```
auth    required    pam_securetty.so
auth    required    pam_env.so
auth    required    pam_unix.so try_first_pass
account required    pam_unix.so
password required   pam_unix.so
session required    pam_unix.so
```

| Línea                           | Función                                   |
| ------------------------------- | ----------------------------------------- |
| `auth required pam_unix.so`     | Autentica contra `/etc/shadow`            |
| `account required pam_unix.so`  | Verifica cuenta válida/bloqueada/expirada |
| `password required pam_unix.so` | Maneja cambio de contraseña               |
| `session required pam_unix.so`  | Configura variables de sesión             |

---

## 🔐 Buenas Prácticas de Seguridad

### 1. Ocultar entrada de contraseña

El código actual usa `scanf()` que muestra los caracteres. Para producción, usa `getpass()`:

```c
#include <unistd.h>

char *password = getpass("Contraseña: ");
```

### 2. Validar resultados de PAM

Siempre verifica el código de retorno:

```c
if (pam_result != PAM_SUCCESS) {
    fprintf(stderr, "Error: %s\n", pam_strerror(pam_handler, pam_result));
    pam_end(pam_handler, pam_result);
    return 1;
}
```

### 3. Limpiar memoria sensible

Borra credenciales después de usar:

```c
memset(password, 0, sizeof(password));  // Sobrescribe con ceros
```

### 4. No hardcodear credenciales

Siempre ingresa credenciales en runtime, nunca en el código fuente.

---

## 🛠️ Solución de Problemas

| Error                                          | Causa                           | Solución                                                  |
| ---------------------------------------------- | ------------------------------- | --------------------------------------------------------- |
| `error: pam_appl.h: No such file or directory` | Falta libpam-dev                | Instala `libpam0g-dev` (Debian) o `pam-devel` (RHEL)      |
| `undefined reference to 'pam_start'`           | No enlazó con `-lpam`           | Usa: `gcc -o pam_test main.c -lpam -lpam_misc`            |
| `Authentication failure`                       | Contraseña incorrecta           | Verifica que uses la contraseña correcta del usuario      |
| `User not known`                               | Usuario no existe en el sistema | Usa un usuario que exista (ej: `root`, tu usuario actual) |
| `Permission denied`                            | Falta permisos de ejecución     | `chmod +x pam_test`                                       |

---

## 📋 Checklist de Compilación y Ejecución

- [ ] Instalar libpam-dev / pam-devel
- [ ] Navegar al directorio Clase9
- [ ] Compilar con: `gcc -o pam_test main.c -lpam -lpam_misc`
- [ ] Verificar que se creó el archivo `pam_test`
- [ ] Ejecutar: `./pam_test`
- [ ] Ingresar credenciales válidas del sistema
- [ ] Verificar mensaje de éxito o error

---

## 📚 Referencias

- Manual de PAM: `man pam`, `man pam.conf`, `man pam.d`
- Funciones de PAM: `man pam_authenticate`, `man pam_start`
- ArchWiki PAM: https://wiki.archlinux.org/title/PAM
- Linux-PAM Project: http://www.linux-pam.org/
- Documentación de pam_appl.h: https://linux-pam.github.io/Linux-PAM_SAG/
