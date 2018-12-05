/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2018 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 25.06.18                                             **
**          Version: 2.0.1                                                **
****************************************************************************/

#include <QApplication>
#include "mainwindow.h"
#include <QSerialPort>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
  QApplication::setGraphicsSystem("raster");
#endif


  // Serial Port ------------------------------------------------------  
  QTextStream standardOutput(stdout);
  QSerialPort serialPort;
  const QString serialPortName = "ttyACM0";
  serialPort.setPortName(serialPortName);

  const int serialPortBaudRate = QSerialPort::Baud9600;
  serialPort.setBaudRate(serialPortBaudRate);

  // Open port
  if (!serialPort.open(QIODevice::ReadOnly)) {
      standardOutput << QObject::tr("Failed to open port %1, error: %2")
                        .arg(serialPortName)
                        .arg(serialPort.errorString())
                     << endl;
  }

  /*
  // Instantiate
  SerialPortReader serialPortReader(&serialPort);
  // -------------------------------------------------------------------
  */


  QApplication a(argc, argv);
  MainWindow w(&serialPort);
  w.show();
  
  return a.exec();
}
