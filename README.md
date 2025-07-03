# The Dartsout! game

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

## Messages

### 1. Status (msg_id=0)

```json
{
  "msg_id": 0,
  "running": false,
  "game_id": 0,
  "options": 0,
  "n_players": 1,
  "players": [
    {
      "user_id": "user1",
      "name": "name1",
    },
    {
      "user_id": "user2",
      "name": "name2",
    }
  ]
}
```

### 2. Cricket (msg_id=1)

```json
{
  "msg_id": 1,
  "n_players": 1,
  "round": 0,
  "max_rounds": 0,
  "max_score": 0,
  "current_player": 0,
  "darts": 0,
  "dart_scores": [
    {
      "num": 0,
      "zone": 0,
    },
    {
      "num": 0,
      "zone": 0,
    },
    {
      "num": 0,
      "zone": 0,
    },
  ],
  "enabled": [16, 17, 18],
  "players": [
    {
      "name": "name1",
      "game_score": 0,
      "round_score": 0,
      "mpr": 1,
      "img_path": "res/user.svg",
      "shots": [0, 0, 0, 0, 0, 0, 0]
    }
  ]
}
```

### 3. X01 (msg_id=2)

```json
{
  "msg_id": 2,
  "score": 301,
  "n_players": 1,
  "round": 0,
  "max_rounds": 0,
  "current_player": 0,
  "darts": 0,
  "dart_scores": [
    {
      "num": 0,
      "zone": 0,
    },
    {
      "num": 0,
      "zone": 0,
    },
    {
      "num": 0,
      "zone": 0,
    },
  ],
  "players": [
    {
      "name": "name1",
      "game_score": 0,
      "round_score": 0,
    }
  ],
}
```

### 4. Last dart (msg_id=3)

```json
{
  "msg_id": 3,
  "valid": true,
  "num": 0,
  "zone": 0,
}
```

### 5a. Winner (msg_id=4)

```json
{
  "msg_id": 4,
  "name": "name1",
}
```

### 5b. Cricket winner (msg_id=4)

```json
{
  "msg_id": 4,
  "game_id": 0,
  "name": "name1",
  "players": [
    {
      "name": "name1",
      "game_score": 0,
      "mpr": 1,
    }
    {
      "name": "name2",
      "game_score": 0,
      "mpr": 1,
    }
  ],
}
```

### 6. Game ID (msg_id=5)

```json
{
  "msg_id": 5,
  "game_id": 0,
}
```

## TODOs

- si no se juega durante un tiempo, llamar a finish-game
- tiempo de partida
- llamar a game_manager_finish
- añadir game_id en el log
- si no players y new_game, se crea. Debería devolver error y manejarse
- Cricket game modes implementation
- Bootstrap alerts instead alert()
- New sounds (po po po)
- When new connection, a new canvas is created below the original
- When there is a game running if someone enters in dartsout server, msg to go there o directly redirect
- Add player button
- Si estoy en /game y no hay juego, redirigir a /; si estoy en / y hay juego, redirigir a /game
- Ahora se filtra mirando si la diana forma parte del juego. Habría que filtrar eso y si la diana es del jugador actual, si no, cualquier diana del juego vale para cualquier jugador

### Mid-term issues

- New games

### Future features

- DB integration
- Stats button
