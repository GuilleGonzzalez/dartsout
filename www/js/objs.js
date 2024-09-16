function createImgButton(onClick, src, align="left") {
  let button = document.createElement("button");
  button.type = "button";
  button.className = "btn";
  button.style = `margin: auto; margin-${align}: 0; display: block`;
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
  let title = document.createElement("col");
  title.className = "col";
  let h1 = document.createElement("h1");
  h1.className = "display-3 fw-bolder text-center";
  h1.id = "title";
  h1.innerHTML = name;
  title.appendChild(h1);

  row.appendChild(title);
  header.appendChild(row);

  return header;
}

function createGameHeader(name) {
  let header = document.createElement("div");
  header.className = "container-fluid";
  let row = document.createElement("div");
  row.className = "row";
  let home = document.createElement("div");
  home.className = "col-3";
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
  nextPlayer.className = "col-3";
  let nextPlayerBtn = createImgButton("next_player()", "res/next_player.svg",
      align="right");
  nextPlayer.appendChild(nextPlayerBtn);

  row.appendChild(home);
  row.appendChild(title);
  row.appendChild(nextPlayer);
  header.appendChild(row);

  return header;
}

function createButton(text, onClick) {
  let button = document.createElement("button");
  button.className = "btn btn-block btn-primary m-2";
  button.type = "button";
  button.setAttribute("onclick", onClick);
  button.innerHTML = text;

  return button;
}

{/* <div class="input-group mb-3">
  <div class="input-group-prepend">
    <span class="input-group-text" id="basic-addon1">@</span>
  </div>
  <input type="text" class="form-control" placeholder="Username" aria-label="Username" aria-describedby="basic-addon1">
</div> */}

function createInputText(id) {
  let input = document.createElement("input");
  input.type = "text";
  input.id = id;
  input.className = "form-control";
  input.setAttribute("placeholder", "Game ID");

  return input;
}

function createSpacer(hSpace) {
  let spacer = document.createElement("div");
  spacer.className = "spacer";
  spacer.style = `height: ${hSpace}px`;

  return spacer;
}

function createTable(tableId, nRows, nCols) {
  let table = document.createElement("table");
  table.id = tableId;
  table.className = "table text-center";
  let thead = document.createElement("thead");
  thead.className = "table-light fw-bolder fs-2";
  let h_tr = document.createElement("tr");
  for (let i = 0; i < nCols; i++) {
    let th = document.createElement("th");
    th.scope = "col";
    h_tr.appendChild(th);
  }
  thead.appendChild(h_tr);
  let tbody = document.createElement("tbody");
  tbody.className = "fs-2";
  for (let i = 0; i < nRows; i++) {
    let b_tr = document.createElement("tr");
    for (let j = 0; j < nCols; j++) {
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

function createSimpleCard(id) {
  let card = document.createElement("div");
  card.className = "card mb-3";
  card.id = id;
  let cardBody = document.createElement("div");
  cardBody.className = "card-body";
  let cardText = document.createElement("h1");
  cardText.className = "card-text fw-bolder";
  cardBody.appendChild(cardText);
  card.appendChild(cardBody);

  return card;
}

function createCard(id, width="10rem", headerFontASize="20px",
    textFontSize="50px") {
  let card = document.createElement("div");
  card.className = "card text-center";
  card.id = id;
  card.style = `width: ${width};`;
  let cardHeader = document.createElement("div");
  cardHeader.className = "card-header fw-bolder";
  cardHeader.style = `font-size: ${headerFontASize};`;
  let cardBody = document.createElement("div");
  cardBody.className = "card-body";
  let cardText = document.createElement("h1");
  cardText.className = "card-text fw-bolder";
  cardText.style = `font-size: ${textFontSize};`;

  cardBody.appendChild(cardText);
  card.appendChild(cardHeader);
  card.appendChild(cardBody);

  return card;
}

function createScoreCards(num, width, headerFontSize, textFontSize) {
  let scores = document.createElement("div");
  scores.className = "row";
  scores.id = "scores";
  for (let i = 0; i < num; i++) {
    let col = document.createElement("div");
    col.className = "col mb-4";
    let card = createCard(`score_card_${i}`, width=width,
        headerFontSize=headerFontSize, textFontSize=textFontSize);
    col.appendChild(card);
    scores.appendChild(col);
  }
  return scores;
}

function createCardInfo() {
  let card = document.createElement("div");
  card.id = "card-info";
  card.className = "card mb-4";
  let img = document.createElement("img");
  img.id = "user_img";
  img.src ="res/user.svg"
  img.className = "card-img-top";
  img.style = "width:50%; margin:auto";
  card.appendChild(img);
  
  let infoTitle = createCardInfoTitle();
  let infoBody = createCardInfoBody();
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

function createCardInfoBody() {
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

function createModal(id) {
  let modal = document.createElement("div");
  modal.className = "modal fade";
  modal.id = id;
  modal.tabIndex = -1;
  modal.setAttribute("aria-labelledby", "registerPlayerModalLabel");
  modal.setAttribute("aria-hidden", "true");
  let modalDialog = document.createElement("div");
  modalDialog.className = "modal-dialog";
  let modalContent = document.createElement("div");
  modalContent.className = "modal-content";
  let modalHeader = document.createElement("div");
  modalHeader.className = "modal-header";
  let modalTitle = document.createElement("h1");
  modalTitle.className = "modal-title fs-5";
  let btnClose = document.createElement("button");
  btnClose.type = "button";
  btnClose.className = "btn-close";
  btnClose.setAttribute("data-bs-dismiss", "modal");
  btnClose.setAttribute("aria-label", "Close");
  let modalBody = document.createElement("div");
  modalBody.className = "modal-body";
  modalHeader.appendChild(modalTitle);
  modalHeader.appendChild(btnClose);
  modalContent.appendChild(modalHeader);
  modalContent.appendChild(modalBody);
  modalDialog.appendChild(modalContent);
  modal.appendChild(modalDialog);

  return modal;
}

function createRadio(id, options) {
  let radioGroup = document.createElement("div");
  for (let i = 0; i < options.length; i++) {
    let radio = document.createElement("div");
    radioGroup.className = "form-check";
    let input = document.createElement("input");
    input.className = "form-check-input";
    input.type = "radio";
    input.name = id;
    input.id = `${id}_${i}`;
    let label = document.createElement("label");
    label.className = "form-check-label";
    label.setAttribute("for", `${id}_${i}`);
    label.innerHTML = options[i];
    radio.appendChild(input);
    radio.appendChild(label);
    radioGroup.appendChild(radio);
  }
  // Default check first option
  radioGroup.firstChild.firstChild.setAttribute("checked", "");

  return radioGroup;
}

function createCheck(id, options) {
  let checkGroup = document.createElement("div");
  for (let i = 0; i < options.length; i++) {
    let check = document.createElement("div");
    checkGroup.className = "form-check";
    let input = document.createElement("input");
    input.className = "form-check-input";
    input.type = "checkbox";
    input.name = id;
    input.id = `${id}_${i}`;
    let label = document.createElement("label");
    label.className = "form-check-label";
    label.setAttribute("for", `${id}_${i}`);
    label.innerHTML = options[i];
    check.append(input);
    check.append(label);
    checkGroup.append(check);
  }

  return checkGroup;
}

function launchModal(id) {
  new bootstrap.Modal(document.getElementById(id)).show();
}

function hideModal(id) {
  new bootstrap.Modal(document.getElementById(id)).hide();
}

function addTitleModal(id, title) {
  let modal = document.getElementById(id);
  let modalTitle = modal.querySelector(".modal-title");
  modalTitle.innerHTML = title;
}

function addContentModal(id, content) {
  let modal = document.getElementById(id);
  let modalBody = modal.querySelector(".modal-body");
  modalBody.appendChild(content);
}

function createdPlayerForm(onClick) {
  let form = document.createElement("form");
  let playerName = document.createElement("div");
  playerName.className = "mb-3";
  let playerNameLabel = document.createElement("label");
  playerNameLabel.className = "form-label";
  playerNameLabel.setAttribute("for", "playerName"); // ??
  playerNameLabel.innerHTML = "Player name";
  let playerNameInput = document.createElement("input");
  playerNameInput.id = "playerName";
  playerNameInput.type = "text";
  playerNameInput.className = "form-control";
  let button = document.createElement("button");
  button.className = "btn btn-primary";
  button.type = "button";
  button.innerHTML = "Add player";
  button.setAttribute("onclick", onClick);

  playerName.appendChild(playerNameLabel);
  playerName.appendChild(playerNameInput);
  form.appendChild(playerName);
  form.appendChild(button);

  return form;
}

function createAlert(severity, text) {
  let alert = document.createElement("div");
  alert.className = "alert";
  alert.classList.add(severity);
  alert.role = "alert";
  alert.innerHTML = text;

  return alert;
}

// Update functions

function updateTitle(text) {
  let title = document.getElementById("title");
  title.innerHTML = text;
}

function updateTable(tableId, header, data, closedNumbers) {
  let table = document.getElementById(tableId);
  if (header) {
    let htr = table.firstChild.firstChild;
    // TODO: closed numbers only for cricket
    // TODO: querySelector
    let i;
    i = 0;
    for (let th = htr.firstChild; th; th = th.nextSibling) {
      th.innerHTML = header[i];
      if (closedNumbers && closedNumbers.includes(header[i])) {
        th.style.color = "#A0A0A0";
      }
      i++;
    }
  }
  if (data) {
    let rows = table.querySelectorAll("tbody tr");
    for (let i = 0; i < data.length; i++) {
      let cols = rows[i].querySelectorAll("td");
      for (let j = 0; j < cols.length; j++) {
        cols[j].innerHTML = data[i][j];
      }
    }
  }
}

function highlightTableRow(tableId, row) {
  let table = document.getElementById(tableId);
  let rows = table.querySelectorAll("tbody tr");
  let selRow = table.querySelector(`tbody tr:nth-child(${row+1}`);
  rows.forEach(function(r) {
    r.classList.remove("table-active");
  });
  selRow.classList.add("table-active");
}

function updateCardInfo(name, infoList) {
  let title = document.getElementById("card-info-title");
  title.innerHTML = name;
  let list = document.getElementById("card-info-list");
  while (list.lastElementChild) {
    list.removeChild(list.lastElementChild);
  }
  for (let i = 0; i < infoList.length; i++) {
    let listItem = document.createElement("li");
    listItem.className = "list-group-item";
    listItem.innerHTML = infoList[i];
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

function udpdateSimpleCard(id, text) {
  let card = document.getElementById(id);
  let textBody = card.querySelector("div h1");
  textBody.innerHTML = text;
}

function updateScoreCards(players, scores) {
  let scoreCards = document.getElementById("scores");
  // TODO: temp
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

function highlightScoreCard(idx) {
  let scoreCards = document.getElementById("scores");
  // TODO: temp
  let i = 0;
  for (let col = scoreCards.firstChild; col; col = col.nextSibling) {
    let card = col.firstChild;
    let body = card.lastChild;
    body.classList.remove("custom-highlight");
    i++;
  }
  let card = document.getElementById(`score_card_${idx}`);
  let body = card.lastChild;
  body.classList.add("custom-highlight");
}
