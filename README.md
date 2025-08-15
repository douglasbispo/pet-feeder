# Alimentador Automático de Pets 

Este é um projeto de um alimentador automático para pets, controlado por um **ESP32** usando o framework **ESP-IDF**. O objetivo é oferecer controle flexível para o usuário.

O alimentador pode ser acionado de duas maneiras:
-   **Remotamente:** Usando um aplicativo web que envia comandos via **MQTT**.
-   **Localmente:** Através de um botão físico conectado ao ESP32.

O projeto também inclui um LED que indica quando o ciclo de alimentação está em andamento. O dispositivo se comunica com um servidor MQTT, onde publica seu estado (como `aberto` e `fechado`), permitindo o monitoramento em tempo real. O mecanismo de alimentação é operado por um **servo motor SG-5010**.

O código foi organizado em módulos, facilitando a manutenção e a adição de novas funcionalidades no futuro.

## Funcionalidades

  - **Controle Remoto via MQTT:** Aciona o alimentador de qualquer lugar através de um comando MQTT.
  - **Controle Local:** Um botão físico aciona o ciclo de alimentação.
  - **Sinalização Visual:** Um LED acende durante o ciclo de alimentação do servo.
  - **Feedback de Status:** O dispositivo publica seu status (online, aberto, fechado) em um tópico MQTT.
  - **Mecanismo de Servo:** Utiliza um servo motor para abrir e fechar a tampa do compartimento de ração.

-----

## Como Executar o Projeto

### 1\. Ferramentas Necessárias

Para compilar e carregar o código para o ESP32, você precisará dos seguintes tens:

  - **Software:**
    - **Visual Studio Code:** O editor de código.
    - **Extensão ESP-IDF para VS Code:** A extensão oficial para desenvolvimento com ESP-IDF.
    - **ESP-IDF (versão 5.4.1 ou superior):** O framework de desenvolvimento do ESP32.
    - **Git:** Para clonar o repositório.
    - **MQTT Explorer:** Um cliente MQTT de desktop para monitorar os tópicos.
  - **Hardware:**
    - 1x Placa de desenvolvimento ESP32
    - 1x Servo Motor SG-5010
    - 1x Botão de pressão
    - 1x LED (qualquer cor)
    - 1x Resistor de 330 Ω (ou 220 Ω)
    - 1x Protoboard
    - Cabos Jumper Macho-Macho

### 2\. Configuração do Ambiente

1.  **Instale o ESP-IDF e a Extensão:** Siga a documentação oficial da [Extensão ESP-IDF para VS Code](https://www.google.com/search?q=https://github.com/espressif/vscode-esp-idf-extension%23installation). Certifique-se de escolher a **instalação padrão** ("Standard") para evitar problemas de configuração.
2.  **Clone o Repositório:** Abra o terminal (PowerShell, Git Bash, etc.) e clone este repositório para o seu computador.
    ```bash
    git clone https://docs.github.com/articles/referencing-and-citing-content
    ```

### 3\. Montagem do Circuito

  - **Servo Motor (SG-5010):**
      - **Sinal (Laranja/Amarelo):** Conecte ao **GPIO 12** do ESP32.
      - **VCC (Vermelho):** Conecte a uma fonte de alimentação externa de **5V**.
      - **GND (Marrom/Preto):** Conecte ao **GND** da fonte externa **e** ao **GND** do ESP32 (GND comum).
  - **Botão Físico:**
      - Conecte um lado do botão ao **GPIO 14**.
      - O outro lado do botão conecte ao **GND**. O resistor de pull-up é ativado via software.
  - **LED de Sinalização:**
      - O terminal longo do LED, com um resistor de **330 Ω** em série, conecte ao **GPIO 32**.
      - O terminal curto do LED conecte ao **GND** do ESP32.

### 4\. Configuração do Código

1.  Abra o projeto no VS Code (`projeto` na pasta clonada).
2.  Vá para a pasta `components/wifi_config/`.
3.  Abra o arquivo **`wifi_config.c`** e substitua as credenciais da sua rede Wi-Fi:
    ```c
    #define WIFI_SSID      "nome"
    #define WIFI_PASS      "senha"
    ```
4.  Abra o arquivo **`components/mqtt_client/mqtt_client.c`** e verifique o endereço do broker MQTT. Para testes, o `broker.hivemq.com` é um bom servidor público.
    ```c
    #define MQTT_BROKER_URL "mqtt://broker.hivemq.com"
    ```

### 5\. Compilar e Carregar para o ESP32

Instrução sobre a porta COM e a forma de execução (terminal ou VS Code) eatão separadas em seções.

#### 5.1\. **Encontrando a Porta Serial (COM)**

Primeiro, você precisa identificar a porta serial (COM) do seu ESP32. A forma mais fácil de fazer isso é no VS Code, no rodapé.

1.  Conecte o ESP32 ao computador via USB.
2.  No rodapé do VS Code, no canto inferior direito, você verá a porta serial. Se estiver escrito **`no port`**, clique nele.
3.  Uma lista de portas disponíveis aparecerá no menu de comandos. Selecione a porta correspondente ao seu ESP32 (geralmente `COM` seguido de um número, como `COM5`).

A porta selecionada aparecerá no rodapé do VS Code, confirmando que o ambiente está pronto.

#### 5.2\. **VS Code ou Terminal**

Agora, você pode escolher se prefere usar os comandos do VS Code ou a linha de comando do terminal.

##### **Opção 1: Pelo VS Code (Interface Gráfica)**

1.  No **Command Palette** (`Ctrl + Shift + P`), execute o comando **`ESP-IDF: Full clean project`**.
2.  Em seguida, execute **`ESP-IDF: Build, Flash and Start a Monitor on Your Device`**.
      * A extensão do VS Code irá compilar o projeto, carregar o código para a porta serial que você selecionou e, por fim, abrir o monitor serial para mostrar os logs do ESP32.

##### **Opção 2: Pelo Terminal (Linha de Comando)**

1.  No **Command Palette** (`Ctrl + Shift + P`), execute o comando **`ESP-IDF: Open ESP-IDF Terminal`**.
2.  Execute o comando `idf.py` com o parâmetro `fullclean`:
    ```bash
    idf.py fullclean
    ```
3.  Depois que a limpeza for concluída, execute o comando para flash, substituindo `COMX` pela porta serial que você encontrou:
    ```bash
    idf.py -p COMX flash monitor
    ```
      * O comando `flash monitor` compila, carrega o código e abre o monitor serial em uma única etapa.

O monitor serial deverá mostrar a conexão com o Wi-Fi e com o broker MQTT.

### 6\. Testar o Frontend Web e MQTT

1.  Abra o arquivo `index.html` em um navegador.
2.  Clique em **"Conectar"**. O status deve mudar para "Conectado".
3.  No seu navegador, clique em **"Acionar"** ou, no MQTT Explorer, publique a mensagem **`acionar`** no tópico **`pet_feeder/comando`**.
4.  No MQTT Explorer, assine o tópico **`pet_feeder/status`** para ver as mensagens `online`, `aberto` e `fechado` sendo publicadas pelo ESP32.
