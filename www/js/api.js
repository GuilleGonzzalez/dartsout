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

async function apiPost(url, json) {
  console.log('API post URL:', url)
  console.log('API post json:', json)
  const response = await fetch(url, {
      method: 'POST',
      headers: {
          'Content-Type': 'application/json',
      },
      body: json,
  });
  if (!response.ok) {
      console.error(rsp["result"]);
  };
  return response.json();
}

async function newGame(game_ref, options, players) {
  const game = game_ref;
  json = JSON.stringify({game, options, players});
  console.log(`New game: ${game_ref} with options=${options} and players=${players}. json=${json}`);
  try {
    const rsp = await apiPost(`/new-game`, json);
    window.location=`game.html?id=${rsp["game_id"]}`;
  } catch (error) {
    console.error("Error in new game");
  }
}

function nextPlayer() {
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
  apiPost(`/new-dart?id=${gameId}`, json);
}
