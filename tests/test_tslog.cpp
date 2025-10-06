#include "../libtslog/tslog.h"
#include <thread>
#include <vector>
#include <string>

// Função executada por cada thread para gerar logs
void log_worker(int thread_id) {
    for (int i = 0; i < 15; ++i) {
        std::string msg = "Log da Thread " + std::to_string(thread_id) + ", mensagem #" + std::to_string(i);
        TSLOG_INFO(msg);

        // Simula um trabalho rápido
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
}

int main() {
    // Configura a biblioteca para logar no arquivo "test.log"
    // Para logar no console, comente a linha abaixo.
    Logger::getInstance().configure("test.log");

    TSLOG_INFO("Teste de logging concorrente iniciado.");

    const int num_threads = 10;
    std::vector<std::thread> threads;

    // Cria e inicia as threads
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(log_worker, i + 1);
    }

    // Aguarda a finalização de todas as threads
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    TSLOG_INFO("Teste de logging concorrente finalizado.");
    TSLOG_ERROR("Exemplo de uma mensagem de erro no final do teste.");

    return 0;
}