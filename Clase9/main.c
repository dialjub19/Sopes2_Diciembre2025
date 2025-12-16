/* * LIBRERÍAS NECESARIAS 
 */

/* Contiene las funciones principales de la API de PAM (pam_start, pam_authenticate, etc.) */
#include <security/pam_appl.h>
/* Contiene utilidades extra y definiciones de estructuras de punteros */
#include <security/pam_misc.h> 
/* Librería estándar de entrada/salida (printf, scanf) */
#include <stdio.h> 
/* Librería estándar para gestión de memoria (calloc, free, exit) */
#include <stdlib.h> 
/* Librería para manejo de strings (strdup, strcmp) */
#include <string.h> 

/*
 * FUNCIÓN DE CONVERSACIÓN (CALLBACK)
 * Esta es la función que PAM llamará cuando necesite pedirle algo al usuario.
 * PAM no habla directamente con el teclado, usa esta función como intermediario.
 */
static int pam_conv_callback(int num_msg, const struct pam_message **msg, struct pam_response **resp, void *appdata_ptr) {
    
    // 1. Validación de seguridad: Si PAM dice que hay 0 o menos mensajes, es un error.
    if (num_msg <= 0) return PAM_CONV_ERR;

    // 2. Reserva de memoria para las respuestas.
    // Usamos 'calloc' en lugar de 'malloc' porque calloc rellena la memoria con ceros.
    // Esto es crucial para evitar basura en la memoria que pueda confundir a PAM.
    struct pam_response *responses = calloc(num_msg, sizeof(struct pam_response));
    
    // Si calloc falla (devuelve NULL por falta de RAM), devolvemos error a PAM.
    if (!responses) return PAM_CONV_ERR;

    // 3. Recuperamos la contraseña.
    // 'appdata_ptr' es un puntero genérico (void*) que pasamos desde el main.
    // Aquí lo convertimos ("casteamos") a char* para poder leer la contraseña que el usuario escribió antes.
    const char *password = (const char *)appdata_ptr;

    // 4. Procesamos cada mensaje que PAM nos envió.
    for (int i = 0; i < num_msg; i++) {
        
        // Caso A: PAM pide un dato secreto (Contraseña). NO debe mostrarse en pantalla (ECHO_OFF).
        if (msg[i]->msg_style == PAM_PROMPT_ECHO_OFF) { 
            
            // Copiamos la contraseña usando 'strdup'.
            // ¿POR QUÉ STRDUP? Porque PAM espera ser el dueño de esa memoria para poder 
            // liberarla (free) cuando termine. Si pasamos el puntero directo, el programa crasheará al liberar.
            responses[i].resp = strdup(password);

        // Caso B: PAM pide un dato visible (ej. "Ingrese Usuario:"). 
        // Como ya pasamos el usuario en pam_start, no necesitamos responder nada aquí, o devolvemos NULL.
        } else if (msg[i]->msg_style == PAM_PROMPT_ECHO_ON) { 
            responses[i].resp = NULL;
        
        // Caso C: PAM envía un mensaje de error o info que no sabemos manejar.
        } else { 
            // Liberamos la memoria que reservamos antes para no dejar fugas (memory leaks)
            free(responses);
            return PAM_CONV_ERR; // Decimos que hubo un error.
        }
    }

    // 5. Asignamos nuestras respuestas a la estructura que PAM leerá (*resp).
    *resp = responses;
    
    // Devolvemos éxito para que PAM continúe su proceso.
    return PAM_SUCCESS;
}

/*
 * FUNCIÓN PRINCIPAL
 */
int main() {
    // Buffers para guardar lo que escribe el usuario. 
    // 64 bytes es un tamaño estándar seguro para pruebas.
    char username[64];
    char password[64];

    // --- Interfaz de usuario básica ---
    printf("Usuario: ");
    // %63s limita la lectura a 63 caracteres para evitar desbordamiento de búfer (Buffer Overflow).
    scanf("%63s", username);

    printf("Contraseña: ");
    scanf("%63s", password); 
    // NOTA: En un entorno real, deberías usar algo que oculte los caracteres, 
    // como 'getpass()' o manipular 'termios' para desactivar el eco de la terminal.

    // --- Configuración de PAM ---
    
    // Preparamos la estructura de conversación.
    // Le decimos a PAM: "Usa la función 'pam_conv_callback' para preguntar cosas, 
    // y si necesitas datos extra (la contraseña), están en la variable 'password'".
    struct pam_conv conversation = {pam_conv_callback, (void *)password};
    
    // Este puntero guardará el estado de la sesión de PAM. Se inicializa en NULL.
    pam_handle_t *pam_handler = NULL;

    // 1. INICIO DE PAM (pam_start)
    // "login": Es el nombre del servicio. Buscará la config en /etc/pam.d/login
    // username: El usuario que intenta entrar.
    // &conversation: Nuestra estructura de callback.
    // &pam_handler: Donde PAM guardará su ID de sesión.
    int pam_result = pam_start("login", username, &conversation, &pam_handler);

    // Verificamos si PAM pudo iniciar correctamente.
    if (pam_result != PAM_SUCCESS) {
        // pam_strerror convierte el código de error numérico en texto legible.
        fprintf(stderr, "Error iniciando PAM: %s\n", pam_strerror(pam_handler, pam_result));
        return 1;
    }

    // 2. AUTENTICACIÓN (pam_authenticate)
    // Aquí es donde PAM llama a nuestro 'pam_conv_callback', obtiene la password y la verifica.
    pam_result = pam_authenticate(pam_handler, 0); 

    // 3. GESTIÓN DE CUENTA (pam_acct_mgmt)
    // Solo si la contraseña fue correcta, verificamos si la cuenta es válida.
    // Esto revisa: ¿La cuenta expiró? ¿Es hora permitida de login? ¿El usuario está bloqueado?
    if (pam_result == PAM_SUCCESS) {
        pam_result = pam_acct_mgmt(pam_handler, 0); 
    }

    // --- Resultado Final ---
    if (pam_result == PAM_SUCCESS) {
        printf("Autenticación Completa ✅\n");
    } else {
        // Si falló la contraseña o la cuenta, imprimimos por qué.
        fprintf(stderr, "Usuario o contraseña incorrectos ❌: %s\n", pam_strerror(pam_handler, pam_result));
    }

    // 4. CIERRE (pam_end)
    // IMPORTANTÍSIMO: Libera toda la memoria que PAM reservó internamente.
    pam_end(pam_handler, pam_result);
    
    // Retornamos 0 si todo salió bien, 1 si hubo error.
    return (pam_result == PAM_SUCCESS ? 0 : 1);
}