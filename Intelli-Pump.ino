#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "intelliPump";  // Nombre de la red WiFi
const char* password = "1234567890";  // Contraseña de la red WiFi

int trig = 2;     //Transmisor
int eco = 4;      //Receptor
int duracion;     //Variable para duracion del pulso
float distancia_cm;    //Variable para hallar la distancia en cm
float vaso_altura_cm = 14.0; // Altura del vaso en cm

WebServer server(80);

void setup() {
  Serial.begin(115200);
  
  // Iniciar el modo de acceso a punto de acceso
  WiFi.mode(WIFI_AP);
  
  // Configurar el punto de acceso con el nombre y la contraseña
  WiFi.softAP(ssid, password);
  
  // Obtener la dirección IP del punto de acceso
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Dirección IP del ESP32: ");
  Serial.println(IP);

  Serial.print("SSID del ESP32: ");
  Serial.println(ssid);

  pinMode(trig, OUTPUT);
  pinMode(eco, INPUT);

  // Configurar las rutas para la interfaz web
  server.on("/", HTTP_GET, [](){
    String html = "<html><head>";
    html += "<meta charset='UTF-8'>";
    html += "<style>";
    html += "    .inputbox {";
    html += "        position: relative;";
    html += "        width: 196px;";
    html += "    }";
    html += "    .inputbox input {";
    html += "        position: relative;";
    html += "        width: 100%;";
    html += "        padding: 20px 10px 10px;";
    html += "        background: transparent;";
    html += "        outline: none;";
    html += "        box-shadow: none;";
    html += "        border: none;";
    html += "        color: #fff;";
    html += "        font-size: 1em;";
    html += "        letter-spacing: 0.05em;";
    html += "        transition: 0.5s;";
    html += "        z-index: 10;";
    html += "    }";
    html += "    .inputbox span {";
    html += "        position: absolute;";
    html += "        left: 0;";
    html += "        padding: 20px 10px 10px;";
    html += "        font-size: 1em;";
    html += "        color: #8f8f8f;";
    html += "        letter-spacing: 00.05em;";
    html += "        transition: 0.5s;";
    html += "        pointer-events: none;";
    html += "    }";
    html += "    .inputbox i {";
    html += "        position: absolute;";
    html += "        left: 0;";
    html += "        bottom: 0;";
    html += "        width: 100%;";
    html += "        height: 2px;";
    html += "        background: #002799;";
    html += "        border-radius: 4px;";
    html += "        transition: 0.5s;";
    html += "        pointer-events: none;";
    html += "        z-index: 9;";
    html += "    }";
    html += "    .inputbox input:valid~i,";
    html += "    .inputbox input:focus~i {";
    html += "        height: 44px;";
    html += "    }";
    html += "    button {";
    html += "        padding: 1.3em 3em;";
    html += "        font-size: 12px;";
    html += "        text-transform: uppercase;";
    html += "        letter-spacing: 2.5px;";
    html += "        font-weight: 500;";
    html += "        color: #000;";
    html += "        background-color: #fff;";
    html += "        border: none;";
    html += "        border-radius: 45px;";
    html += "        box-shadow: 0px 8px 15px rgba(0, 0, 0, 0.1);";
    html += "        transition: all 0.3s ease 0s;";
    html += "        cursor: pointer;";
    html += "        outline: none;";
    html += "    }";
    html += "    button:hover {";
    html += "        background-color: #000;";
    html += "        box-shadow: 0px 15px 20px #000;";
    html += "        color: #fff;";
    html += "        transform: translateY(-7px);";
    html += "    }";
    html += "    button:active {";
    html += "        transform: translateY(-1px);";
    html += "    }";
    html += "</style>";

    // JavaScript para actualizar el porcentaje en el campo de entrada y mostrar alertas
    html += "<script>";
    html += "function actualizarPorcentaje() {";
    html += "  var tinacoInput = document.getElementById('tinaco');";
    html += "  var porcentaje = calcularPorcentaje();";
    html += "  tinacoInput.value = porcentaje;";
    html += "  if (porcentaje < 30) {";
    html += "    alert('Nivel de agua demasiado bajo, iniciando llenado');";
    html += "  }";
    html += "}";
    html += "function calcularPorcentaje() {";
    html += "  var distancia = " + String(distancia_cm) + ";";
    html += "  var porcentaje = ((14 - distancia) / 14) * 100;";
    html += "  return porcentaje;";
    html += "}";
    html += "setInterval(actualizarPorcentaje, 1000);"; // Actualizar cada segundo
    html += "</script>";
    
    html += "</head><body>";
    html += "<h1>Sistema de Control de Agua</h1>";
    html += "<div class='input' style='margin-bottom:30px;'>";
    html += "    <input type='number' name='tinaco' id='tinaco'>";
    html += "    <span>Porcentaje del tinaco</span>";
    html += "    <i></i>";
    html += "</div>";
   
    html += "<button onclick='actualizarPorcentaje()'> Actualizar </button>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/submit", HTTP_GET, [](){
    String tinaco = server.arg("tinaco");
   
    
    
    Serial.println(tinaco);

    server.send(200, "text/html", "Distancia recibida");
  });

  // Iniciar el servidor
  server.begin();
}

void loop() {
  server.handleClient();

  //Enviamos el pulso
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  duracion = pulseIn(eco, HIGH);  //Recibe el pulso
  distancia_cm = duracion / 58.2;    //Calculo para hallar la distancia en cm

  delay(500);
}

