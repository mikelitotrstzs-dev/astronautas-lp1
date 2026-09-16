#!/usr/bin/env python3
"""Verificacoes extras de integracao; executar depois de compilar agencia."""
from pathlib import Path
import subprocess
import tempfile

RAIZ = Path(__file__).resolve().parents[1]
EXECUTAVEL = RAIZ / "agencia"


def executar(comandos, pasta):
    resultado = subprocess.run(
        [str(EXECUTAVEL)], input=comandos, text=True, cwd=pasta,
        capture_output=True, timeout=10, check=True,
    )
    assert not resultado.stderr, resultado.stderr
    return resultado.stdout


with tempfile.TemporaryDirectory() as pasta:
    # Falha no segundo tripulante nao pode ocupar o primeiro.
    saida = executar("""CADASTRAR_ASTRONAUTA 111 30 Ana Teste
CADASTRAR_ASTRONAUTA 222 31 Beto Teste
CADASTRAR_VOO 10
CADASTRAR_VOO 20
CADASTRAR_VOO 30
ADICIONAR_ASTRONAUTA 222 10
LANCAR_VOO 10
ADICIONAR_ASTRONAUTA 111 20
ADICIONAR_ASTRONAUTA 222 20
LANCAR_VOO 20
ADICIONAR_ASTRONAUTA 111 30
LANCAR_VOO 30
SALVAR ativo.txt
FIM
""", pasta)
    assert "ERRO: astronauta 222 esta indisponivel\n" in saida
    assert "OK: voo 30 lancado\n" in saida
    print("OK: lancamento falho preserva disponibilidade")

    saida = executar("""CARREGAR ativo.txt
LANCAR_VOO 20
FINALIZAR_VOO 10
FINALIZAR_VOO 30
LANCAR_VOO 20
HISTORICO 111
FIM
""", pasta)
    assert "ERRO: astronauta 111 esta indisponivel\n" in saida
    assert "OK: voo 20 lancado\n" in saida
    assert saida.endswith("HISTORICO DE 111 Ana Teste\nvoo 20: em curso\nvoo 30: finalizado com sucesso\n")
    print("OK: restaura voos em curso e preserva ordem do historico")

    arquivos_invalidos = [
        "ASTRONAUTAS_V1\n1\n111\nNome Teste\n",  # incompleto
        "ASTRONAUTAS_V1\n0\n1\n10\nplanejado\n1\n999\n",  # CPF desconhecido
        "ASTRONAUTAS_V1\n1\n111\nNome Teste\n30 1 0\n0\n",  # ocupado sem voo
        "ASTRONAUTAS_V1\n0\n0\nconteudo_extra\n",
    ]
    for conteudo in arquivos_invalidos:
        Path(pasta, "invalido.txt").write_text(conteudo)
        saida = executar("""CADASTRAR_ASTRONAUTA 777 25 Pessoa Teste
CARREGAR invalido.txt
HISTORICO 777
FIM
""", pasta)
        assert saida == (
            "OK: astronauta 777 cadastrado\n"
            "ERRO: nao foi possivel carregar de invalido.txt\n"
            "HISTORICO DE 777 Pessoa Teste\n(nenhum voo)\n"
        )
    print("OK: quatro arquivos invalidos rejeitados sem perder dados")

    saida = executar("""SALVAR vazio.txt
CADASTRAR_VOO 99
CARREGAR vazio.txt
TRIPULACAO 99
SALVAR pasta_inexistente/dados.txt
XYZ
FIM
""", pasta)
    assert saida == (
        "OK: dados salvos em vazio.txt\nOK: voo 99 cadastrado\n"
        "OK: dados carregados de vazio.txt\nERRO: voo 99 nao cadastrado\n"
        "ERRO: nao foi possivel salvar em pasta_inexistente/dados.txt\n"
        "ERRO: comando desconhecido XYZ\n"
    )
    print("OK: agencia vazia, erro de escrita e comando desconhecido")
