#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QFormLayout>
#include <QMessageBox>
#include <MQTTClient.h>

// Variabili globali per la connessione MQTT
MQTTClient client;
const int TIMEOUT = 10000L;
const char* TOPIC = "home/gate";
const char* PAYLOAD = "opengate";

/*
 * classe che rappresenta la schermata dei controlli, per ora contenente solo il pulsante per aprire il cancello.
 */
class ControlWindow : public QWidget {
public:
    ControlWindow() {
        // Crea il pulsante per aprire il cancello
        QPushButton *sendButton = new QPushButton("Gate Opening", this);

        // Collega il pulsante alla funzione per inviare il messaggio MQTT
        connect(sendButton, &QPushButton::clicked, this, &ControlWindow::sendOpenGateMessage);

        // Layout per il pulsante
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(sendButton);
        setLayout(layout);
        setWindowTitle("Gate Control");
    }

private slots:
    /*
    *
*/
    void sendOpenGateMessage() {
        // Pubblica il payload al topic
        MQTTClient_message pubmsg = MQTTClient_message_initializer;
        pubmsg.payload = (void*) PAYLOAD;
        pubmsg.payloadlen = (int)strlen(PAYLOAD);
        pubmsg.qos = 1;
        pubmsg.retained = 0;
        MQTTClient_deliveryToken token;

        int rc = MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
        if (rc == MQTTCLIENT_SUCCESS) {
            //QMessageBox::information(this, "Successo", "Messaggio inviato con successo!");
            MQTTClient_waitForCompletion(client, token, TIMEOUT);
        } else {
            QMessageBox::critical(this, "Errore", "Errore nell'invio del messaggio MQTT!");
        }
    }
};

class MainWindow : public QWidget {
public:
    MainWindow() {
        // Campi per l'inserimento dei dati necessari alla connessione
        brokerInput = new QLineEdit(this);
        clientIdInput = new QLineEdit(this);
        usernameInput = new QLineEdit(this);
        passwordInput = new QLineEdit(this);
        passwordInput->setEchoMode(QLineEdit::Password);  // Maschera la password

        QPushButton *connectButton = new QPushButton("Connetti", this);

        // Layout del form
        QFormLayout *formLayout = new QFormLayout;
        formLayout->addRow("Broker Address:", brokerInput);
        formLayout->addRow("Client ID:", clientIdInput);
        formLayout->addRow("Username:", usernameInput);
        formLayout->addRow("Password:", passwordInput);
        formLayout->addWidget(connectButton);
        setLayout(formLayout);

        // Collegamento del pulsante di connessione alla funzione connectToBroker
        connect(connectButton, &QPushButton::clicked, this, &MainWindow::connectToBroker);

        setWindowTitle("Connessione MQTT");
    }

private slots:
    void connectToBroker() {
        const QString broker = brokerInput->text();
        const QString clientId = clientIdInput->text();
        const QString username = usernameInput->text();
        const QString password = passwordInput->text();

        if (broker.isEmpty() || clientId.isEmpty() || username.isEmpty() || password.isEmpty()) {
            QMessageBox::warning(this, "Dati mancanti", "Per favore inserisci tutti i dati.");
            return;
        }

        // Crea il client MQTT
        MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
        conn_opts.keepAliveInterval = 20;
        conn_opts.cleansession = 1;

        // Aggiungi username e password alle opzioni di connessione
        conn_opts.username = username.toStdString().c_str();
        conn_opts.password = password.toStdString().c_str();

        MQTTClient_create(&client, broker.toStdString().c_str(), clientId.toStdString().c_str(), MQTTCLIENT_PERSISTENCE_NONE, NULL);

        // Connessione al broker
        int rc = MQTTClient_connect(client, &conn_opts);
        if (rc == MQTTCLIENT_SUCCESS) {
            QMessageBox::information(this, "Successo", "Connessione al broker riuscita!");

            // Apri la finestra di controllo
            ControlWindow *controlWindow = new ControlWindow();
            controlWindow->resize(200, 100);
            controlWindow->show();
            this->close();  // Chiudi la finestra principale
        } else {
            QMessageBox::critical(this, "Errore", "Connessione al broker fallita!");
        }
    }

private:
    QLineEdit *brokerInput;
    QLineEdit *clientIdInput;
    QLineEdit *usernameInput;
    QLineEdit *passwordInput;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Crea la finestra principale
    MainWindow mainWindow;
    mainWindow.resize(400, 300);
    mainWindow.show();

    // Esegui l'applicazione
    return app.exec();
}
