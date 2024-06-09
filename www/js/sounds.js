
function soundsNewDart(last_dart_valid, last_dart_zone) {
  if (last_dart_valid) {
    switch(last_dart_zone) {
    case(0):
      document.getElementById("triple_audio").play();
    case(1):
      document.getElementById("double_audio").play();
    default:
      document.getElementById("simple_audio").play();
    }
  } else {
    document.getElementById("no_audio").play();
  }
}

function soundsWinnwer()) {
  document.getElementById("winner_audio").play();
}