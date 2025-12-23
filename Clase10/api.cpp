#include <unistd.h>
#include <errno.h>
#include <cstring>
#include <crow.h>

// --- Middleware CORS ---
struct CORS {
    struct context {}; // Crow exige un 'context' aunque esté vacío

    void before_handle(crow::request& req, crow::response& res, context&) {
        if (req.method == crow::HTTPMethod::OPTIONS) {
            // Responder preflight inmediatamente
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
            res.add_header("Access-Control-Allow-Methods", "GET,POST,PUT,PATCH,DELETE,OPTIONS");
            res.code = 204; // No Content
            res.end();
        }
    }

    void after_handle(crow::request&, crow::response& res, context&) {
        // Añadir siempre CORS a las respuestas normales
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Headers", "Content-Type, Authorization");
        res.add_header("Access-Control-Allow-Methods", "GET,POST,PUT,PATCH,DELETE,OPTIONS");
    }
};


int main(){
  // Activa el middleware CORS
  crow::App<CORS> app;

  CROW_ROUTE(app, "/")([]{
      return "Hello world from C++;";
  });

  app.port(18080).multithreaded().run();

  return 0;
}

/* 

Compilar: g++ main.cpp -lpthread -lpam -lpam_misc
Ejecutar: sudo ./a.out

*/