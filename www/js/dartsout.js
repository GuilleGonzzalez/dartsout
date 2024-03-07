let ws_url = "ws://localhost:8000/websocket"
let socket = new WebSocket(ws_url);

const MsgId = {
	NewGame: 0,
	Cricket: 1,
}

function sendWsMsg() {
  let msg = document.getElementById("wsMsg").value;
  socket.send(msg);
};

socket.onmessage = function(event) {
  let incomingMessage = event.data;
  showMessage(incomingMessage);
};

socket.onclose = event => console.log(`Closed ${event.code}`);

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
    case MsgId.NewGame:
      let running = json["running"];
      let game_id = json["game_id"];
      let players = json["players"];
      let game_n_players = json["n_players"];
      for (let i = 0; i < game_n_players; i++) {
        document.getElementById("player"+i).innerHTML = players[i]["name"];
      }
      break;
    case MsgId.Cricket:
      let n_players = json["n_players"];
      let curr_player_idx = json["current_player"];
      let curr_player = json["players"][curr_player_idx]
      document.getElementById("player").innerHTML = curr_player["name"];
      document.getElementById("game_score").innerHTML = curr_player["game_score"];
      document.getElementById("darts").innerHTML = json["darts"];
      document.getElementById("round").innerHTML = json["round"];
      document.getElementById("max_rounds").innerHTML = json["max_rounds"];
      break;
    default:
      alert("Error");
  }
}

function api_post(url, json) {
  fetch(url, {
      method: 'POST',
      headers: {
          'Content-Type': 'application/json',
      },
      body: json,
  })
  .then(response => response.json())
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
  json = JSON.stringify({game})
  api_post("/new-game", json)
}

function new_player() {
  let player = prompt("Player name:", "Player 1");
  if (player == null || player == "") {
    alert("Invalid player");
    return;
  }
  json = JSON.stringify({player})
  api_post("/new-player", json) 
}

function next_player() {
  api_post("/next-player", json) 
}

var tableData = [];

function createTable() {
  var table = document.getElementById("myTable");
  var columns = 8;
  var rows = 5;

  var tableHeader = document.createElement("thead");
  var headerRow = document.createElement("tr");
  tableHeader.appendChild(headerRow);
  for (var i = 0; i < columns; i++) {
    var th = document.createElement("th");
    th.setAttribute("scope", "col");
    headerRow.appendChild(th);
  }
  table.appendChild(tableHeader);

  for (var i = 0; i < rows; i++) {
    var rowData = [];
    var tableBody = document.createElement("tbody");
    var row = document.createElement("tr");
    tableBody.appendChild(row);
    var th = document.createElement("th");
    th.setAttribute("scope", "row");
    row.appendChild(th);
    for (var j = 1; j < columns; j++) {
      var cell = document.createElement("td");
      rowData.push("");
      row.appendChild(cell);
    }
    table.appendChild(tableBody);
    tableData.push(rowData);
  }
}

function updateData(row, col, value) {
  var table = document.getElementById("myTable");
  var cells = table.rows[row + 1].cells;
  cells[row][col] = value;
  tableData = value;
}

function deleteRow(row) {
  var table = document.getElementById("myTable");
  table.deleteRow(row);
  tableData.splice(row, 1);
}

function updateRow(row) {
  var table = document.getElementById("myTable");
  var rowData = tableData[row];
  var cells = table.rows[row + 1].cells;
  for (var i = 0; i < rowData.length; i++) {
    var value = cells[i].textContent.trim();
    rowData[i] = value;
  }
}

function displayData() {
  console.log(tableData);
}