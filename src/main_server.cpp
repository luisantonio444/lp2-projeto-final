#include "server.h"
#include <iostream>

const int PORT = 8080;

int main() {
    try {
        Server chat_server(PORT);
        chat_server.start();
    } catch (const std::runtime_error& e) {
        TSLOG_ERROR("Excecao fatal no servidor: " + std::string(e.what()));
        return 1;
    }
    return 0;
}