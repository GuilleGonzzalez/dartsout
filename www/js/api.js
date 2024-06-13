function api_post(url, json) {
  fetch(url, {
      method: 'POST',
      headers: {
          'Content-Type': 'application/json',
      },
      body: json,
  })
  .then(response => {
    if (!response.ok) {
      response.json().then(rsp => {
        alert(rsp["result"])
      });
      return;
    } else {
      return response.json();
    }
  })
  .then(data => {
      console.log('API response:', data);
  })
  .catch(error => {
      console.error('API error:', error);
  });
}

function new_player() {
  let name = prompt("Player name:", "Player 1");
  if (name == null || name == "") {
    alert("Invalid player");
    return;
  }
  json = JSON.stringify({name});
  api_post("/new-player", json);
}

function new_game(game_id, options) {
  const game = game_id;
  json = JSON.stringify({game, options});
  console.log(`New game: ${game_id} with options=${options}. json=${json}`);
  api_post("/new-game", json);
}

function next_player() {
  api_post("/next-player", json);
  soundsNextPlayer();
}

function home() {
  // TODO: modal: are you sure...? and finish game if yes
  
  window.location.href = "/";
}

// For testing purposes
function newDart(zone) {
  const board_id = 99;
  const num = parseInt(document.getElementById('num').value);
  json = JSON.stringify({board_id, num, zone})
  api_post("/new-dart", json);
}
