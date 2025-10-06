#ifndef CLIENT_HANDLER_H
#define CLIENT_HANDLER_H

#include <string>
#include <thread>

class Server; // Forward declaration

// Responsável por gerenciar a comunicação com um único cliente
class ClientHandler {
public:
    ClientHandler(int socket, Server* server);
    ~ClientHandler();

    void startHandling();
    void stopHandling();

    void sendMessage(const std::string& message);
    int getSocket() const;

private:
    void handleClientCommunication(); // Loop de recebimento de mensagens

    int m_socket;
    Server* m_server; // Ponteiro para o servidor (para broadcast e remoção)
    std::thread m_thread;
    bool m_running;
};

#endif // CLIENT_HANDLER_H