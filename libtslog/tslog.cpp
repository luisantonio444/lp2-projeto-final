#include "tslog.h"

Logger& Logger::getInstance() {
    static Logger instance; // Instância estática, criada de forma segura na primeira chamada
    return instance;
}

Logger::Logger() : m_logToConsole(true) {}

Logger::~Logger() {
    if (m_logfile.is_open()) {
        m_logfile.close();
    }
}

void Logger::configure(const std::string& filename) {
    std::lock_guard<std::mutex> lock(m_mutex);
    if (m_logfile.is_open()) {
        m_logfile.close();
    }

    if (!filename.empty()) {
        m_logfile.open(filename, std::ios::out | std::ios::app);
        m_logToConsole = false;
        if (!m_logfile.is_open()) {
            std::cerr << "ERRO: Nao foi possivel abrir o arquivo de log: " << filename << std::endl;
            m_logToConsole = true; // Retorna a logar no console se falhar
        }
    } else {
        m_logToConsole = true;
    }
}

void Logger::log(LogLevel level, const std::string& message) {
    // ---- INÍCIO DA CORREÇÃO ----

    // 1. Monta a string de log COMPLETA fora do lock para operações que não precisam dele.
    //    No entanto, localtime não é thread-safe, então faremos tudo dentro do lock por segurança.
    
    // Adquire o lock. Apenas uma thread pode executar este bloco por vez.
    std::lock_guard<std::mutex> lock(m_mutex);

    // Formata o timestamp (a chamada a std::localtime agora está protegida pelo lock)
    auto now = std::chrono::system_clock::now();
    auto time_t_now = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t_now), "%Y-%m-%d %H:%M:%S");
    
    // PONTO-CHAVE 1: A mensagem de log inteira é montada em uma única variável string.
    std::string full_message = "[" + ss.str() + "] [" + levelToString(level) + "] " + message;

    // PONTO-CHAVE 2: A string completa é escrita no stream de UMA SÓ VEZ.
    // Isso garante que a operação de escrita é atômica e não será interrompida.
    if (m_logToConsole) {
        std::ostream& output = (level == LogLevel::ERROR) ? std::cerr : std::cout;
        output << full_message << std::endl;
    } else if (m_logfile.is_open()) {
        m_logfile << full_message << std::endl;
    }

    // O lock é liberado automaticamente quando 'lock' sai de escopo.
    // ---- FIM DA CORREÇÃO ----
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warn(const std::string& message) {
    log(LogLevel::WARN, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERROR: return "ERROR";
        default:              return "UNKNOWN";
    }
}