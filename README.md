# My-STM32-IR-API
This project aims to consolidate the lessons of the UFMG "Embedded Systems Programming" course about the development of microcontrollers APIs, in this case for STM32 microcontrollers.

## Hardware
Para testar essa API você vai precisar de um sensor de IR. Durante o desenvolvimento utilizamos uma Shield Multifunções para Arduino, de forma a simplificar a montagem (inclusive o desenvolvimento da aplicação de exemplo). Os pinos e componentes da [shield](https://www.eletrogate.com/shield-multifuncoes-para-arduino-com-sensores) usada você pode ver na imagem abaixo:

<p align="center">
  <img src="images/shield.png" width="500" title="hover text">
</p>

Essa API foi desenvolvida e testada para o [Kit SMT32 Nucleo-G474RE](https://www.st.com/en/microcontrollers-microprocessors/stm32g474re.html#overview) você pode conferir todas as características desse kit no link disponibilizado.

## IR Protocol

A API desenvolvida é baseada em um protocolo de comunicação IR chamado NEC. Esse protocolo é um dos mais utilizados comercialmente e apresenta um baixo nível de complexidade e, como esperado, trabalha em cima de uma portadora com frequência de 38khz. Basicamente um comando enviado nesse protocolo segue o seguinte padrão:

- Um pulso de 9ms de largura seguido de um espaço de 4.5ms indica que um comando será enviado (bit de start do protocolo)
- Na sequência serão enviados 32 pulsos, que compõe um comando de 32 bits de comprimento organizados da seguinte forma:
	- 8 bits correspondentes ao endereço do dispositivo
	- 8 bits correspondentes ao endereço do dispositivo com a lógica invertida
	- 8 bits correspondentes ao comando enviado
	- 8 bits correspondentes ao comando enviado com a lógica invertida
	- Um pulso de 562.5µs que indica que o comando acabou
- Um pulso de 562.5µs seguido de um espaço de também 562.5µs indica o envio de um bit 0
- Um pulso de 562.5µs seguido de um espaço de 1.6875ms indica o recebimento de um bit 1

Um exemplo de um sinal contendo um comando usando esse protocolo pode ser observado na imagem abaixo:

<p align="center">
  <img src="images/nec_protocol.png" width="500" title="hover text">
</p>

Além desses comando simples, caso o mesmo comando seja enviado repetidas vezes (por exemplo, mantendo o botão de um controle remoto pressionado), o sinal transmitido que indica a repetição do comando está representado na imagem abaixo e é composto por:

- Um pulso de 9ms, que indica o início de um novo comando, porém seguido por um espaço de apenas 2.25ms dessa vez, indicando que se trata de um comando repetido
- Um pulso de  562.5µs que indica que o comando acabou

<p align="center">
  <img src="images/nec_rep_protocol.png" width="500" title="hover text">
</p>

## API
