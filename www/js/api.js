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

// function new_player() {
//   let name = prompt("Player name:", "Player 1");
//   if (name == null || name == "") {
//     alert("Invalid player");
//     return;
//   }
//   json = JSON.stringify({name});
// }

// function new_player2(name) {
//   json = JSON.stringify({name});
//   // TODO: change to websockets
//   api_post(`/new-player?game_id=${gameId}`, json);
// }

function new_game(game_ref, options, players) {
  const game = game_ref;
  json = JSON.stringify({game, options, players});
  console.log(`New game: ${game_ref} with options=${options} and players=${players}. json=${json}`);
  // TODO: change to websockets
  api_post(`/new-game`, json);
}

function next_player() {
  // TODO: change to websockets
  api_get(`/next-player?game_id=${gameId}`);
  soundsNextPlayer();
}

function finish_game() {
  api_get(`/finish-game?game_id=${gameId}`);
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
  api_post("/new-dart?game_id=0", json);
}
