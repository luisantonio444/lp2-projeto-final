#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include "client_handler.h"
#include "../libtslog/tslog.h"

// Forward declaration para evitar dependência circular
class ClientHandler;

class Server {
public:
    Server(int port);
    ~Server();

    void start();
    void stop();

    // Transmite uma mensagem para todos os clientes conectados
    void broadcastMessage(const std::string& message, int sender_socket);
    
    // Remove um cliente da lista de clientes ativos
    void removeClient(int client_socket);

private:
    void acceptConnections(); // Loop para aceitar novas conexões

    int m_port;
    int m_server_socket;
    bool m_running;
    std::vector<std::unique_ptr<ClientHandler>> m_clients;
    std::mutex m_clients_mutex; // Mutex para proteger a lista m_clients
    std::thread m_accept_thread;
};

#endif // SERVER_H