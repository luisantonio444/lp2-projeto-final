CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -I./include -I./libtslog
LDFLAGS = -lpthread

OBJ_DIR = obj
BIN_DIR = bin
LIB_DIR = libtslog
TEST_DIR = tests

# Fontes e Objetos
LIB_SRCS = $(LIB_DIR)/tslog.cpp
TEST_SRCS = $(TEST_DIR)/test_tslog.cpp
LIB_OBJS = $(patsubst $(LIB_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(LIB_SRCS))
TEST_OBJS = $(patsubst $(TEST_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(TEST_SRCS))

TARGET_TEST = $(BIN_DIR)/test_tslog

.PHONY: all clean run_test

# Alvo principal
all: $(TARGET_TEST)

# Regra para criar o executável de teste
$(TARGET_TEST): $(LIB_OBJS) $(TEST_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	@echo "Executavel de teste '$(TARGET_TEST)' criado com sucesso."

# Regra genérica para compilar arquivos .cpp em .o no diretório obj
$(OBJ_DIR)/%.o: $(LIB_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(TEST_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Alvo para executar o teste
run_test: all
	@echo "--- Executando teste de log concorrente ---"
	./$(TARGET_TEST)
	@echo "--- Teste finalizado. Verifique o arquivo test.log ---"

# Alvo para limpar os arquivos gerados
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) test.log