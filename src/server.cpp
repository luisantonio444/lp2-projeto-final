#include "server.h"
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

Server::Server(int port) : m_port(port), m_running(false) {
    Logger::getInstance().configure("server.log");
    TSLOG_INFO("Iniciando o servidor na porta " + std::to_string(port));

    m_server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_server_socket < 0) {
        throw std::runtime_error("Falha ao criar o socket do servidor.");
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(m_port);

    if (bind(m_server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        throw std::runtime_error("Falha ao fazer o bind do socket na porta " + std::to_string(m_port));
    }
}

Server::~Server() {
    stop();
    if (m_server_socket >= 0) {
        close(m_server_socket);
    }
}

void Server::start() {
    if (listen(m_server_socket, 10) < 0) {
        throw std::runtime_error("Falha ao escutar no socket.");
    }
    m_running = true;
    TSLOG_INFO("Servidor escutando...");

    m_accept_thread = std::thread(&Server::acceptConnections, this);
    m_accept_thread.join(); // Mantém o servidor rodando
}

void Server::stop() {
    m_running = false;
    // Lógica para parar as threads e limpar recursos pode ser adicionada aqui
}

void Server::acceptConnections() {
    while (m_running) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_socket = accept(m_server_socket, (struct sockaddr*)&client_addr, &client_len);

        if (client_socket < 0) {
            TSLOG_WARN("Falha ao aceitar nova conexao.");
            continue;
        }

        TSLOG_INFO("Nova conexao aceita. Socket: " + std::to_string(client_socket));

        std::lock_guard<std::mutex> lock(m_clients_mutex);
        m_clients.push_back(std::make_unique<ClientHandler>(client_socket, this));
        m_clients.back()->startHandling();
    }
}

void Server::broadcastMessage(const std::string& message, int sender_socket) {
    std::lock_guard<std::mutex> lock(m_clients_mutex);
    TSLOG_INFO("Broadcast da mensagem de socket " + std::to_string(sender_socket) + ": " + message);
    for (const auto& client : m_clients) {
        if (client->getSocket() != sender_socket) {
            client->sendMessage(message);
        }
    }
}

void Server::removeClient(int client_socket) {
    std::lock_guard<std::mutex> lock(m_clients_mutex);
    
    auto it = std::remove_if(m_clients.begin(), m_clients.end(), 
        [client_socket](const std::unique_ptr<ClientHandler>& client) {
            return client->getSocket() == client_socket;
        }
    );

    if (it != m_clients.end()) {
        m_clients.erase(it, m_clients.end());
        TSLOG_INFO("Cliente com socket " + std::to_string(client_socket) + " removido.");
    }
}