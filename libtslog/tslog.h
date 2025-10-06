#ifndef TSLOG_H
#define TSLOG_H

#include <iostream>
#include <fstream>
#include <string>
#include <mutex>
#include <chrono>
#include <iomanip>

// Define os níveis de severidade do log
enum class LogLevel {
    INFO,
    WARN,
    ERROR
};

// Classe Logger implementada com o padrão Singleton
class Logger {
public:
    // Retorna a única instância da classe
    static Logger& getInstance();

    // Configura o destino do log (arquivo ou console)
    void configure(const std::string& filename = "");

    // Método principal para registrar uma mensagem
    void log(LogLevel level, const std::string& message);

    // Métodos de atalho para os diferentes níveis
    void info(const std::string& message);
    void warn(const std::string& message);
    void error(const std::string& message);

private:
    Logger();
    ~Logger();

    // Impede a cópia e a atribuição para manter o Singleton
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    std::string levelToString(LogLevel level);

    std::mutex m_mutex;         // Mutex para garantir exclusão mútua
    std::ofstream m_logfile;    // Stream para o arquivo de log
    bool m_logToConsole;        // Flag para indicar se o log é no console
};

// Macros para facilitar o uso da biblioteca no código cliente
#define TSLOG_INFO(message)  Logger::getInstance().info(message)
#define TSLOG_WARN(message)  Logger::getInstance().warn(message)
#define TSLOG_ERROR(message) Logger::getInstance().error(message)

#endif // TSLOG_H