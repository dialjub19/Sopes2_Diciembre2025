#include "crow.h"
#include <unistd.h>
#include <sys/syscall.h>

// Definición de tu syscall
#define SYS_CPU_USAGE 551

int main() {
    crow::SimpleApp app;

    // Endpoint: /stats
    CROW_ROUTE(app, "/stats")([](){
        int cpu_usage = 0;
        
        // Ejecutamos la syscall
        long res = syscall(SYS_CPU_USAGE, &cpu_usage);

        if (res != 0) {
            // Si la syscall falla, devolvemos un error 500
            return crow::response(500, "Error al ejecutar la syscall");
        }

        // Cálculos
        // Suponiendo que cpu_usage viene en formato XXXX (ej. 1500 = 15.00%)
        double usage_percentage = cpu_usage / 100.0;
        double idle_percentage = 100.0 - usage_percentage;

        // Construimos el JSON de respuesta
        crow::json::wvalue response;
        response["cpu_usage_percent"] = usage_percentage;
        response["cpu_idle_percent"] = idle_percentage;
        response["raw_value"] = cpu_usage;

        return crow::response(response);
    });

    app.port(18080).multithreaded().run();
}