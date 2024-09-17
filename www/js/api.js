function apiGet(url) {
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

function apiPost(url, json) {
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

function newGame(game_ref, options, players) {
  const game = game_ref;
  json = JSON.stringify({game, options, players});
  console.log(`New game: ${game_ref} with options=${options} and players=${players}. json=${json}`);
  // TODO: change to websockets
  apiPost(`/new-game`, json);
}

function nextPlayer() {
  // TODO: change to websockets
  apiGet(`/next-player?id=${gameId}`);
  soundsNextPlayer();
}

function finishGame() {
  apiGet(`/finish-game?id=${gameId}`);
}

function home() {
  launchModal("home-modal");
}

function homeCb() {
  finishGame();
  window.location.href = "/";
}

function goHomeCb() {
  window.location.href = "/";
}

// For testing purposes
function newDart(zone) {
  const board_id = 99;
  const num = parseInt(document.getElementById('num').value);
  json = JSON.stringify({board_id, num, zone});
  // TODO: change to websockets
  apiPost(`/new-dart?id=${gameId}`, json);
}
