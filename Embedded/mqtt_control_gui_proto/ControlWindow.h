#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <QWidget>

class ControlWindow : public QWidget {
    Q_OBJECT

public:
    ControlWindow(QWidget *parent = nullptr);

private slots:
    void sendOpenGateMessage();
};

#endif // CONTROLWINDOW_H
