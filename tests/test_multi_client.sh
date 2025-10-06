#!/bin/bash

SERVER_IP="127.0.0.1"
PORT="8080"
NUM_CLIENTS=3

echo "Iniciando $NUM_CLIENTS clientes..."

for i in $(seq 1 $NUM_CLIENTS)
do
    gnome-terminal --title="Cliente $i" -- ./bin/client $SERVER_IP $PORT
    sleep 0.5
done

echo "$NUM_CLIENTS clientes iniciados em novas janelas de terminal."