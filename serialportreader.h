#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <QByteArray>
#include <QSerialPort>
#include <QTextStream>
#include <QTimer>

class MainWindow; // forward declaration

class SerialPortReader : public QObject
{
    Q_OBJECT

public:
    explicit SerialPortReader(QSerialPort *serialPort, QObject *parent = nullptr);


private slots:
    void handleReadyRead();
    void handleTimeout();
    void handleError(QSerialPort::SerialPortError error);

private:
    QSerialPort *m_serialPort = nullptr;
    QByteArray m_readData;  // data read from serial port
    QTextStream m_standardOutput;
    QTimer m_timer;
    friend class MainWindow ;
};

#endif // SERIALPORTREADER_H


