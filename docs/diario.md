# Diário do experimento com IA

Este registro foi escrito pelo agente de IA e descreve ações reais. Não é um
relato pessoal de aprendizagem do estudante. O solicitante autorizou executar
o trabalho integralmente e publicar no GitHub para uma pesquisa sobre IA.
Isso altera o protocolo didático original: a Parte 1 também é produzida por IA,
e as etapas não dependem de novas mensagens de aprovação. AGENTS.md e os testes
fornecidos são preservados; prevalece a autorização explícita desta conversa
para implementar, documentar e fazer os commits da entrega.

## Ambiente

- Ferramenta: Codex no ChatGPT Work; OpenCode não foi utilizado.
- Modelo: agente GPT disponibilizado pela sessão; versão exata não verificada.
- Linguagem: C++11, somente biblioteca padrão; compilador g++ do ambiente.
- Insumos: astronautas.zip, enunciado e três PDFs de aulas fornecidos.
- Dados dos cenários: fictícios, conforme o projeto-base.

## Parte 1: antes de programar

- Astronauta guarda CPF, nome, idade e indicadores de vida e disponibilidade.
- Voo guarda código, estado e CPFs. Não armazena cópias de astronautas.
- Agencia guarda os dois vetores e coordena as validações e as mudanças.
- Lançar exige localizar o voo, conferir seu estado e validar todos os
  tripulantes antes de alterar qualquer objeto. Só então embarca todos e muda
  o estado do voo. Isso evita embarque parcial quando um integrante falha.
- Ponto de atenção técnico: estar num voo planejado não torna alguém ocupado;
  o bloqueio só acontece no lançamento.
- Não houve desenho manual nem dúvida pessoal do estudante registrada.

## Parte 1: uso de IA

O pedido real foi produzir o trabalho autonomamente, com base nos arquivos de
apoio, e publicá-lo em astronautas-lp1. A IA escreveu as três classes e ligou
os comandos do esqueleto. As buscas retornam índices ou -1, sem ponteiros.
Os construtores inicializam os estados e os métodos públicos alteram os
atributos privados. O programa-base compilou e imprimiu TODO como previsto.

Resultado: compilação sem avisos e 6/6 testes da Parte 1 aprovados.
Falha operacional real: uma tentativa de compilação foi executada na pasta
pai e não encontrou src/main.cpp. O diretório de execução foi corrigido;
não houve mudança de requisitos nem de testes para resolver isso.

## Primeiro contato: revisão antes da edição

O sistema já cadastra pessoas e voos e controla as transições. A revisão da
IA identificou três melhorias possíveis:

1. Marcar consultas com const: `int getCodigo() const` deixa explícito que
   consultar um código não modifica o objeto.
2. Separar classes em arquivos: `Astronauta.hpp` poderia conter a declaração,
   facilitando a navegação quando o projeto crescer.
3. Centralizar estados: constantes para `planejado` reduziriam erros de digitação.

A IA escolheu somente a primeira, por ser pequena e manter o formato das aulas.
Não houve escolha nem declaração de aprendizagem atribuída ao estudante.
Plano: marcar getters, buscas, verificações e listagens com const; manter os
métodos que alteram objetos sem const. Testar novamente a Parte 1.
Resultado da revisão aplicada: 6/6 testes da Parte 1 aprovados, sem avisos.
Conceito explicado: const protege contra alterações acidentais dentro de
métodos de consulta; não congela permanentemente o objeto.

## Missão 1: listagem e histórico

Pedido: autorização geral da conversa, sem mensagens individuais fictícias.
Plano da IA: acrescentar listarAstronautas e historico em Agencia, ligar os
dois comandos em main e consultar os voos para achar o código em curso.
Histórico considera apenas voos não planejados e mantém a ordem de cadastro.
Não houve nova intervenção do usuário no plano.
Resultado: Missão 1 2/2; Parte 1 6/6. Não foi necessário refazer a implementação.

## Missão 2: salvar e carregar

Pedido: continuação da autorização geral para concluir as missões.
Plano: usar fstream para gravar um cabeçalho de versão, os astronautas e os
voos em texto. Nomes e estados ocupam linhas próprias. Ler em uma Agencia
temporária e substituir os dados atuais apenas depois de validar tudo.
Os estados são reconstruídos pelos construtores e métodos já existentes.
Também serão rejeitados arquivos incompletos ou incoerentes, mantendo a
mesma mensagem de erro de carregamento e preservando a agência atual.

Primeiras cinco linhas reais do arquivo produzido pelo teste 01:

```text
ASTRONAUTAS_V1
3
111
Ana Maria
30 1 1
```

O cabeçalho identifica o formato. O 3 indica a quantidade de astronautas.
Depois vêm CPF, nome e idade/vivo/disponível. Após os astronautas, vêm a
quantidade de voos e, para cada voo, código, estado, quantidade e seus CPFs.
Resultado: Missão 2 3/3; Parte 1 6/6. Não foi necessário refazer o código.
