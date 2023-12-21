valores = []

num_elements = 10

for (var i = 0; i < num_elements; i++) {
  var ul = document.getElementById("lista");
  var li = document.createElement("li");
  id = "item" + (i + 1)
  li.appendChild(document.createTextNode("0"));
  li.setAttribute("id", id);
  ul.appendChild(li);

  valores[i] = 0
}

function lerDados() {
  fetch("http://192.168.50.12/leitura")
  .then(response => response.text())
  .then(texto => {
    for (var i = num_elements - 1; i > 0; i--) {
      valores[i] = valores[i - 1]
    }
    valores[0] = texto

    for (var i = 0; i < num_elements; i++) {
      id = "item" + (i + 1)
      document.getElementById(id).innerHTML = valores[i]
    }
  })
}

function mandarDados() {
  ws = document.getElementById("ws").value
  amp = document.getElementById("amp").value
  
  fetch("http://192.168.50.12/escrita?ws=" + ws + "&amplitude=" + amp)
  .then(response => response.text())
  .then(text => console.log(text))
}

setInterval(lerDados, 500)