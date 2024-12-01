#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>


using namespace std;

void configureSerial(HANDLE& serialHandle, const char* portName) {
    serialHandle = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (serialHandle == INVALID_HANDLE_VALUE) {
        cerr << "No se pudo abrir el puerto serial." << endl;
        exit(1);
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(serialHandle, &dcbSerialParams)) {
        cerr << "Error al obtener la configuración del puerto serial." << endl;
        exit(1);
    }

    dcbSerialParams.BaudRate = CBR_9600;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(serialHandle, &dcbSerialParams)) {
        cerr << "Error al configurar el puerto serial." << endl;
        exit(1);
    }

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    SetCommTimeouts(serialHandle, &timeouts);
}

int main() {
    HANDLE serialHandle;
    configureSerial(serialHandle, "COM3"); // Cambiar "COM3" al puerto serial de tu Arduino.

    ofstream file("inventario.txt", ios::app);
    if (!file.is_open()) {
        cerr << "No se pudo abrir el archivo de inventario." << endl;
        return 1;
    }

    cout << "Servidor iniciado. Esperando datos..." << endl;

    char buffer[256];
    DWORD bytesRead;
    while (true) {
        if (ReadFile(serialHandle, buffer, sizeof(buffer), &bytesRead, NULL)) {
            string data(buffer, bytesRead);

            if (!data.empty()) {
                cout << "Producto recibido: " << data << endl;
                file << data << endl;

                // Confirmar recepción
                const char* confirmation = "1";
                DWORD bytesWritten;
                WriteFile(serialHandle, confirmation, 1, &bytesWritten, NULL);
            }
        }
    }

    CloseHandle(serialHandle);
    file.close();
    return 0;
}
