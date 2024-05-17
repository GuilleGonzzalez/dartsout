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

function new_dart(zone) {
  const board_id = 1;
  const num = parseInt(document.getElementById('num').value);
  json = JSON.stringify({board_id, num, zone})
  api_post("/new-dart", json) 
}

function new_game() {
  const game = 0;
  json = JSON.stringify({game});
  api_post("/new-game", json);
}

function register_player() {
  let userid = prompt("New player userid:", "");
  let name = prompt("New player name:", "");
  json = JSON.stringify({userid, name});
  api_post("/register-player", json);
}

function new_reg_player() {
  // let userid = prompt("Player userid:", "");
  let userid = document.getElementById('modalUserId').value;

  json = JSON.stringify({userid});
  api_post("/get-player", json);
}

function validate_player() {
  // If user_id and btn "add player" pressed
  // "/get-player". If error, show a modal, otherwise, show name and add player
}

function new_player() {
  // New modal
  // if userid, "/get-player" and post userid and name
  // if name, post "", name
  let userid = "";
  let name = prompt("Player name:", "Player 1");
  if (name == null || name == "") {
    alert("Invalid player");
    return;
  }
  json = JSON.stringify({userid, name});
  api_post("/new-player", json);
}

function next_player() {
  api_post("/next-player", json);
}