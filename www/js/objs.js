
// Test
window.onload = (event) => {
	let game = document.getElementById("game");
	let table = createCricketTable("cricket", 3);
	game.appendChild(table);
	for (let i = 0; i < 6; i++) {
		let card = createSimpleCard(`Card ${i}`, `Text ${i}`);
		game.appendChild(card);
	}
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
