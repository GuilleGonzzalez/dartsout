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

### Easy and prior fixes

- Cricket game modes implementation
- Bootstrap alerts instead alert()
- New sounds (po po po)
- When new connection, a new canvas is created below the original
- When there is a game running if someone enters in dartsout server, msg to go there o directly redirect
- Add player button

### Mid-term issues

- fw-dartsout IP and WiFi config
- Back button
- New games
- Multiple dartboards (board_id)

### Future features

- DB integration
- Stats button
- Configurable IP/port
