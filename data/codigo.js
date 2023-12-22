valores_loop = []
valores_pwm = []

num_elements = 10

for (var i = 0; i < num_elements; i++) {
  var ul = document.getElementById("lista-loop");
  var li = document.createElement("li");
  id = "loop-item" + (i + 1)
  li.appendChild(document.createTextNode("0"));
  li.setAttribute("id", id);
  ul.appendChild(li);

  valores_loop[i] = 0
}

for (var i = 0; i < num_elements; i++) {
  var ul = document.getElementById("lista-pwm");
  var li = document.createElement("li");
  id = "pwm-item" + (i + 1)
  li.appendChild(document.createTextNode("0"));
  li.setAttribute("id", id);
  ul.appendChild(li);

  valores_pwm[i] = 0
}

function lerDados() {
  fetch("http://192.168.50.12/leitura")
  .then(response => response.json())
  .then(dados => {
    for (var i = num_elements - 1; i > 0; i--) {
      valores_loop[i] = valores_loop[i - 1]
    }
    valores_loop[0] = dados.Loop_time

    for (var i = 0; i < num_elements; i++) {
      id = "loop-item" + (i + 1)
      document.getElementById(id).innerHTML = valores_loop[i]
    }

    for (var i = num_elements - 1; i > 0; i--) {
      valores_pwm[i] = valores_pwm[i - 1]
    }
    valores_pwm[0] = dados.PWM

    for (var i = 0; i < num_elements; i++) {
      id = "pwm-item" + (i + 1)
      document.getElementById(id).innerHTML = valores_pwm[i]
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