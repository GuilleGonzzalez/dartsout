// let ws_url = "ws://localhost:8000/websocket"
let ws_url = "ws://0.0.0.0:8000/websocket"
// let ws_url = "ws://192.168.0.39:8000/websocket"
let socket = new WebSocket(ws_url);

const MsgId = {
	GameStatus: 0,
	Cricket:    1,
	X01:        2,
  // Player:     3,
}

function sendWsMsg() {
  let msg = document.getElementById("wsMsg").value;
  socket.send(msg);
};

window.onload = getStatus();

function getStatus() {
  console.log(window.location.href);
  if (window.location.href.endsWith("game.html") || window.location.href.endsWith("x01.html")) {
    const game = 0;
    json = JSON.stringify({game});
    api_post("/status", json);
  }
}

socket.onmessage = function(event) {
  let incomingMessage = event.data;
  showMessage(incomingMessage);
};

socket.onclose = function(event) {
  console.log(`Closed ${event.code}`);
}

function showMessage(message) {
  let messageElem = document.createElement('div');
  messageElem.textContent = "MSG rvc: " + message;
  document.getElementById('messages').prepend(messageElem);
  let json;
  try {
    json = JSON.parse(message);
  } catch {
    console.error("Invalid JSON");
    return;
  }
  let msgId = json["msg_id"];
  switch (msgId) {
    case MsgId.GameStatus:
      let running = json["running"];
      let game_id = json["game_id"];
      if (running) {
        if (game_id == 0) {
          window.location="game.html";
        } else if (game_id == 1) {
          window.location="x01.html";
        } else {
          console.error("Game not implemented!");
        }
      }
      let players = json["players"];
      let game_n_players = json["n_players"];
      for (let i = 0; i < game_n_players; i++) {
        document.getElementById("player"+i).innerHTML = players[i]["name"];
      }
      break;
    case MsgId.Cricket:
      let n_players = json["n_players"];
      let darts = json["darts"];
      let dart_scores = json["dart_scores"];
      let curr_player_idx = json["current_player"];
      let curr_player = json["players"][curr_player_idx];
      document.getElementById("player").innerHTML = curr_player["name"];
      document.getElementById("game_score").innerHTML = curr_player["game_score"];
      document.getElementById("round_score").innerHTML = curr_player["round_score"];
      document.getElementById("round").innerHTML = json["round"];
      document.getElementById("max_rounds").innerHTML = json["max_rounds"];
      
      let shots = [];
      const scoresRow = document.getElementById("scores");
      const cardHeaders = scoresRow.getElementsByClassName("card-header");
      const cardTexts = scoresRow.getElementsByClassName("card-text");
      for (let i = 0; i < n_players; i++) {
        cardHeaders[i].textContent = json["players"][i]["name"];
        cardTexts[i].textContent = json["players"][i]["game_score"];
        shots[i] = json["players"][i]["shots"];
      }
      drawTable(json["players"]);
      drawDarts(darts, dart_scores);
      drawShots(shots);
      highlightRow(curr_player_idx + 1);
      break;
    case MsgId.X01:
      // TODO: fix this
      let x01_n_players = json["n_players"];
      let x01_darts = json["darts"];
      let x01_dart_scores = json["dart_scores"];
      let x01_curr_player_idx = json["current_player"];
      let x01_curr_player = json["players"][x01_curr_player_idx];
      document.getElementById("player").innerHTML = x01_curr_player["name"];
      document.getElementById("game_score").innerHTML = x01_curr_player["game_score"];
      document.getElementById("round_score").innerHTML = x01_curr_player["round_score"];
      document.getElementById("round").innerHTML = json["round"];
      document.getElementById("max_rounds").innerHTML = json["max_rounds"];
      document.getElementById("title").textContent = json["score"];

      const x01_scoresRow = document.getElementById("scores");
      const x01_cardHeaders = x01_scoresRow.getElementsByClassName("card-header");
      const x01_cardTexts = x01_scoresRow.getElementsByClassName("card-text");
      for (let i = 0; i < x01_n_players; i++) {
        x01_cardHeaders[i].textContent = json["players"][i]["name"];
        x01_cardTexts[i].textContent = json["players"][i]["game_score"];
      }
      drawDarts(x01_darts, x01_dart_scores);
      break;
    case MsgId.Player:
      addPlayerToTable(json, 1);
      break;
    default:
      alert("Error");
  }
}

function drawDarts(n_darts, dart_scores) {
  let dart_img_path = "/res/dart.svg";
  let dart_closed_img_path = "/res/dart_dark.svg";
  if (n_darts == 0) {
    document.getElementById("dart1_img").src = dart_img_path;
    document.getElementById("dart2_img").src = dart_img_path;
    document.getElementById("dart3_img").src = dart_img_path;
    document.getElementById("dart1_num").innerHTML = "";
    document.getElementById("dart1_zone").innerHTML = "";
    document.getElementById("dart2_num").innerHTML = "";
    document.getElementById("dart2_zone").innerHTML = "";
    document.getElementById("dart3_num").innerHTML = "";
    document.getElementById("dart3_zone").innerHTML = "";
  }
  if (n_darts >= 1) {
    document.getElementById("dart1_img").src = dart_closed_img_path;
    document.getElementById("dart1_num").innerHTML = getNumStr(dart_scores[0]["num"]);
    document.getElementById("dart1_zone").innerHTML = getZoneStr(dart_scores[0]["zone"]);
    // document.getElementById("dart1_num").classList.add(<<texto_en_plateado_dorado>>);
  }
  if (n_darts >= 2) {
    document.getElementById("dart2_img").src = dart_closed_img_path;
    document.getElementById("dart2_num").innerHTML = getNumStr(dart_scores[1]["num"]);
    document.getElementById("dart2_zone").innerHTML = getZoneStr(dart_scores[1]["zone"]);
  }
  if (n_darts >= 3) {
    document.getElementById("dart3_img").src = dart_closed_img_path;
    document.getElementById("dart3_num").innerHTML = getNumStr(dart_scores[2]["num"]);
    document.getElementById("dart3_zone").innerHTML = getZoneStr(dart_scores[2]["zone"]);
  }
}

function drawTable(players) {
  const table = document.getElementById("cricketTable");
  const tableBody = table.querySelector("tbody");
  tableBody.innerHTML = "";
  for (let i = 0; i < players.length; i++) {
    var row = document.createElement("tr");
    var colHeader = document.createElement("th");
    colHeader.setAttribute('scope', 'row');
    colHeader.innerHTML = players[i]["name"];
    row.append(colHeader);
    for (let i = 0; i < 7; i++) {
      var col = document.createElement("td");
      row.append(col);
    }
    tableBody.append(row);
  }
}

function addPlayerToTable(player, idx) {
  const table = document.getElementById("playersTable");
  const tableBody = table.querySelector("tbody");
  var row = document.createElement("tr");
  var colHeader = document.createElement("th");
  colHeader.setAttribute('scope', 'row');
  colHeader.innerHTML = idx;
  row.append(colHeader);
  var userid_cell = document.createElement("td");
  userid_cell.innerHTML = player["userid"];
  row.append(userid_cell);
  var name_cell = document.createElement("td");
  name_cell.innerHTML = player["name"];
  row.append(name_cell);
  tableBody.append(row);
}

function highlightRow(row) {
  const table = document.getElementById("cricketTable");
  const rows = table.querySelectorAll("tbody tr");
  const sel_row = table.querySelector(`tbody tr:nth-child(${row})`);
  rows.forEach(function(r) {
    r.classList.remove("table-active");
  });
  sel_row.classList.add("table-active");
  
}

function getSymbol(num) {
  switch(num) {
  case 1:
    return "𝈺";
  case 2:
    return "⨉";
  case 3:
    return "⦻";
  default:
    return "";
  }
}

function getNumStr(num) {
  if (num == 0) {
    return "Bull";
  } else {
    return num;
  }
}

function getZoneStr(zone_id) {
  if (zone_id == 0) {
    return "Triple";
  } else if (zone_id == 1) {
    return "Double";
  } else {
    return "Single";
  }
}

// function drawNumbers(shots) {
//   const table = document.getElementById("cricketTable");
//   const rows = table.getElementsByTagName("tr");
//   const cells = rows[0].getElementsByTagName("td");
//     for (let i = 0; i < cells.length; i++) {
//       cells[j].textContent = getSymbol(numbers[i]);
//     }
// }

function checkClosed(shots) {
  let isOpen = Array(21).fill(0);
  for (let i = 0; i < shots.length; i++) {
    console.log(`Shots[${i}]: ${shots[i]}`);
    for (let j = 0; j < 22; j++) {
      if (shots[i][j] < 3) {
        console.log(shots[i][j]);
        isOpen[j] = 1;
      } else {
      }
    }
  }
  console.log(`Numbers open: ${isOpen}`);
  return isOpen;
}

function drawShots(shots) {
  const table = document.getElementById("cricketTable");
  const rows = table.getElementsByTagName("tr");
  // Closed numbers
  let numbersOpen = checkClosed(shots);
  const cells = rows[0].getElementsByTagName("th");
  for (let i = 0; i < cells.length-1; i++) {
    if (numbersOpen[21-i] == 0) {
      cells[i].textContent = "";
    }
  }
  // Bull
  if (numbersOpen[0] == 0) {
    cells[cells.length-1].textContent = "";
  }
  // Draw shots
  for (let i = 0; i < shots.length; i++) {
    const cells = rows[i+1].getElementsByTagName("td");
      for (let j = 0; j < cells.length; j++) {
        // Shots is an array with shots in numbers: bull, 1, 2... 20
        // Cells is 20, 19, 18, 17, 16, 15, bull
        cells[j].textContent = getSymbol(shots[i][20-j]);
      }
      // Bull
      cells[cells.length-1].textContent = getSymbol(shots[i][0]);
  }  
}