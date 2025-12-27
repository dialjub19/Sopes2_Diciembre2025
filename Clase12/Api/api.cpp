#include <crow.h>
#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <string>
#include <cstring>

/* ---------------- PAM ---------------- */

static const char* PAM_SERVICE_NAME = "login";

static int pam_conv_cb(int num_msg,
                       const struct pam_message **msg,
                       struct pam_response **resp,
                       void *appdata_ptr)
{
    if (num_msg <= 0) return PAM_CONV_ERR;

    auto *responses =
        (pam_response*)calloc(num_msg, sizeof(pam_response));
    if (!responses) return PAM_CONV_ERR;

    const char *password = (const char *)appdata_ptr;

    for (int i = 0; i < num_msg; i++) {
        switch (msg[i]->msg_style) {
            case PAM_PROMPT_ECHO_OFF:
                responses[i].resp = strdup(password ? password : "");
                responses[i].resp_retcode = 0;
                break;
            case PAM_PROMPT_ECHO_ON:
            case PAM_ERROR_MSG:
            case PAM_TEXT_INFO:
                responses[i].resp = nullptr;
                responses[i].resp_retcode = 0;
                break;
            default:
                free(responses);
                return PAM_CONV_ERR;
        }
    }

    *resp = responses;
    return PAM_SUCCESS;
}

static bool pam_authenticate_user(const std::string& username,
                                  const std::string& password,
                                  std::string* error_out = nullptr)
{
    pam_handle_t* pamh = nullptr;
    struct pam_conv conv { pam_conv_cb, (void*)password.c_str() };

    int r = pam_start(PAM_SERVICE_NAME, username.c_str(), &conv, &pamh);
    if (r != PAM_SUCCESS) {
        if (error_out) *error_out = pam_strerror(pamh, r);
        return false;
    }

    r = pam_authenticate(pamh, 0);
    if (r == PAM_SUCCESS)
        r = pam_acct_mgmt(pamh, 0);

    bool ok = (r == PAM_SUCCESS);
    if (!ok && error_out)
        *error_out = pam_strerror(pamh, r);

    pam_end(pamh, r);
    return ok;
}

/* ---------------- CORS ---------------- */

struct CORS {
    struct context {};

    void before_handle(crow::request& req,
                       crow::response& res,
                       context&)
    {
        if (req.method == crow::HTTPMethod::OPTIONS) {
            res.add_header("Access-Control-Allow-Origin", "*");
            res.add_header("Access-Control-Allow-Headers", "Content-Type");
            res.add_header("Access-Control-Allow-Methods", "POST,OPTIONS");
            res.code = 204;
            res.end();
        }
    }

    void after_handle(crow::request&,
                      crow::response& res,
                      context&)
    {
        res.add_header("Access-Control-Allow-Origin", "*");
        res.add_header("Access-Control-Allow-Headers", "Content-Type");
        res.add_header("Access-Control-Allow-Methods", "POST,OPTIONS");
    }
};

/* ---------------- MAIN ---------------- */

int main()
{
    crow::App<CORS> app;

    CROW_ROUTE(app, "/")([]{
        return "Hello world from C++;";
    });

    CROW_ROUTE(app, "/auth").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req){
        auto json = crow::json::load(req.body);
        if (!json || !json.has("username") || !json.has("password")) {
            return crow::response(400,
                "JSON con 'username' y 'password' requerido");
        }

        std::string username = json["username"].s();
        std::string password = json["password"].s();

        std::string pam_err;
        if (!pam_authenticate_user(username, password, &pam_err)) {
            crow::json::wvalue body;
            body["ok"] = false;
            body["error"] = pam_err;
            return crow::response(401, body);
        }

        crow::json::wvalue body;
        body["ok"] = true;
        body["username"] = username;
        return crow::response(200, body);
    });

    app.port(18080).multithreaded().run();
}

/* g++ api.cpp -o api -lpthread -lpam -lpam_misc */