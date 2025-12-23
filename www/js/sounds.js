function soundsNewGame() {
  document.getElementById("new_game_audio").play();
}

function soundsNewDart(last_dart_valid, last_dart_zone) {
  if (last_dart_valid) {
    switch(last_dart_zone) {
    case(3):
      document.getElementById("triple_audio").play();
    case(2):
      document.getElementById("double_audio").play();
    default:
      document.getElementById("simple_audio").play();
    }
  } else {
    document.getElementById("no_audio").play();
  }
}

function soundsWinner() {
  document.getElementById("winner_audio").play();
}

function soundsNextPlayer() {
  document.getElementById("next_player_audio").play();
}

function soundsBack() {
  document.getElementById("back_audio").play();
}