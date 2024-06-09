function createImgButton(onClick, src) {
  let button = document.createElement("button");
  button.type = "button";
  button.className = "btn";
  button.setAttribute("onclick", onClick);
  let img = document.createElement("img");
  img.src = src;
  img.height = 40;
  button.appendChild(img);

  return button;
}

function createHeader(name) {
  let header = document.createElement("div");
  header.className = "container-fluid";
  let row = document.createElement("div");
  row.className = "row";
  let home = document.createElement("div");
  home.className = "col-1";
  let homeBtn = createImgButton("home()", "res/home.svg");
  home.appendChild(homeBtn);
  let title = document.createElement("col");
  title.className = "col";
  let h1 = document.createElement("h1");
  h1.className = "display-3 fw-bolder text-center";
  h1.id = "title";
  h1.innerHTML = name;
  title.appendChild(h1);
  let nextPlayer = document.createElement("div");
  nextPlayer.className = "col-1";
  let nextPlayerBtn = createImgButton("next_player()", "res/next_player.svg");
  nextPlayer.appendChild(nextPlayerBtn);

  row.appendChild(home);
  row.appendChild(title);
  row.appendChild(nextPlayer);
  header.appendChild(row);

  return header;
}

function createSpacer(hSpace) {
  let spacer = document.createElement("div");
  spacer.className = "spacer";
  spacer.style = `height: ${hSpace}px`;

  return spacer;
}

function createGrid(nPlayers) { // Example
  let grid = document.createElement("div");
  grid.className = "container-fluid";
  let row = document.createElement("div");
  row.className = "row";
  let colLeft = document.createElement("div");
  colLeft.className = "col col-lg-3";
  let colRight = document.createElement("div");
  colRight.className = "col";
  
  let info = createCardInfo();
  
  let cards = createScoreCards(nPlayers);
  let table = createCricketTable(nPlayers);

  colLeft.appendChild(info);
  colRight.appendChild(cards);
  colRight.appendChild(table);

  row.appendChild(colLeft);
  row.appendChild(colRight);
  grid.appendChild(row);

  return grid;
}

function createCricketTable(rows) {
  let table = document.createElement("table");
  table.id = "cricket-table";
  table.className = "table text-center";
  let thead = document.createElement("thead");
  thead.className = "table-light fw-bolder fs-2";
  let h_tr = document.createElement("tr");
  for (let i = 0; i < 8; i++) {
    let th = document.createElement("th");
    th.scope = "col";
    h_tr.appendChild(th);
  }
  thead.appendChild(h_tr);
  let tbody = document.createElement("tbody");
  tbody.className = "fs-2";
  for (let i = 0; i < rows; i++) {
    let b_tr = document.createElement("tr");
    for (let j = 0; j < 8; j++) {
      let td = document.createElement("td");
      td.scope = "row";
      b_tr.appendChild(td);
    }
    tbody.appendChild(b_tr);
  }

  table.appendChild(thead);
  table.appendChild(tbody);

  return table;
}

function createSimpleCard(headerText, bodyText) {
  let card = document.createElement("div");
  card.className = "card text-center";
  card.style = "width: 10rem;";
  let cardHeader = document.createElement("div");
  cardHeader.className = "card-header fw-bolder";
  cardHeader.innerHTML = headerText;
  let cardBody = document.createElement("div");
  cardBody.className = "card-body";
  let cardText = document.createElement("h1");
  cardText.className = "card-text fw-bolder fs-1";
  cardText.innerHTML = bodyText;

  cardBody.appendChild(cardText);
  card.appendChild(cardHeader);
  card.appendChild(cardBody);

  return card;
}

function createScoreCards(num) {
  let scores = document.createElement("div");
  scores.className = "row";
  scores.id = "scores";
  for (let i = 0; i < num; i++) {
    let col = document.createElement("div");
    col.className = "col mb-4";
    let card = createSimpleCard(`Card ${i}`, `Text ${i}`);
    col.appendChild(card);
    scores.appendChild(col);
  }

  return scores;
}

function createCardInfo() {
  let card = document.createElement("div");
  card.id = "card-info";
  card.className = "card mb-4";
  card.style = "width: 18rem;";
  let img = document.createElement("img");
  img.id = "user_img";
  img.src ="res/user.svg"
  img.className = "card-img-top";
  img.height = 200;
  card.appendChild(img);
  
  let infoTitle = createCardInfoTitle();
  let listItems = ["Round", "Game score", "Round score"];
  let infoBody = createCardInfoBody(listItems);
  card.appendChild(infoTitle);
  card.appendChild(infoBody);

  return card;
}

function createCardInfoTitle() {
  let cardTitle = document.createElement("h2");
  cardTitle.id = "card-info-title"
  cardTitle.className = "card-title text-center fs-2";

  return cardTitle;
}

function createCardInfoBody(listItems) {
  let cardBody = document.createElement("div");
  cardBody.className = "card-body";
  let list = document.createElement("ul");
  list.id = "card-info-list";
  list.className = "list-group list-group-flush fs-5 fw-bolder";
  let row = document.createElement("div");
  row.className = "row";
  row.id = "darts";
  for (let i = 0; i < 3; i++) { // TODO: variable?
    let col = document.createElement("div");
    col.className = "col text-center";
    let img = document.createElement("img");
    img.id = `dart${i}_img`;
    img.src = "res/dart.svg";
    img.className = "card-img-top";
    img.height = 50;
    let br = document.createElement("br");
    let num = document.createElement("span");
    num.id = `dart${i}_num`;
    num.className = "fs-2 fw-bolder";
    num.innerHTML = "7";
    let zone = document.createElement("span");
    zone.innerHTML = "Triple";
    zone.id = `dart${i}_zone`;
    col.appendChild(img);
    col.appendChild(br);
    col.appendChild(num);
    col.appendChild(br);
    col.appendChild(zone);
    row.appendChild(col);
  }
  let br = document.createElement("br");
  cardBody.appendChild(list);
  cardBody.appendChild(br);
  cardBody.appendChild(row);

  return cardBody;
}

function createAudio(id, src) {
  let audio = document.createElement("audio");
  audio.id = id;
  audio.src = src;

  return audio;
}
// Update functions

function updateTitle(text) {
  let title = document.getElementById("title");
  title.innerHTML = text;
}

function updateCricketTable(header, data, closedNumbers) {
  let table = document.getElementById("cricket-table");
  let htr = table.firstChild.firstChild;
  let i;
  i = 0;
  for (let th = htr.firstChild; th; th = th.nextSibling) {
    th.innerHTML = header[i];
    console.log(header[i], closedNumbers, closedNumbers.includes(header[i]));
    if (closedNumbers.includes(header[i])) {
      th.style.color = "#A0A0A0";
    }
    i++;
  }
  let tBody = table.lastChild; // TODO: query selector
  i = 0;
  for (let btr = tBody.firstChild; btr; btr = btr.nextSibling) {
    j = 0;
    for (let td = btr.firstChild; td; td = td.nextSibling) {
      td.innerHTML = data[i][j];
      j++
    }
    i++;
  }
}

function highlightCricketTableRow(row) {
  let table = document.getElementById("cricket-table");
  let rows = table.querySelectorAll("tbody tr");
  let selRow = table.querySelector(`tbody tr:nth-child(${row+1}`);
  rows.forEach(function(r) {
    r.classList.remove("table-active");
  });
  selRow.classList.add("table-active");
}

function updateCardInfo(name, info_list) {
  let title = document.getElementById("card-info-title");
  title.innerHTML = name;
  let list = document.getElementById("card-info-list");
  while (list.lastElementChild) {
    list.removeChild(list.lastElementChild);
  }
  for (let i = 0; i < info_list.length; i++) {
    let listItem = document.createElement("li");
    listItem.className = "list-group-item";
    listItem.innerHTML = info_list[i];
    list.appendChild(listItem);
  }
}

function updateDarts(nums, zones, nDarts) {
  // TODO: in config file
  let dart_img_path = "/res/dart.svg";
  let dart_closed_img_path = "/res/dart_dark.svg";
  for (let i = 0; i < 3; i++) { // TODO: check
    let num = document.getElementById(`dart${i}_num`);
    let zone = document.getElementById(`dart${i}_zone`);
    let img = document.getElementById(`dart${i}_img`);
    num.innerHTML = nums[i];
    zone.innerHTML = zones[i];
    num.style.textShadow = getZoneShadow(zones[i]);
    zone.style.textShadow = getZoneShadow(zones[i]);
    img.src = dart_img_path;
    if (i < nDarts) {
      img.src = dart_closed_img_path;
    }
  }
}

function updateScoreCards(players, scores) {
  let scoreCards = document.getElementById("scores");
  let i = 0;
  for (let col = scoreCards.firstChild; col; col = col.nextSibling) {
    let card = col.firstChild;
    let header = card.firstChild;
    header.innerHTML = players[i];
    let body = card.lastChild;
    let text = body.firstChild;
    text.innerHTML = scores[i];
    i++;
  }
}
