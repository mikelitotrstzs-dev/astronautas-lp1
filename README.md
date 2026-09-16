# Astronautas - LP1

Sistema de agência espacial em C++11: cadastro de astronautas e voos,
controle de tripulações, lançamento, finalização, histórico e persistência.

Implementação produzida com auxilio de IA, o diário
registra a execução do agente.

## Compilar e executar

Requisito: compilador g++ com suporte a C++11. Não há bibliotecas externas.
Na raiz do repositório:

```bash
g++ -std=c++11 -Wall -Wno-sign-compare src/main.cpp -o agencia
./agencia
```

Digite um comando por linha e termine com `FIM`, ou redirecione um arquivo:

```bash
./agencia < testes/parte1/06_cenario_completo.in
```

## Comandos

| Comando | Ação |
| --- | --- |
| `CADASTRAR_ASTRONAUTA cpf idade nome completo` | Cadastra uma pessoa viva e disponível |
| `CADASTRAR_VOO codigo` | Cria um voo planejado |
| `ADICIONAR_ASTRONAUTA cpf codigo` | Inclui uma pessoa num voo planejado |
| `REMOVER_ASTRONAUTA cpf codigo` | Remove uma pessoa de um voo planejado |
| `LANCAR_VOO codigo` | Valida todos e inicia o voo |
| `FINALIZAR_VOO codigo` | Conclui o voo com sucesso |
| `EXPLODIR_VOO codigo` | Registra a perda do voo e dos tripulantes |
| `LISTAR_VOOS` | Agrupa voos pelos quatro estados |
| `LISTAR_MORTOS` | Lista falecidos e seus voos lançados |
| `LISTAR_ASTRONAUTAS` | Agrupa pessoas por situação |
| `HISTORICO cpf` | Mostra os voos lançados com a pessoa a bordo |
| `SALVAR arquivo` | Grava o estado completo em texto |
| `CARREGAR arquivo` | Substitui a agência pelo conteúdo validado |
| `RELATORIO` | Apresenta contagens, experiência e taxa de sucesso |
| `TRIPULACAO codigo` | Consulta um único voo, com idade e situação das pessoas |
| `FIM` | Encerra o programa |

Os nomes aceitam espaços; CPF é texto. O programa pressupõe comandos com os
tipos e argumentos especificados no enunciado. Caminhos de arquivos não devem
conter espaços. Não há validação documental de CPF: use dados fictícios.

## Testar

Os 16 casos originais foram preservados integralmente. Os comandos abaixo
compilam antes de testar; execute-os em sequência, pois compartilham um
arquivo de dados temporário:

```bash
bash testes/testar.sh parte1
bash testes/testar.sh missao1
bash testes/testar.sh missao2
bash testes/testar.sh missao3
bash exemplos/missao4/testar.sh
```

Resultado conferido: Parte 1 **6/6**, Missão 1 **2/2**, Missão 2 **3/3**,
Missão 3 **5/5** e cenário da Missão 4 **aprovado**.

Verificações adicionais opcionais, com Python 3 e o executável já compilado:

```bash
python3 verificacoes/verificar.py
```

Elas verificam lançamento sem alteração parcial, restauração de voos em
curso, arquivos inválidos sem perda de dados, agência vazia e erro de escrita.
O programa principal continua sendo exclusivamente C++11.

## Missão 4: TRIPULACAO

`TRIPULACAO 10` mostra o estado do voo e as pessoas na ordem de inclusão:

```text
TRIPULACAO DO VOO 10
estado: planejado
111 Ana Maria (30 anos) - disponivel
```

Sem tripulantes, a terceira linha é `(nenhum)`. Código inexistente produz
`ERRO: voo 10 nao cadastrado`. A situação da pessoa pode ser `disponivel`,
`em voo` ou `morto`, e reflete o presente, inclusive ao consultar um voo antigo.
A consulta não altera dados. O cenário e sua saída esperada estão em
[exemplos/missao4](exemplos/missao4).

## Organização e aulas

As três classes ficam em [src/main.cpp](src/main.cpp), mantendo a organização
simples do projeto-base:

- `Astronauta`: atributos privados, construtor, getters e mudanças de situação.
- `Voo`: código, estado e vetor de CPFs; não armazena objetos Astronauta.
- `Agencia`: vetores de objetos, buscas, validações e operações coordenadas.

A aula de classes fundamenta a divisão entre dados e métodos. A de
construtores fundamenta a inicialização dos objetos. A de encapsulamento,
herança e polimorfismo fundamenta o acesso controlado aos atributos privados.
Herança e polimorfismo foram considerados, mas não são necessários para
esta missão livre: não há uma hierarquia de tipos de voo. Não se utilizam
ponteiros, `new` ou `delete`.

Veja [o guia do código](docs/guia.md), [o diário do experimento](docs/diario.md)
e [o enunciado fornecido](ENUNCIADO.md). O histórico mantém o commit
`Parte 1 pronta`, seguido da revisão e das missões.

## Arquivo de dados

O formato `ASTRONAUTAS_V1` armazena contagens, astronautas e voos. Nomes e
estados ficam em linhas próprias; detalhes e exemplo em [docs/guia.md](docs/guia.md).
Carregar arquivo inexistente, incompleto ou incoerente preserva a agência
atual. Salvar substitui o arquivo informado; não é um mecanismo de backup.
Não são versionados executáveis nem arquivos gerados pelos testes.
