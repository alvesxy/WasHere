# Tópicos 📜
- Objetivo
- Implementação
- Tratando informações no backend
- Estrutura do Application.json
- Agradecimentos
- Dados de contato

# Objetivo 🎯
Ter um módulo, principalmente, de controle de presença simples e facilmente configurável usando RFID e ESP32. Porém, o módulo pode ser aplicado em vários casos, já que a responsabillidade da lógica está no backend.

Por exemplo, em um restaurante, podemos ter três módulos do projeto:
- Módulo para resgatar um refrigerante
- Módulo para resgatar o almoço
- Módulo para pagar o total

Então, um cliente entra no restaurante e recebe um cartão com chip. Ele passa o cartão pela máquina de refrigerante, após isso passa o mesmo com o atendente para pegar a comida. Logo, se dirige ao caixa e têm o total das compras para ser pago.

Você vê a facilidade que o restaurante e o cliente possuem?

# Implementação 🛠️
## Materiais
- RFID MFRC522
- ESP32
- 10 Cabos Jumper Macho X Macho
- Protoboard
- 3 LED
## Organização do diretório
Como será necessário fazer upload de arquivos na ESP32, recomendo o uso do [platformio](https://platformio.org/) para facilitar o uso. Logo, a organização do diretório seria assim:
```
├── 📂 .pio
│   └── ...
├── 📂 data
│   └── 📄 application.json
├── 📂 src
│   └── 📄 main.cpp
└── 📄 platform.ini
```
O arquivo application.json deve ser contido na pasta **data/**, porém pode estar em qualquer subdiretório dela. ⚠️ Atenção para a configuração do construtor, caso o arquivo esteja em subdiretórios.
## Código com o módulo
### main.cpp
Na versão atual do projeto, o módulo funciona de forma assíncrona, então não é possível pôr código normal após o início dele. Mas, podemos efetivar ele apenas com esse trecho de código:
``` C++
#include <WasHere.h>
#include <SPI.h>

void setup () {

    Serial.begin(115200);
    SPI.begin();

    WasHere wasHere = WasHere();
    wasHere.run();
}

void loop () {}
```
### platform.ini
Após isso, preenchemos o arquivo **platform.ini**, que vai ficar algo como isso:
```
[env:esp32doit-devkit-v1]
platform = espressif32
board = esp32doit-devkit-v1
framework = arduino

monitor_speed = 115200

board_build.filesystem = littlefs

lib_deps =  
    https://github.com/alvesxy/WasHere
    https://github.com/miguelbalboa/rfid.git
    ArduinoJson
    littlefs
    https://github.com/chrismoorhouse/ArduinoJWT
```
### application.json
Mas, para o preenchimento das propriedades, o projeto facilita o uso, já que não é necessário um construtor com 10 campos diferentes, mas um arquivo específico, que é o **application.json**:
```json
{
    "SS": 5,
    "RST": 22,
        
    "URL": "http://10.*.**.***:8080/change",
    "method": "PUT",

    "SSID": "Internet",
    "password": "12345678",

    "states": [
        {
            "status": 200,
            "pin": 15
        },

        {
            "status": 404,
            "pin": 2
        },

        {
            "status": 409,
            "pin": 4
        }, 

        {
            "status": 403,
            "pin": 21
        }
    ],

    "secret": "chave secreta",
    "issuer": "emissor"
}
```
## Conexões de hardware
A biblioteca, usada no projeto para o [RFID](https://github.com/miguelbalboa/rfid), possui pinos estáticos e dinâmicos na conexão. Os pinos que são obrigatórios conectar:
| ESP32   | RFID   | 
|------------|------------|
| 3V3 | 3.3v |
| GND | GND |
| 18 | SCK |
| 23 | MOSI |
| 19 | MISO |

E também temos os pinos que podemos escolher, que são os pinos **SDA** e **RST** do RFID. Para isso, a definição é feita no [application.json](#applicationjson).

# Tratando informações no Backend 🗄️
## Forma do JSON
A estrutura do JSON formada no c++ é assim:
``` json
{
  "token": {
    "identifier": 123456789,
    "iss": "emissor"
  }
}
```
Mas, caso ele realmente fosse assim, a pessoa poderia falsificar a presença. Por exemplo, um aluno poderia enviar descobrir o identificador dele e sempre computar que está presente.
## Criptografia 🔐
Para evitar o problema anterior, os dados são criptografados usando a [ArduinoJWT](https://github.com/chrismoorhouse/ArduinoJWT), que suporta apenas HS256. Com isso, o json teria sua forma real assim:
``` json
{
  "token": "eyJhbGciOiJIUzI1NiJ9.ew0KICAic3ViIjogIjMxOTkyMjI0NyIsDQogICJuYW1lIjogIkFuaXNoIE5hdGgiLA0KICAiaWF0IjogMTUxNjIzOTAyMg0KfQ.LAzWDkWCw6axOIDh_cOBrNwtz-JVa5vCnsP6lAz9tQc"
}
```
## Resgatando valor
Agora que houve a explicação de como é estruturado o json, o backend deve resgatar o token, sobrando apenas a String dele:
```
eyJhbGciOiJIUzI1NiJ9.ew0KICAic3ViIjogIjMxOTkyMjI0NyIsDQogICJuYW1lIjogIkFuaXNoIE5hdGgiLA0KICAiaWF0IjogMTUxNjIzOTAyMg0KfQ.LAzWDkWCw6axOIDh_cOBrNwtz-JVa5vCnsP6lAz9tQc
```
Após isso, o backend descriptografa o token fornecendo o **issuer** e a **chave secreta**, que devem estar em ambos os lados, além de resgatar o identificador. O código, em Java, seria assim:
``` java
final Algorithm algorithm = Algorithm.HMAC256("https://www.linkedin.com/in/c-alves-f?");

DecodedJWT decoded;

try {

  JWTVerifier verifier = JWT.require(algorithm)
  .withIssuer("com.github.alvesxy")
  .build();
            
  decoded = verifier.verify(card.token());
} 
        
catch (JWTVerificationException exception){
  return ResponseEntity.status(403).build();
}

final String identifier = decoded.getClaim("identifier").asLong().toString();
```
Enfim, o backend consulta a lógica, já que possui o identificador em mãos. 

Para consulta, o [backend](https://github.com/alvesxy/WasHere-Test) que complementa esse projeto.

# Estrutura do application.json 📝
Como dito na seção [application.json](#application-json), esse arquivo é de configurações da aplicação, já que construtores com muitos argumentos dificulta todo o processo de instanciação de uma classe. Tabela sobre utilidade de cada campo do json:
| Campo   | Exemplo de valor   | Utilidade |
|------------|------------| ------------|
| SS | 5 | Especificar o pino da ESP32 que se conecta com o SDA do RFID
| RST | 22 | Especificar o pino da ESP32 que se conecta com o RESET do RFID
| URL | http://10.0.10.900:8080/change | Rota completa que receberá as informações para serem tratadas
| method | PUT | Verbo http que indica a operação a ser realizada
| SSID | Bar do João | nome que aparece quando você procura por wifis próximos
| password | apenasclientes | senha que você fornece para se autenticar no wifi
| secret | 12345678 | senha responsável por criptografar o identificador, que é necessário estar no backend e no hardware
| issuer | com.github.alvesxy | emissor da chave, que é necessário estar no backend e no hardware
| states | {"status": 200, "pin": 15} | lista para fazer o OUTPUT de um pino baseado no código http

Em relação ao **states**, a quantidade de valores pode ser mais que uma, pois o **states** é uma lista. Logo, você pode ter algo assim:
- Se o status for igual a 200 🟢, então acende sinal verde
- Se o status for igual a 400 🔴, então liga o alerta sonoro
- Se o status for igual a 404 🟠, então liga o motor

# Agradecimentos
Agradeço ao meu instrutor Vagner Jandré e os alunos da UERJ de Nova Friburgo, que deram ideias excelentes, já, que no começo, eu não fazia ideia do que era uma ESP32 ou um RFID ❤️

# Dados de contato
Linkedin: https://www.linkedin.com/in/c-alves-f/

Email: alvescauaferreira@icloud.com
