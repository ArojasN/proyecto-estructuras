#include <SPI.h>
#include <MFRC522.h>
#include "DataQueue.h" // Incluye la implementación de la cola

// Pines del módulo RFID
#define RST_PIN 49
#define SS_PIN 53

MFRC522 mfrc522(SS_PIN, RST_PIN); // Inicializa el lector RFID
DataQueue queue; // Cola para almacenar los IDs leídos

void setup() {
    Serial.begin(9600); // Configura la comunicación serial
    SPI.begin();        // Inicializa el bus SPI
    mfrc522.PCD_Init(); // Inicializa el módulo RFID
    Serial.println("Sistema de inventario iniciado.");
}

void loop() {
    // Detectar una nueva tarjeta RFID
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        // Leer el ID único de la tarjeta
        String id = "";
        for (byte i = 0; i < mfrc522.uid.size; i++) {
            id += String(mfrc522.uid.uidByte[i], HEX);
        }

        // Encolar el ID leído
        queue.enqueue(id);
        Serial.println("ID leído: " + id);
        mfrc522.PICC_HaltA(); // Detener la lectura de la tarjeta
    }

    // Enviar datos al servidor si la cola no está vacía
    if (!queue.isEmpty()) {
        String data = queue.frontData(); // Obtiene el primer dato de la cola
        Serial.println(data);           // Envía el dato al servidor

        delay(100); // Esperar una confirmación del servidor

        // Simular confirmación: El servidor debe enviar '1' si recibe el dato
        if (Serial.available() > 0 && Serial.read() == '1') {
            queue.dequeue(); // Elimina el dato de la cola si se confirma
            Serial.println("Dato confirmado por el servidor.");
        }
    }
}
