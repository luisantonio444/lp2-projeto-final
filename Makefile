CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -I./include -I./libtslog
LDFLAGS = -lpthread

OBJ_DIR = obj
BIN_DIR = bin
SRC_DIR = src
LIB_DIR = libtslog

# Fontes e Objetos
SRCS = $(SRC_DIR)/server.cpp $(SRC_DIR)/client_handler.cpp
LIB_SRCS = $(LIB_DIR)/tslog.cpp
MAIN_SERVER_SRC = $(SRC_DIR)/main_server.cpp
MAIN_CLIENT_SRC = $(SRC_DIR)/main_client.cpp

OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
LIB_OBJS = $(patsubst $(LIB_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(LIB_SRCS))
MAIN_SERVER_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(MAIN_SERVER_SRC))
MAIN_CLIENT_OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(MAIN_CLIENT_SRC))

TARGET_SERVER = $(BIN_DIR)/server
TARGET_CLIENT = $(BIN_DIR)/client

.PHONY: all clean run_server

# Alvo principal
all: $(TARGET_SERVER) $(TARGET_CLIENT)

# Regras para criar os executáveis
$(TARGET_SERVER): $(OBJS) $(LIB_OBJS) $(MAIN_SERVER_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Servidor '$(TARGET_SERVER)' criado com sucesso."

$(TARGET_CLIENT): $(MAIN_CLIENT_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Cliente '$(TARGET_CLIENT)' criado com sucesso."

# Regra genérica para compilar arquivos .cpp em .o no diretório obj
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Alvo para executar o servidor
run_server: all
	@echo "--- Iniciando o Servidor de Chat ---"
	./$(TARGET_SERVER)

# Alvo para limpar os arquivos gerados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) *.log