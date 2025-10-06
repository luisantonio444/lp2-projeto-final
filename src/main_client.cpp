#include <iostream>
#include <string>
#include <thread>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void receive_messages(int sock) {
    char buffer[4096];
    while (true) {
        int bytes_received = recv(sock, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            std::cout << "Desconectado do servidor." << std::endl;
            break;
        }
        buffer[bytes_received] = '\0';
        std::cout << buffer << std::endl;
    }
}

void send_messages(int sock) {
    std::string line;
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            send(sock, line.c_str(), line.length(), 0);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <IP do Servidor> <Porta>" << std::endl;
        return 1;
    }

    std::string server_ip = argv[1];
    int port = std::stoi(argv[2]);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Falha ao criar o socket." << std::endl;
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, server_ip.c_str(), &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Falha ao conectar ao servidor." << std::endl;
        return 1;
    }

    std::cout << "Conectado ao servidor de chat! Digite suas mensagens." << std::endl;

    std::thread receiver_thread(receive_messages, sock);
    std::thread sender_thread(send_messages, sock);

    receiver_thread.join();
    sender_thread.join();

    close(sock);
    return 0;
}