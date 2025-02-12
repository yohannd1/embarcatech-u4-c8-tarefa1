# Tarefa U4.C8.1

Esta tarefa envolveu a criação de um programa para o BitDogLab que
responde primariamente ao joystick, disponível nas portas GPIO 22/26/27.
Foi utilizado em especial o módulo ADC (Analog-Digital Converter) do
RP2040 para possibilitar a leitura dos dados dos potenciômetros que
representam os eixos X (GPIO 27) e Y (GPIO 26) do joystick.

## Execução

Para rodar este código, é possível utilizar o CMake para a compilação,
enviando depois o arquivo `.uf2` para a placa via USB. Com isso, o
programa irá rodar na placa, tendo então as opções seguintes para
interação:

- Manipular o eixo X do joystick para controlar o brilho do LED vermelho
    central;

- Manipular o eixo Y do joystick para controlar o brilho do LED azul
    central;

- Apertar o botão do joystick para ativar/desativar o LED verde, e
    trocar o estilo da borda no display OLED;

- Apertar o botão A para ativar/desativar o brilho dos LEDs
    azul e vermelho (invalidando as instruções anteriores);

Durante isso, também há um retângulo visível no display OLED,
centralizado no meio e variando de acordo com a posição do joystick.
