# Dartsout!

A simple darts game.

![](./res/dartsout_shot.png)

## Dartboard Hardware and Firmware

[Dartsout! Hardware](https://github.com/GuilleGonzzalez/hw-dartsout)

[Dartsout! Firmware](https://github.com/GuilleGonzzalez/fw-dartsout)

## Installation
- Clone repository ```git clone https://github.com/GuilleGonzzalez/dartsout ```
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
- Remove home and next player button in home header
- 301 Outchart
- Bootstrap alerts instead alert()

### Requested features
- MPR, MPD
- New games
- Multiple dartboards (board_id)
- External access (server with nginx)

### Future features
- DB integration
- Stats button
- Configurable IP/port
