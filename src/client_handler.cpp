#include "client_handler.h"
#include "server.h"
#include <unistd.h>
#include <sys/socket.h>
#include <iostream>

ClientHandler::ClientHandler(int socket, Server* server) 
    : m_socket(socket), m_server(server), m_running(false) {}

ClientHandler::~ClientHandler() {
    stopHandling();
    if (m_socket >= 0) {
        close(m_socket);
    }
}

void ClientHandler::startHandling() {
    m_running = true;
    m_thread = std::thread(&ClientHandler::handleClientCommunication, this);
    m_thread.detach(); // O servidor gerencia o ciclo de vida
}

void ClientHandler::stopHandling() {
    m_running = false;
}

void ClientHandler::sendMessage(const std::string& message) {
    send(m_socket, message.c_str(), message.length(), 0);
}

int ClientHandler::getSocket() const {
    return m_socket;
}

void ClientHandler::handleClientCommunication() {
    char buffer[4096];
    std::string client_name = "Cliente " + std::to_string(m_socket);

    while (m_running) {
        int bytes_received = recv(m_socket, buffer, sizeof(buffer), 0);
        
        if (bytes_received <= 0) {
            TSLOG_INFO(client_name + " desconectado.");
            m_running = false;
            break;
        }

        buffer[bytes_received] = '\0';
        std::string received_msg = client_name + ": " + std::string(buffer);
        m_server->broadcastMessage(received_msg, m_socket);
    }

    m_server->removeClient(m_socket);
    close(m_socket);
    m_socket = -1;
}