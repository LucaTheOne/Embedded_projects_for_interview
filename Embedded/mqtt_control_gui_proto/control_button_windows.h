#ifndef CONTROL_BUTTON_WINDOWS_H
#define CONTROL_BUTTON_WINDOWS_H

class ButtonWindow : public QWidget {

public:
    ButtonWindow() {
        // Layout a griglia per i pulsanti
        QGridLayout *layout = new QGridLayout;

        // Crea 6 pulsanti
        for (int i = 0; i < 6; i++) {
            QPushButton *button = new QPushButton(QString("Pulsante %1").arg(i + 1), this);
            layout->addWidget(button, i / 3, i % 3);  // Disporre i pulsanti in 2 righe e 3 colonne

            // Collega ogni pulsante a una funzione specifica
            connect(button, &QPushButton::clicked, this, [this, i]() { buttonFunction(i + 1); });
        }

        // Imposta il layout
        setLayout(layout);
        setWindowTitle("Finestra con 6 Pulsanti");
    }

private slots:
    void buttonFunction(int buttonNumber) {
        // Mostra un messaggio diverso a seconda del pulsante premuto
        QMessageBox::information(this, "Pulsante Premuto", QString("Hai premuto il pulsante %1").arg(buttonNumber));

        // Aggiungi qui le funzioni specifiche per ogni pulsante
        if (buttonNumber == 1) {
            // Funzione del pulsante 1
        } else if (buttonNumber == 2) {
            // Funzione del pulsante 2
        } // E così via...

        switch(buttonNumber){
            case 1:

        }
    }
};

#endif // CONTROL_BUTTON_WINDOWS_H
