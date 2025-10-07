# Servidor de Chat Multi-Cliente em C++

Este projeto é uma implementação de um servidor de chat TCP multi-cliente em C++, que permite que vários clientes se conectem e troquem mensagens em tempo real. O servidor é construído com threads para lidar com conexões e mensagens simultâneas de forma eficiente.

## Funcionalidades

* **Servidor Multi-Threaded**: Capaz de lidar com múltiplas conexões de clientes simultaneamente, onde cada cliente é gerenciado em sua própria thread.
* **Broadcast de Mensagens**: As mensagens enviadas por um cliente são transmitidas para todos os outros clientes conectados.
* **Histórico de Mensagens**: O servidor mantém um histórico das últimas 20 mensagens trocadas. Novos clientes recebem esse histórico ao se conectarem.
* **Logging Thread-Safe**: Utiliza uma biblioteca de log singleton (`libtslog`) para registrar eventos do servidor de forma segura em um ambiente com múltiplas threads.
* **Gerenciamento de Conexão**: Lida com a conexão e desconexão de clientes de forma robusta.

## Como Compilar

O projeto utiliza um `Makefile` para simplificar o processo de compilação.

**Pré-requisitos:**
* Um compilador C++17 (como o g++)
* `make`

Para compilar o servidor e o cliente, execute o seguinte comando na raiz do projeto:

```bash
make all
```

Isso irá gerar os executáveis server e client no diretório bin/.

Para limpar os arquivos de compilação, execute:

Bash
```
make clean
```
Como Executar
1. Servidor
Para iniciar o servidor (que por padrão rodará na porta 8080), execute um dos seguintes comandos:

Bash
```
make run_server
```
ou

Bash
```
./bin/server
```
2. Cliente
Para conectar um cliente ao servidor, abra um novo terminal e execute:

Bash
```
./bin/client <IP_DO_SERVIDOR> <PORTA>
Por exemplo, para se conectar a um servidor local:
```
Bash
```
./bin/client 127.0.0.1 8080
```
Você pode iniciar múltiplos clientes para testar a comunicação em grupo. O script tests/test_multi_client.sh pode ser usado para iniciar vários clientes de uma vez.


Diagrama de Sequência
O projeto inclui um diagrama de sequência que ilustra o fluxo de comunicação, desde a conexão de um cliente até o broadcast de mensagens.
