let ws_url = "ws://localhost:8000/websocket"
// let ws_url = "ws://0.0.0.0:8000/websocket"
let socket = new WebSocket(ws_url);

const MsgId = {
	GameStatus: 0,
	Cricket:    1,
  Player:     2,
}

function sendWsMsg() {
  let msg = document.getElementById("wsMsg").value;
  socket.send(msg);
};

window.onload = getStatus();

function getStatus() {
  console.log(window.location.href);
  if (window.location.href.endsWith("game.html")) {
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
        window.location="game.html";
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
    case MsgId.Player:
      addPlayerToTable(json, 1);
      break;
    default:
      alert("Error");
  }
}

function drawDarts(n_darts, dart_scores) {
  if (n_darts == 0) {
    document.getElementById("dart1_img").src = "/res/dart.svg";
    document.getElementById("dart2_img").src = "/res/dart.svg";
    document.getElementById("dart3_img").src = "/res/dart.svg";
    document.getElementById("dart1_num").innerHTML = "";
    document.getElementById("dart1_zone").innerHTML = "";
    document.getElementById("dart2_num").innerHTML = "";
    document.getElementById("dart2_zone").innerHTML = "";
    document.getElementById("dart3_num").innerHTML = "";
    document.getElementById("dart3_zone").innerHTML = "";
  }
  if (n_darts >= 1) {
    document.getElementById("dart1_img").src = "/res/dart_dark.svg";
    document.getElementById("dart1_num").innerHTML = getNumStr(dart_scores[0]["num"]);
    document.getElementById("dart1_zone").innerHTML = getZoneStr(dart_scores[0]["zone"]);
    dart_scores[0]["zone"];
  }
  if (n_darts >= 2) {
    document.getElementById("dart2_img").src = "/res/dart_dark.svg";
    document.getElementById("dart2_num").innerHTML = getNumStr(dart_scores[1]["num"]);
    document.getElementById("dart2_zone").innerHTML = getZoneStr(dart_scores[1]["zone"]);
  }
  if (n_darts >= 3) {
    document.getElementById("dart3_img").src = "/res/dart_dark.svg";
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

function drawShots(shots) {
  const table = document.getElementById("cricketTable");
  const rows = table.getElementsByTagName("tr");
  for (let i = 0; i < shots.length; i++) {
    const cells = rows[i+1].getElementsByTagName("td");
      for (let j = 0; j < cells.length; j++) {
        // Shots is an array with shots in numbers: bull, 1, 2... 20
        // Cells is 20, 19, 18, 17, 16, 15, bull
        cells[j].textContent = getSymbol(shots[i][20-j]);
      }
      // Bull
      console.log(shots[i][0]);
      cells[cells.length-1].textContent = getSymbol(shots[i][0]);
  }  
}