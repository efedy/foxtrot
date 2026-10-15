
#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// --- NOME E SENHA DA REDE WI-FI DO CARRINHO ---
const char* ssid = "Carrinho_ESP32";
const char* password = "12345678";

// --- MAPEAMENTO DOS MOTORES ---
const int motorEsqIN1 = 26;
const int motorEsqIN2 = 25;
const int motorDirIN3 = 33;
const int motorDirIN4 = 32;

// --- CONFIGURACAO DOS MOTORES ---
const int VELOCIDADE = 220; // 0 a 255

// Canais PWM do ESP32 (Arduino-ESP32 2.x)
const int canalEsqIN1 = 0;
const int canalEsqIN2 = 1;
const int canalDirIN3 = 2;
const int canalDirIN4 = 3;

const int frequenciaPWM = 1000;
const int resolucaoPWM = 8;

// --- SERVIDOR WEB ---
WebServer server(80);

// --- PAGINA WEB ---
const char HTML_PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
    <meta charset="UTF-8">
    <meta name="viewport"
          content="width=device-width, initial-scale=1.0,
          maximum-scale=1.0, user-scalable=no">
    <title>Controle do Carrinho</title>

    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            background-color: #1e1e24;
            color: white;
            user-select: none;
            -webkit-user-select: none;
            margin: 0;
            padding: 20px;
        }

        h2 {
            margin-bottom: 30px;
        }

        .grid {
            display: grid;
            grid-template-columns: repeat(3, 90px);
            grid-template-rows: repeat(3, 90px);
            gap: 15px;
            justify-content: center;
            align-items: center;
            margin: 0 auto;
        }

        .btn {
            background-color: #007bff;
            color: white;
            font-size: 24px;
            font-weight: bold;
            border: none;
            border-radius: 15px;
            box-shadow: 0 5px #0056b3;
            cursor: pointer;
            touch-action: none;
            width: 100%;
            height: 100%;
            display: flex;
            align-items: center;
            justify-content: center;
        }

        .btn:active {
            background-color: #0056b3;
            box-shadow: 0 2px #003580;
            transform: translateY(3px);
        }

        .btn-stop {
            background-color: #dc3545;
            box-shadow: 0 5px #a71d2a;
        }

        .btn-stop:active {
            background-color: #a71d2a;
        }
    </style>
</head>

<body>
    <h2>🏎️ Controle ESP32</h2>

    <div class="grid">
        <div></div>
        <button class="btn" data-dir="frente">▲</button>
        <div></div>

        <button class="btn" data-dir="esquerda">◄</button>
        <button class="btn btn-stop" id="stop">■</button>
        <button class="btn" data-dir="direita">►</button>

        <div></div>
        <button class="btn" data-dir="tras">▼</button>
        <div></div>
    </div>

    <script>
        let ultimoComando = 0;

        async function enviar(comando) {
            try {
                await fetch('/' + comando, {
                    cache: 'no-store'
                });
            } catch (e) {
                console.log('Falha na comunicacao com o carrinho');
            }
        }

        document.querySelectorAll('[data-dir]').forEach(btn => {
            const direcao = btn.dataset.dir;

            btn.addEventListener('pointerdown', e => {
                e.preventDefault();
                ultimoComando = Date.now();
                enviar(direcao);
            });

            btn.addEventListener('pointerup', e => {
                e.preventDefault();
                enviar('parar');
            });

            btn.addEventListener('pointercancel', () => {
                enviar('parar');
            });

            btn.addEventListener('contextmenu', e => {
                e.preventDefault();
            });
        });

        document.getElementById('stop').addEventListener(
            'click', () => enviar('parar')
        );

        // Para o carrinho se a pagina perder o foco.
        window.addEventListener('blur', () => enviar('parar'));
        document.addEventListener('visibilitychange', () => {
            if (document.hidden) enviar('parar');
        });
    </script>
</body>
</html>
)rawliteral";

// --- CONTROLE PWM DOS MOTORES ---
void acionarMotor(int canal1, int canal2, int velocidade1, int velocidade2) {
    ledcWrite(canal1, velocidade1);
    ledcWrite(canal2, velocidade2);
}

// --- MOVIMENTOS ---
void parar() {
    acionarMotor(canalEsqIN1, canalEsqIN2, 0, 0);
    acionarMotor(canalDirIN3, canalDirIN4, 0, 0);
}

void frente() {
    acionarMotor(canalEsqIN1, canalEsqIN2, VELOCIDADE, 0);
    acionarMotor(canalDirIN3, canalDirIN4, VELOCIDADE, 0);
}

void tras() {
    acionarMotor(canalEsqIN1, canalEsqIN2, 0, VELOCIDADE);
    acionarMotor(canalDirIN3, canalDirIN4, 0, VELOCIDADE);
}

void esquerda() {
    acionarMotor(canalEsqIN1, canalEsqIN2, 0, VELOCIDADE);
    acionarMotor(canalDirIN3, canalDirIN4, VELOCIDADE, 0);
}

void direita() {
    acionarMotor(canalEsqIN1, canalEsqIN2, VELOCIDADE, 0);
    acionarMotor(canalDirIN3, canalDirIN4, 0, VELOCIDADE);
}

// --- CONFIGURACAO ---
void setup() {
    Serial.begin(115200);

    // Configura os canais PWM.
    ledcSetup(canalEsqIN1, frequenciaPWM, resolucaoPWM);
    ledcSetup(canalEsqIN2, frequenciaPWM, resolucaoPWM);
    ledcSetup(canalDirIN3, frequenciaPWM, resolucaoPWM);
    ledcSetup(canalDirIN4, frequenciaPWM, resolucaoPWM);

    // Associa os canais aos pinos GPIO.
    ledcAttachPin(motorEsqIN1, canalEsqIN1);
    ledcAttachPin(motorEsqIN2, canalEsqIN2);
    ledcAttachPin(motorDirIN3, canalDirIN3);
    ledcAttachPin(motorDirIN4, canalDirIN4);

    parar();

    // Cria a rede Wi-Fi do carrinho.
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, password);

    Serial.println();
    Serial.println("=== CARRINHO ESP32 ===");
    Serial.print("Rede Wi-Fi: ");
    Serial.println(ssid);
    Serial.print("IP do carrinho: ");
    Serial.println(WiFi.softAPIP());

    // Pagina principal.
    server.on("/", HTTP_GET, []() {
        server.send(200, "text/html; charset=UTF-8", HTML_PAGE);
    });

    // Comandos de movimento.
    server.on("/frente", HTTP_GET, []() {
        frente();
        server.send(200, "text/plain", "OK");
    });

    server.on("/tras", HTTP_GET, []() {
        tras();
        server.send(200, "text/plain", "OK");
    });

    server.on("/esquerda", HTTP_GET, []() {
        esquerda();
        server.send(200, "text/plain", "OK");
    });

    server.on("/direita", HTTP_GET, []() {
        direita();
        server.send(200, "text/plain", "OK");
    });

    server.on("/parar", HTTP_GET, []() {
        parar();
        server.send(200, "text/plain", "OK");
    });

    // Enderecos desconhecidos.
    server.onNotFound([]() {
        server.send(404, "text/plain", "Comando nao encontrado");
    });

    server.begin();
    Serial.println("Servidor HTTP iniciado!");
}

void loop() {
    server.handleClient();
}
