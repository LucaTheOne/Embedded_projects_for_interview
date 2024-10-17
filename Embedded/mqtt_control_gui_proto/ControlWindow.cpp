#include "controlwindow.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <MQTTClient.h>

extern MQTTClient client;  // Definiamo il client come esterno
const int TIMEOUT = 10000L;
const char* TOPIC = "home/gate";
const char* PAYLOAD = "opengate";

ControlWindow::ControlWindow(QWidget *parent)
    : QWidget(parent) {
    // Crea il pulsante per aprire il cancello
    QPushButton *sendButton = new QPushButton("Apri Cancello", this);

    // Collega il pulsante alla funzione per inviare il messaggio MQTT
    connect(sendButton, &QPushButton::clicked, this, &ControlWindow::sendOpenGateMessage);

    // Layout per il pulsante
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(sendButton);
    setLayout(layout);
    setWindowTitle("Controllo Cancello");
}

void ControlWindow::sendOpenGateMessage() {
    // Pubblica il payload al topic
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    pubmsg.payload = (void*) PAYLOAD;
    pubmsg.payloadlen = (int)strlen(PAYLOAD);
    pubmsg.qos = 1;
    pubmsg.retained = 0;
    MQTTClient_deliveryToken token;

    int rc = MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    if (rc == MQTTCLIENT_SUCCESS) {
        QMessageBox::information(this, "Successo", "Messaggio inviato con successo!");
        MQTTClient_waitForCompletion(client, token, TIMEOUT);
    } else {
        QMessageBox::critical(this, "Errore", "Errore nell'invio del messaggio MQTT!");
    }
}
