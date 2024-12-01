#ifndef DATAQUEUE_H
#define DATAQUEUE_H

#include <Arduino.h> // Incluye las definiciones específicas de Arduino

struct Node {
    String data; // Almacena el ID como un String
    Node* next;  // Apuntador al siguiente nodo
    Node(const String& d) : data(d), next(nullptr) {}
};

class DataQueue {
private:
    Node* front; // Inicio de la cola
    Node* rear;  // Final de la cola
    int size;    // Tamaño de la cola

public:
    DataQueue() : front(nullptr), rear(nullptr), size(0) {}

    ~DataQueue() {
        while (!isEmpty()) {
            dequeue();
        }
    }

    void enqueue(const String& data) {
        Node* newNode = new Node(data);
        if (isEmpty()) {
            front = rear = newNode;
        } else {
            rear->next = newNode;
            rear = newNode;
        }
        size++;
    }

    void dequeue() {
        if (isEmpty()) return;
        Node* temp = front;
        front = front->next;
        delete temp;
        size--;
        if (front == nullptr) {
            rear = nullptr;
        }
    }

    String frontData() const {
        return isEmpty() ? "" : front->data;
    }

    bool isEmpty() const {
        return size == 0;
    }
};

#endif
