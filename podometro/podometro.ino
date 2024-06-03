#include <ArduinoBLE.h>

const int led_pin = 13;

BLEService podometroService("bdc1fe11-f4ab-41dc-af14-da79a495f3eb"); // Bluetooth® Low Energy LED Service

// Bluetooth® Low Energy LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEStringCharacteristic txtCharacteristic("89b782c7-dbae-453e-9081-bbe813727e32", BLERead | BLEWrite, 512);

void setup() {
  Serial.begin(115200);

  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);

  Serial.println("Iniciando conexion BluetoothLE!");
  if (!BLE.begin()) {
    Serial.println("Error al inciar la conexion BluetoothLE!");
    while (1){
      led_blink(1);
    }
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("-Podometro IOT-");
  BLE.setAdvertisedService(podometroService);

  // add the characteristic to the service
  podometroService.addCharacteristic(txtCharacteristic);

  // add service
  BLE.addService(podometroService);

  // set the initial value for the characeristic:
  txtCharacteristic.writeValue("0");

  // start advertising
  BLE.advertise();

  Serial.println("Buscando dispositivos...");
  delay(1000);
}

void loop() {
  // listen for Bluetooth® Low Energy peripherals to connect:
  BLEDevice central = BLE.central();
  Serial.println("Buscando dispositivos...");
  // if a central is connected to peripheral:
  if (central) {
    led_blink(2);
    Serial.print("Dispositivo conectado: ");
    // print the central's MAC address:
    Serial.println(central.address());
    delay(1000);
    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (txtCharacteristic.written()) {
        Serial.print("Texto recibido: ");
        Serial.println(txtCharacteristic.value());        
      }
      
      delay(100);
    }
    // when the central disconnects, print it out:
    led_blink(3);
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
  led_blink(0);
}

void led_blink(int type){
  switch(type){
    case 0: //Buscando dispositivo
      digitalWrite(led_pin, HIGH);
      delay(100);
      digitalWrite(led_pin, LOW);
      delay(100);
      digitalWrite(led_pin, HIGH);
      delay(100);
      digitalWrite(led_pin, LOW);
      delay(500);
      break;
    
    case 1: // Notificar error
      digitalWrite(led_pin, HIGH);
      delay(100);
      digitalWrite(led_pin, LOW);
      delay(100);
      break;
    
    case 2: //Conectado
      digitalWrite(led_pin, HIGH);
      break;
    
    default: //Apagado
      digitalWrite(led_pin, LOW);
      break;
  }
}