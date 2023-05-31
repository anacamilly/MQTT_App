# MQTT App
Utilizamos o MQTT para o desenvolvimento de uma aplicação mobile que recebe dados de um dispositivo de IoT (esp32) sobre os sensores de luz e temperatura. O dispositivo iot deve possuir um led que poderá ser ligado através de um comando na aplicação mobile.

#### 📱 Aplicação Mobile: 

- Apresenta dados do sensor de luz (assinante do tópico /luz)
- Apresenta dados do sensor de temperatura (assinante do tópico /temperatura)
- Apresenta botão que alterna o estado do led (ligado/desligado) (publicador do tópico /botão)
###### [Ver código](https://github.com/anacamilly/MQTT_App/tree/master/app/src/main).

#### 🕹 Aplicação microcontrolador: 

- coleta dados do sensor de luz (publicador do tópico /luz)
- coleta dados do sensor de temperatura (publicador do tópico /temperatura)
- Apresenta botão que alterna o estado do led (ligado/desligado) (assinante do tópico /botão)</br>
###### [Ver código](https://github.com/anacamilly/MQTT_App/blob/master/mqtt.ino).

## ✔ Tecnologias Utilizadas
- ``Android``
- ``Java``
- ``MQTT``
- ``ESP32``

## 👩‍💻 Desenvolvedores
| [<img src="https://avatars.githubusercontent.com/u/90116902?v=4" width=115><br><sub>Ana Camilly</sub>](https://github.com/anacamilly) |  [<img src="https://avatars.githubusercontent.com/u/93331493?v=4" width=115><br><sub>Lucas Pelinca</sub>](https://github.com/lucasbpc2002) |  [<img src="https://avatars.githubusercontent.com/u/103040741?v=4" width=115><br><sub>Tiago de Melo</sub>](https://github.com/tiago1811) |
| :---: | :---: | :---: |
