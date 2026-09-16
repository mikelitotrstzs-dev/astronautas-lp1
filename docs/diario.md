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
