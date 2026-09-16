# Guia de leitura do código

## Relação com as aulas fornecidas

| Material | Aplicação |
| --- | --- |
| aula_classes_cpp.pdf | Classes reúnem atributos e métodos; objetos são criados e usados pelo programa |
| aula_construtores_cpp.pdf | Construtores com parâmetros deixam cada objeto pronto para uso; passagem por referência aparece na leitura do arquivo |
| aula_heranca_polimorfismo_cpp.pdf | Encapsulamento com private e public; getters consultam sem expor escrita direta |
| Enunciado | vector, buscas por índice, mensagens exatas, separação das três responsabilidades |

Const e fstream complementam esses conceitos nas missões. Não foi necessário
usar todos os recursos apresentados nas aulas. Sem recursos alocados
manualmente, os destrutores padrão de string, vector e streams bastam.
Herança seria adequada se houvesse tipos especializados de voo com regras
diferentes. Aqui todos os voos seguem o mesmo ciclo.

## Responsabilidades

`Astronauta` cuida da própria situação. O construtor inicia vivo e disponível.
`embarcar` ocupa; `desembarcar` libera somente se vivo; `morrer` marca os dois
indicadores como falsos.

`Voo` guarda os CPFs na ordem de inclusão. Seus métodos de transição apenas
mudam o estado. `Agencia` é responsável por permitir ou rejeitar a operação.
As buscas devolvem índices do vector, ou -1 quando não encontram o cadastro.

## Por que o lançamento usa dois laços?

No primeiro laço a agência verifica cada pessoa. No segundo, embarca todas.
Se a primeira pessoa estiver livre e a segunda ocupada, a validação termina
sem ocupar a primeira. O voo permanece planejado. A verificação de morte
vem antes da indisponibilidade porque uma pessoa morta também está ocupada
no sentido do indicador, mas a mensagem exigida é a de morte.

## Como reconhecer uma participação?

Um CPF no vetor de um voo planejado é apenas uma previsão. A participação
existe quando o voo já saiu do estado planejado. Por isso histórico, mortos
e experiência ignoram voos planejados. A tripulação não pode ser alterada
após o lançamento, então o próprio voo serve como registro histórico.

## Relatório e empates

A agência conta os quatro estados dos voos. Para cada astronauta, conta os
voos não planejados que contêm seu CPF. Só troca o mais experiente quando
encontra uma contagem maior, preservando o primeiro cadastro num empate.
Uma pessoa morta continua participando desse cálculo.

A taxa usa `sucessos * 100LL / finalizados`. O sufixo LL faz a multiplicação
usar long long, e a divisão entre inteiros descarta a parte fracionária.
Sem finalizações, o programa imprime a mensagem específica e não divide.

## Formato persistido

Exemplo completo de uma pessoa disponível num voo planejado:

```text
ASTRONAUTAS_V1
1
111
Ana Maria
30 1 1
1
10
planejado
1
111
```

Leitura das linhas:

1. Identificador e versão do formato.
2. Quantidade de astronautas.
3. CPF do primeiro astronauta.
4. Nome completo.
5. Idade, vivo (0/1), disponível (0/1).
6. Quantidade de voos, depois de todos os astronautas.
7. Código do primeiro voo.
8. Estado do voo.
9. Quantidade de CPFs nesse voo.
10. CPF do primeiro tripulante.

Cada astronauta ocupa três linhas. Cada voo começa com três linhas, seguidas
de uma linha por CPF. A ordem é a mesma do cadastro. Getline lê nomes e
estados, pois ambos podem conter espaços. Os números e CPFs usam `>>`.

`carregar` abre o arquivo e cria uma agência temporária. `lerDados` valida o
cabeçalho, contagens, flags, duplicidades, estados, referências e coerência
entre disponibilidade e voos em curso. Construtores criam os objetos e seus
métodos restauram os estados. Somente uma leitura válida substitui os vetores
atuais. Erros mantêm os dados anteriores e imprimem a mensagem de falha.

A experiência não fica no arquivo como contador separado: é recalculada
usando os voos restaurados. Isso evita perder ou duplicar contagens.

## Missão livre

`listarTripulacao` localiza um voo, mostra seu estado e resolve cada CPF no
vetor de astronautas. A idade e a situação são atuais. Por exemplo, alguém
pode aparecer como morto na consulta de um voo antigo bem-sucedido, caso
outra missão posterior tenha terminado com explosão.

## Limites do projeto

O foco é o contrato de comandos da atividade. O parser original pressupõe
argumentos válidos, inclusive números onde são esperados. Não há interface
gráfica, banco de dados nem validação oficial de documentos. As buscas
lineares são adequadas ao pequeno volume da atividade, sem pretensão de
escala operacional. O formato de arquivo foi criado para este programa.
