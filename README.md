# Controle de LEDs e Display com Joystick utilizando Raspberry Pi Pico W

Este projeto utiliza um Raspberry Pi Pico W para controlar LEDs RGB e um display OLED baseado no movimento de um joystick. O sistema permite interatividade com o display e LEDs por meio do joystick e botões.

## Requisitos

### Hardware:
- Raspberry Pi Pico.
- Joystick analógico com eixo X, eixo Y e botão.
- Botão adicional (botão A).
- LEDs RGB (vermelho, verde e azul).
- Display OLED SSD1306 (resolução 128x64).
- Resistor de pull-up (integrado ou externo, se necessário).

### Software:
- SDK do Raspberry Pi Pico configurado no ambiente de desenvolvimento (C).
- Biblioteca SSD1306 para o controle do display (inclusa no diretório `inc`).

## Configuração do Hardware

Conecte os componentes conforme as conexões abaixo:

### Conexão do Display OLED SSD1306 (I2C):
- **Pino SDA**: GPIO 14 do Raspberry Pi Pico W.
- **Pino SCL**: GPIO 15 do Raspberry Pi Pico W.

### Conexão do Joystick:
- **Eixo X**: GPIO 27 
- **Eixo Y**: GPIO 26
- **Botão do joystick**: GPIO 22

### Conexão do Botão A:
- GPIO 5.

### Conexão dos LEDs RGB:
- **LED Vermelho**: GPIO 13
- **LED Verde**: GPIO 11
- **LED Azul**: GPIO 12

Certifique-se de conectar resistores adequados em série com os LEDs para limitar a corrente.

## Como Compilar e Executar o Código

1. **Instale o SDK do Raspberry Pi Pico**:
   Configure o ambiente de desenvolvimento para compilar e carregar códigos C/C++ no Raspberry Pi Pico. Siga a [documentação oficial](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf).

2. **Clone o Repositório**:
   No terminal do vscode coloque:

   git clone https://github.com/Rayaneqsp/Conversores.git

3. **Compile o Código**:
   - Navegue até o diretório raiz do projeto.
   - Execute o comando `cmake` e `make` para compilar o projeto:
     ```
     mkdir build && cd build
     cmake ..
     make
     ```

4. **Carregue o Arquivo `.uf2` no Raspberry Pi Pico W**:
   - Conecte o Raspberry Pi Pico ao computador enquanto pressiona o botão BOOTSEL.
   - Monte o Pico como uma unidade USB.
   - Arraste o arquivo `.uf2` gerado no diretório `build` para a unidade montada.

5. **Conecte o Hardware e Ligue o Pico**:
   Certifique-se de que todos os componentes estejam conectados corretamente antes de alimentar o Raspberry Pi Pico.

## Funcionalidades

1. **Controle do Quadrado no Display**:
   - O joystick controla a posição de um quadrado exibido no display OLED.
   - O quadrado se move com base nos valores analógicos dos eixos X e Y do joystick.

2. **Estilos de Borda no Display**:
   - Pressione o botão do joystick para alternar entre os estilos de borda:
     - Borda dupla.
     - Borda tripla.

3. **Controle de LEDs RGB**:
   - O LED vermelho é controlado pela posição do eixo X do joystick.
   - O LED azul é controlado pela posição do eixo Y do joystick.
   - O LED verde alterna entre ligado e desligado ao pressionar o botão do joystick.

4. **Ativação/Desativação do PWM**:
   - Pressione o botão A para ativar ou desativar o led RGB.

## Autora
RAYANE QUEIROZ DOS SANTOS PASSOS

## LINK VIDEO: [https://drive.google.com/file/d/1J3YFpolhfOKWG8-v4XROqkEpFUXXNUH9/view?usp=sharing]



