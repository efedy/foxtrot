# 🏎️ Carrinho de Competição ESP32 (RC Wi-Fi)

Repositório oficial do carrinho de controle remoto desenvolvido para competição, utilizando **ESP32** e uma interface web hospedada diretamente no microcontrolador.

---

## 📋 Sobre o Projeto
O carrinho cria sua própria rede Wi-Fi (Access Point). Ao conectar o celular ou computador a essa rede, o usuário acessa uma interface web interativa por botões direcionais (Frente, Ré, Esquerda, Direita e Parar) que controlam os motores em tempo real via requisições HTTP assíncronas (`fetch`).

---

## 🛠️ Componentes Utilizados
* **Microcontrolador:** ESP32 DevKit
* **Driver de Motor:** Ponte H (ex: L298N ou similar)
* **Motores DC:** Motores de tração padrão para robótica
* **Alimentação:** Bateria dedicada para os motores e regulação de tensão

---

## 🔌 Pinagem e Conexões (Mapeamento de Hardware)

| Função no ESP32 | Pino GPIO | Descrição |
| :--- | :--- | :--- |
| **Motor Esquerdo (IN1)** | `GPIO 26` | Controle de direção/velocidade esq. |
| **Motor Esquerdo (IN2)** | `GPIO 25` | Controle de direção/velocidade esq. |
| **Motor Direito (IN3)** | `GPIO 33` | Controle de direção/velocidade dir. |
| **Motor Direito (IN4)** | `GPIO 32` | Controle de direção/velocidade dir. |

---

## 🚀 Como Executar o Projeto

### Pré-requisitos
1. Ter a **Arduino IDE** instalada com o suporte à placa **ESP32** configurado, ou utilizar o **PlatformIO** no VS Code.
2. Certificar-se de que a biblioteca padrão `WiFi.h` e `WebServer.h` estão disponíveis para o ESP32.

### Passos para Upload
1. Clone este repositório ou baixe o código-fonte.
2. Abra o arquivo principal de firmware na sua IDE.
3. Conecte o ESP32 ao computador via cabo USB.
4. Selecione a placa correta (`ESP32 Dev Module`) e a porta COM correspondente.
5. Faça o upload (*Compile and Upload*) do código para a placa.

---

## 📱 Como Controlar o Carrinho
1. Abra as configurações de Wi-Fi do seu celular ou notebook.
2. Conecte-se à rede gerada pelo carrinho:
   * **SSDI:** `Carrinho_ESP32`
   * **Senha:** `12345678`
3. Abra o navegador de internet e digite o IP padrão: `http://192.168.4.1`
4. Utilize os botões na tela para pilotar! O carrinho possui um sistema de segurança: ao soltar o botão ou se a aba perder o foco, o carrinho para automaticamente.
