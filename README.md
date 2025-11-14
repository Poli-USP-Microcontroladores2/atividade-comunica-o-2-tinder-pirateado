# PSI-Microcontroladores2-Aula10
Atividade: Comunicação UART

# Projeto UART – Atividade em Duplas (Echo Bot + Async API)

## 1. Informações Gerais

* Dupla:

  * Integrante 1: Bruno Ferreira 
  * Integrante 2: Samuel Tavares

* Objetivo: implementar, testar e documentar aplicações de comunicação UART baseadas nos exemplos oficiais “echo_bot” e “async_api”, utilizando desenvolvimento orientado a testes, diagramas de sequência D2 e registro de evidências.

---

# 2. Estrutura Esperada do Repositório

```
README.md
src/

docs/
  evidence/
  sequence-diagrams/

```

---

# 3. Etapa 1 – Echo Bot (UART Polling/Interrupt)

## 3.1 Descrição do Funcionamento

Descrever aqui de forma textual o comportamento esperado baseado no exemplo oficial.
Link usado como referência:
[https://docs.zephyrproject.org/latest/samples/drivers/uart/echo_bot/README.html](https://docs.zephyrproject.org/latest/samples/drivers/uart/echo_bot/README.html)

## 3.2 Casos de Teste Planejados (TDD)

### CT1 – Eco básico

* Entrada: Teste 1 
* Saída esperada: Teste 1
* Critério de Aceitação: Ao enviar a mensagem Teste 1, o monitor serial deve printar exatamente a mesma mensagem

### CT2 – Linha vazia

* Entrada: Acionar a tecla ENTER
* Saída esperada: Uma mensagem vazia
* Critério de Aceitação: Ao acionar a tecla ENTER, o monitor serial não mandará mensagem alguma por causa da linha de código rx_buf_pos > 0.

### CT3 – Linha longa

* Entrada: Uma mensagem com mais de 31 caracteres. Ex "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
* Saída esperada: A mesma mensagem, porém com os caracteres maiores que 31 excluídos. Nesse caso, ABCDEFGHIJKLMNOPQRSTUVWXYZ01234
* Critério de Aceitação: O sistema deve truncar o texto ao tamanho máximo do rx_buf (MSG_SIZE - 1), ignorando o restante sem travar nem corromper a fila.

### CT4 - Caracteres especiais

* Entrada: @#%&*!~çÇáÁêÊ
* Saída esperada: Echo: @#%&*!~çÇáÁêÊ
* Critério de Aceitação: O sistema deve ecoar corretamente todos os caracteres especiais e acentuados, preservando sua integridade e ordem, sem substituí-los por símbolos incorretos (como “?”) e sem causar travamento no envio.

## 3.3 Implementação

* Arquivo(s) modificados:
* Justificativa das alterações:

## 3.4 Evidências de Funcionamento

Sent utf8 encoded message: "Teste 1\r" ----
Echo: Teste 1 

Sent utf8 encoded message: "\r" ----


Sent utf8 encoded message: "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789\r" ----
Echo: ABCDEFGHIJKLMNOPQRSTUVWXYZ01234

Sent utf8 encoded message: "@#%&*!~çÇáÁêÊ\r" ----
Echo: @#%&*!~çÇáÁêÊ
```

## 3.5 Diagramas de Sequência D2

Arquivo "uart_diagram.d2"

---

# 4. Etapa 2 – Async API (Transmissão/Recepção Assíncrona)

## 4.1 Descrição do Funcionamento

Descrever o comportamento esperado de forma textual, especialmente com a alternância TX/RX.
Link usado como referência:
[https://docs.zephyrproject.org/latest/samples/drivers/uart/async_api/README.html](https://docs.zephyrproject.org/latest/samples/drivers/uart/async_api/README.html)

## 4.2 Casos de Teste Planejados (TDD)

✅ CT1 – Transmissão de pacotes a cada 5 s

Objetivo:
Verificar se o dispositivo envia corretamente a mensagem "Arerê o Santos vai jogar a Série B" durante o período ativo de transmissão.

Procedimento:

Conecte a FRDM-KL25Z ao PC e abra o monitor serial (por exemplo, 115200 bps).

Reinicie a placa.

Observe no terminal:

O log "[Modo Envio - 5s]" aparece.

Durante cerca de 5 s, são transmitidas várias linhas com o texto definido.

Após esse tempo, o log muda para "[Modo Recepção - 5s]".
Nenhum envio deve ocorrer nesse período.

Resultado esperado:

Pelo menos 8–10 mensagens do texto são enviadas (uma a cada 0,5 s).

Não há envio fora da janela de 5 s.

O ciclo recomeça após 10 s (envio → recepção → envio...).

✅ CT2 – Recepção

Objetivo:
Confirmar se o firmware recebe corretamente mensagens UART e as imprime durante o modo de recepção.

Procedimento:

Aguarde o log "[Modo Recepção - 5s]".

Digite no terminal serial uma mensagem curta (por exemplo, "teste123").

Pressione Enter.

Resultado esperado:
O firmware responde imediatamente com:

Recebido: teste123


Cada linha enviada manualmente deve gerar uma resposta com o prefixo "Recebido:".

Durante o modo de envio, as mensagens digitadas não devem ser respondidas (porque o firmware está transmitindo, não lendo).

✅ CT3 – Verificação de timing dos 5 s

Objetivo:
Validar que cada modo (envio e recepção) dura aproximadamente 5 s.

Procedimento:

Observe no terminal os logs alternando entre:

[Modo Envio - 5s]
[Modo Recepção - 5s]


Use um cronômetro (ou registre timestamps no monitor serial).

Meça o tempo entre dois logs consecutivos do mesmo tipo (por exemplo, entre dois “[Modo Envio - 5s]”).

Resultado esperado:

Cada fase dura 5 ± 0.2 s.

O ciclo completo (envio + recepção) dura aprox. 10 s.

As transições são regulares e contínuas.

(Adicionar mais casos se necessário.)

## 4.3 Implementação

* Arquivos modificados:
* Motivos/Justificativas:

## 4.4 Evidências de Funcionamento
CT1:
[Iniciando transmissão - 5s]
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
[Transmissão encerrada - 10 mensagens]
[Pausa de 5s - sem transmissão]
```
CT2: [Mensagem recebida]: Clube de regatas vasco da gama

CT3: [CICLO 2] MODO: RECEPÇÃO
[Tempo real do ciclo]: 5008 ms

[CICLO 3] MODO: TRANSMISSÃO
Arer�� o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a S��rie B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
Arerê o Santos vai jogar a Série B
[Tempo real do ciclo]: 5005 ms
## 4.5 Diagramas de Sequência D2

Arquivo "UART_Async_flow.d2"

---

# 5. Conclusões da Dupla

* O que deu certo:
* O que foi mais desafiador:
