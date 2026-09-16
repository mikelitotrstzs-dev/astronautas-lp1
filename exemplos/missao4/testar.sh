#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")/../.."
g++ -std=c++11 -Wall -Wno-sign-compare src/main.cpp -o agencia
# Executa em pasta temporaria para nao sobrescrever dados do usuario.
raiz="$PWD"
pasta_temporaria="$(mktemp -d)"
trap 'rm -rf "$pasta_temporaria"' EXIT
cd "$pasta_temporaria"
"$raiz/agencia" < "$raiz/exemplos/missao4/01_tripulacao.in" > resultado.out
diff -u "$raiz/exemplos/missao4/01_tripulacao.out" resultado.out
echo 'OK: missao 4 (tripulacao)'
