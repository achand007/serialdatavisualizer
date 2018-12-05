#include "serialportreader.h"

#include <QCoreApplication>

SerialPortReader::SerialPortReader(QSerialPort *serialPort, QObject *parent) :
    QObject(parent),
    m_serialPort(serialPort),
    m_standardOutput(stdout)
{
    connect(m_serialPort, &QSerialPort::readyRead, this, &SerialPortReader::handleReadyRead);
    // connect(m_serialPort, &QSerialPort::errorOccurred, this, &SerialPortReader::handleError);
    connect(&m_timer, &QTimer::timeout, this, &SerialPortReader::handleTimeout);

    m_timer.start(100);  // Interval 0 means to refresh as fast as possible
}

void SerialPortReader::handleReadyRead()
{
    m_readData.append(m_serialPort->readAll());

    if (!m_timer.isActive())
        m_timer.start(100);
}

void SerialPortReader::handleTimeout()
{
    if (m_readData.isEmpty()) {
        m_standardOutput << QObject::tr("No data was currently available "
                                        "for reading from port %1")
                            .arg(m_serialPort->portName())
                         << endl;
    } else {
        m_standardOutput << QObject::tr("Data successfully received from port %1")
                            .arg(m_serialPort->portName())
                         << endl;
        m_standardOutput << m_readData << endl;

        /*
         * Something like this should work, using a data stream to read from the buffer:
         *
         * QDataStream ds(buffer);
           short size; // Since the size you're trying to read appears to be 2 bytes
           ds >> size;
         */

   }

    // QCoreApplication::quit();
}

void SerialPortReader::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (serialPortError == QSerialPort::ReadError) {
        m_standardOutput << QObject::tr("An I/O error occurred while reading "
                                        "the data from port %1, error: %2")
                            .arg(m_serialPort->portName())
                            .arg(m_serialPort->errorString())
                         << endl;
        QCoreApplication::exit(1);
    }
}

