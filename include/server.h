#ifndef SERVER_H
#define SERVER_H

#include <vector>
#include <thread>
#include <mutex>
#include <memory>
#include <string>
#include <deque> // Adicionado para o histórico
#include "client_handler.h"
#include "../libtslog/tslog.h"

class ClientHandler;

class Server {
public:
    Server(int port);
    ~Server();

    void start();
    void stop();

    void broadcastMessage(const std::string& message, int sender_socket);
    void removeClient(int client_socket);

private:
    void acceptConnections();
    void sendHistoryToClient(int client_socket); // Novo método

    int m_port;
    int m_server_socket;
    bool m_running;
    
    // Estruturas compartilhadas
    std::vector<std::unique_ptr<ClientHandler>> m_clients;
    std::mutex m_clients_mutex;

    std::deque<std::string> m_message_history;
    std::mutex m_history_mutex;
    const size_t MAX_HISTORY_SIZE = 20; // Guardar as últimas 20 mensagens
    
    std::thread m_accept_thread;
};

#endif // SERVER_H