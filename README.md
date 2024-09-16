# Dartsout!

A simple darts game.

![](./res/dartsout_shot.png)

## Dartboard Hardware and Firmware

[Dartsout! Hardware](https://github.com/GuilleGonzzalez/hw-dartsout)

[Dartsout! Firmware](https://github.com/GuilleGonzzalez/fw-dartsout)

## Installation

- Clone repository ```git clone https://github.com/GuilleGonzzalez/dartsout```
- Go to directory ```cd dartsout```
- Install dependencies ```mongoose``` and ```cjson```. In arch:
  - ```sudo pikaur -S mongoose```
  - ```sudo pacman -S cjson```
- Compile ```make```
- Execute program ```./dartsout```
- ```localhost:8000``` in explorer

## Games

### Cricket

- Cricket normal
- Cricket no score (TODO)
- Cricket cut throat (TODO)
- Cricket wild (TODO)
- Cricket crazy (TODO)
- Cricket wild and crazy (TODO)

### X01

- 301
- 501
- 701
- 901
- 1001

#### Options

- Double in
- Double out

## TODO list

### Dónde me he quedado

Implementando la funcionalidad de poder tener varias instancias de juegos, de forma que puedan existir varias partidas a la vez.
La idea es que cada partida (cada game), tenga un ID distinto. Un jugador podrá unirse a una partida ya iniciada (ya se ha pulsado en Play Cricket / Play xxxx) introduciendo su ID, que el creador de la partida podrá facilitar por otra vía.
El problema es que el ID del juego se genera en new_game (al pulsar en Play xxxx), pero al añadir jugadores ya es necesario este gameID.
Soluciones:

- El gameId se crea al pulsar sobre Play XXX, y sale otro menú para añadir jugadores
  - PROS: soluciona el problema
  - CONS: nuevo menú, menos inteligente
- El gameId se crea al añadir el primer jugador
  - PROS: soluciona el problema
  - CONS: no me parece que sea una solución limpia
- New player no necesita gameId <--- ESTA es la buena
  - PROS: soluciona el problema, misma interfaz
  - CONS: se puede? ---> SI

Implementación de la solución seleccionada:

- El servidor no es el que lleva la cuenta de los jugadores, es el cliente el que crea una lista que al pulsar 'Play', se la pasará de una vez, quizá en el mensaje de new_game (cambiar a ws)
- new player no genera un evento, solo responde con datos de interés (ahora nada, en el futuro será, username, name, mpr, clasificación, etc).

### Easy and prior fixes

- Cricket game modes implementation
- Bootstrap alerts instead alert()
- New sounds (po po po)
- When new connection, a new canvas is created below the original
- When there is a game running if someone enters in dartsout server, msg to go there o directly redirect
- Add player button
- Si estoy en /game y no hay juego, redirigir a /; si estoy en / y hay juego, redirigir a /game
- Arreglar status / new_player

### Mid-term issues

- fw-dartsout IP and WiFi config
- Back button
- New games
- Multiple dartboards (board_id)

### Future features

- DB integration
- Stats button
- Configurable IP/port
