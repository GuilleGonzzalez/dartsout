
// Test
window.onload = (event) => {
	let game = document.getElementById("game");
	let table = createCricketTable("cricket", 3);
	game.appendChild(table);
	for (let i = 0; i < 6; i++) {
		let card = createSimpleCard(`Card ${i}`, `Text ${i}`);
		game.appendChild(card);
	}
	let info = createCardInfo();
	game.appendChild(info);
}

function createCricketTable(id, rows) {
	let table = document.createElement("table");
	table.setAttribute('id', id);
	table.setAttribute('class', 'table text-center');
	let thead = document.createElement("thead");
	thead.setAttribute('class', 'table-light fw-bolder fs-2');
	let h_tr = document.createElement("tr");
	for (let i = 0; i < 8; i++) {
		let th = document.createElement("th");
		th.setAttribute('scope', 'col');
		th.innerHTML = 'test';
		h_tr.appendChild(th);
	}
	thead.appendChild(h_tr);
	let tbody = document.createElement("tbody");
	tbody.setAttribute('class', 'fs-2');
	for (let i = 0; i < rows; i++) {
		let b_tr = document.createElement("tr");
		for (let j = 0; j < 8; j++) {
			let td = document.createElement("td");
			td.setAttribute('scope', 'row');
			td.innerHTML = `${i},${j}`;
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

function createCardInfo() {
	let card = document.createElement("div");
	card.className = "card mb-4";
	card.style = "width: 18rem;";
	let img = document.createElement("img");
	img.id = "user_img";
	img.src ="res/user.svg"
	img.className = "card-img-top";
	img.height = 100;
	card.appendChild(img);
	
	let infoTitle = createCardInfoTitle("Paco");
	let infoBody = createCardInfoBody();
	card.appendChild(infoTitle);
	card.appendChild(infoBody);

	return card;
}

function createCardInfoTitle(title) {
	let cardTitle = document.createElement("h2");
	cardTitle.className = "card-title text-center fs-2";
	cardTitle.innerHTML = title;

	return cardTitle;
}

function createCardInfoBody() {
	let cardBody = document.createElement("div");
	cardBody.className = "card-body";
	let list = document.createElement("ul");
	list.className = "list-group list-group-flush fs-5 fw-bolder";
	let listItems = ["Round", "Game score", "Round score"];
	for (let i = 0; i < listItems.length; i++) {
		let listItem = document.createElement("li");
		listItem.className = "list-group-item";
		listItem.innerHTML = listItems[i];
		list.appendChild(listItem);
	}
	let row = document.createElement("div");
	row.className = "row";
	for (let i = 0; i < 3; i++) {
		let col = document.createElement("div");
		col.className = "col text-center";
		let img = document.createElement("img");
		img.id = `dart${i}_img`;
		img.src = "res/dart.svg";
		img.className = "card-img-top";
		img.height = 50;
		let br = document.createElement("br");
		let spanNum = document.createElement("span");
		spanNum.id = `dart${i}_num`;
		spanNum.className = "fs-2 fw-bolder";
		let spanZone = document.createElement("span");
		spanZone.id = `dart${i}_zone`;
		col.appendChild(img);
		col.appendChild(br);
		col.appendChild(spanNum);
		col.appendChild(br);
		col.appendChild(spanZone);
		row.appendChild(col);
	}
	cardBody.appendChild(list);
	cardBody.appendChild(row);

	return cardBody;
}
