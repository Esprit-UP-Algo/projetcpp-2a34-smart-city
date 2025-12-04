#ifndef SERIALREADER_H
#define SERIALREADER_H

#include <QObject>
#include <QSerialPort>

class SerialReader : public QObject
{
    Q_OBJECT

public:
    explicit SerialReader(QObject *parent = nullptr);
    void openPort(QString portName);

signals:
    void uidReceived(QString uid);

private slots:
    void onReadyRead();

private:
    QSerialPort *serial;
};

#endif
