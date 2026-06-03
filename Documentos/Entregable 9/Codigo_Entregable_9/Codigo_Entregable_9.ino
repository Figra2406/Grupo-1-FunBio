const int PIN_BOMBA_IZQ = 4;
const int PIN_BOMBA_DER = 5;
const int PIN_NSLEEP = 6;

const int PIN_VALVULA_IZQ = 7;
const int PIN_VALVULA_DER = 8;

const int PIN_BUZZER = 9;
const int PIN_NFAULT = 10;

const unsigned long TIEMPO_INFLADO = 8000;
const unsigned long TIEMPO_MANTENER = 10000;
const unsigned long TIEMPO_DESINFLADO = 5000;
const unsigned long TIEMPO_ESPERA = 15000;

void setup() {
  pinMode(PIN_BOMBA_IZQ, OUTPUT);
  pinMode(PIN_BOMBA_DER, OUTPUT);
  pinMode(PIN_NSLEEP, OUTPUT);

  pinMode(PIN_VALVULA_IZQ, OUTPUT);
  pinMode(PIN_VALVULA_DER, OUTPUT);

  pinMode(PIN_BUZZER, OUTPUT);
  pinMode(PIN_NFAULT, INPUT);

  apagarBombas();
  cerrarValvulas();
  apagarBuzzer();

  digitalWrite(PIN_NSLEEP, LOW);

  delay(1000);
  beep(200);
}

void loop() {
  if (hayFallaDRV()) {
    modoEmergencia();
    return;
  }

  beep(100);
  habilitarDRV();
  cerrarValvulas();
  encenderBombas();

  unsigned long inicioInflado = millis();

  while (millis() - inicioInflado < TIEMPO_INFLADO) {
    if (hayFallaDRV()) {
      modoEmergencia();
      return;
    }
  }

  apagarBombas();

  unsigned long inicioMantener = millis();

  while (millis() - inicioMantener < TIEMPO_MANTENER) {
    if (hayFallaDRV()) {
      modoEmergencia();
      return;
    }
  }

  abrirValvulas();
  beep(300);

  delay(TIEMPO_DESINFLADO);

  cerrarValvulas();
  deshabilitarDRV();

  beep(150);
  delay(150);
  beep(150);

  delay(TIEMPO_ESPERA);
}

void habilitarDRV() {
  digitalWrite(PIN_NSLEEP, HIGH);
}

void deshabilitarDRV() {
  digitalWrite(PIN_NSLEEP, LOW);
}

void encenderBombas() {
  digitalWrite(PIN_BOMBA_IZQ, HIGH);
  digitalWrite(PIN_BOMBA_DER, HIGH);
}

void apagarBombas() {
  digitalWrite(PIN_BOMBA_IZQ, LOW);
  digitalWrite(PIN_BOMBA_DER, LOW);
}

void abrirValvulas() {
  digitalWrite(PIN_VALVULA_IZQ, HIGH);
  digitalWrite(PIN_VALVULA_DER, HIGH);
}

void cerrarValvulas() {
  digitalWrite(PIN_VALVULA_IZQ, LOW);
  digitalWrite(PIN_VALVULA_DER, LOW);
}

void encenderBuzzer() {
  digitalWrite(PIN_BUZZER, HIGH);
}

void apagarBuzzer() {
  digitalWrite(PIN_BUZZER, LOW);
}

void beep(unsigned long tiempo) {
  encenderBuzzer();
  delay(tiempo);
  apagarBuzzer();
}

bool hayFallaDRV() {
  return digitalRead(PIN_NFAULT) == LOW;
}

void modoEmergencia() {
  apagarBombas();
  deshabilitarDRV();
  abrirValvulas();

  for (int i = 0; i < 5; i++) {
    beep(300);
    delay(200);
  }

  delay(5000);
  cerrarValvulas();
}