function api_get(url) {
  console.log('API get URL:', url)
  fetch(url, {
      method: 'GET',
  })
  .then(response => {
    if (!response.ok) {
      response.json().then(rsp => {
        window.location.href = "/";
        console.error(rsp["result"]);
      });
      return;
    } else {
      return response.json();
    }
  })
  .then(data => {
      console.log(`[API GET] ${url} response:`, data);
  })
  .catch(error => {
      console.error('API error:', error);
  });
}

function api_post(url, json) {
  console.log('API post URL:', url)
  console.log('API post json:', json)
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
        window.location.href = "/";
        console.error(rsp["result"]);
      });
      return;
    } else {
      return response.json();
    }
  })
  .then(data => {
      console.log(`[API POST] ${url} response:`, data);
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
  // TODO: change to websockets
  api_post("/new-player", json);
}

function new_player2(name) {
  json = JSON.stringify({name});
  // TODO: change to websockets
  api_post("/new-player", json);
}

function new_game(game_id, options) {
  const game = game_id;
  json = JSON.stringify({game, options});
  console.log(`New game: ${game_id} with options=${options}. json=${json}`);
  // TODO: change to websockets
  api_post("/new-game", json);
}

function next_player() {
  // TODO: change to websockets
  api_get("/next-player");
  soundsNextPlayer();
}

function finish_game() {
  api_get("/finish-game");
}

function home() {
  launchModal("home-modal");
}

function homeCb() {
  finish_game();
  window.location.href = "/";
}

// For testing purposes
function newDart(zone) {
  const board_id = 99;
  const num = parseInt(document.getElementById('num').value);
  json = JSON.stringify({board_id, num, zone});
  // TODO: change to websockets
  api_post("/new-dart", json);
}
