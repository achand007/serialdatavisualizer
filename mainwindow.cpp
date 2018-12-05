
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <QtGlobal>

#include <QSerialPort>
#include <QStringList>
#include <QTextStream>

#include "serialportreader.h"

MainWindow::MainWindow(QSerialPort *serialPort, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  standardOutput(stdout),
  m_standardOutput(stdout),
  m_serialPort(serialPort)
{
  ui->setupUi(this);
  setGeometry(400, 250, 542, 390);
  
  // setupDemo();


  // Serial Port ------------------------------------------------------
  /*
  serialPort.setPortName(serialPortName);

  const int serialPortBaudRate = QSerialPort::Baud9600;
  serialPort.setBaudRate(serialPortBaudRate);

  // Open port
  if (!serialPort.open(QIODevice::ReadOnly)) {
      standardOutput << QObject::tr("Failed to open port %1, error: %2")
                        .arg(serialPortName)
                        .arg(serialPort.errorString())
                     << endl;
  }*/

  // Instantiate
  // SerialPortReader serialPortReader(&serialPort);
  // m_serialPort = &serialPort ;
  // -------------------------------------------------------------------

  // setup a timer that repeatedly calls serial port reading
  connect(m_serialPort, &QSerialPort::readyRead, this, &MainWindow::handleReadyRead);
  // connect(m_serialPort, &QSerialPort::errorOccurred, this, &SerialPortReader::handleError);
  connect(&m_timer, &QTimer::timeout, this, &MainWindow::handleTimeout);

  m_timer.start(100);  // Interval 0 means to refresh as fast as possible

}

void MainWindow::setupDemo()
{

  setupRealtimeDataDemo(ui->customPlot);

  setWindowTitle("QCustomPlot: "+demoName);
  statusBar()->clearMessage();

  ui->customPlot->replot();
}


void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
  demoName = "Real Time Data Demo";
  
  // include this section to fully disable antialiasing for higher performance:
  /*
  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);
  */
  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));


  customPlot->addGraph(); // red line
  customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

  QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
  timeTicker->setTimeFormat("%h:%m:%s");
  customPlot->xAxis->setTicker(timeTicker);
  customPlot->axisRect()->setupFullAxesBox();
  customPlot->yAxis->setRange(-1.2, 1.2);
  
  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));
  
  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible




}







void MainWindow::realtimeDataSlot()
{
  static QTime time(QTime::currentTime());
  // calculate two new data points:
  double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
  static double lastPointKey = 0;
  if (key-lastPointKey > 0.002) // at most add point every 2 ms
  {

    // Get and prepare data from serialportreader class
   // QDataStream ds(serialPortReader_.m_readData);
   // int size; // Since the size you're trying to read appears to be 2 bytes
   // ds >> size;

    // add data to lines:
    ui->customPlot->graph(0)->addData(key, 10);
    ui->customPlot->graph(1)->addData(key, qCos(key)+qrand()/(double)RAND_MAX*0.5*qSin(key/0.4364));

    // rescale value (vertical) axis to fit the current data:
    ui->customPlot->graph(0)->rescaleValueAxis();
    ui->customPlot->graph(1)->rescaleValueAxis(true);
    lastPointKey = key;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
  ui->customPlot->xAxis->setRange(key, 8, Qt::AlignRight);
  ui->customPlot->replot();
  
  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
    ui->statusBar->showMessage(
          QString("%1 FPS, Total Data points: %2")
          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
          .arg(ui->customPlot->graph(0)->data()->size()+ui->customPlot->graph(1)->data()->size())
          , 0);
    lastFpsKey = key;
    frameCount = 0;
  }





}

void MainWindow::bracketDataSlot()
{
  double secs = QCPAxisTickerDateTime::dateTimeToKey(QDateTime::currentDateTime());
  
  // update data to make phase move:
  int n = 500;
  double phase = secs*5;
  double k = 3;
  QVector<double> x(n), y(n);
  for (int i=0; i<n; ++i)
  {
    x[i] = i/(double)(n-1)*34 - 17;
    y[i] = qExp(-x[i]*x[i]/20.0)*qSin(k*x[i]+phase);
  }
  ui->customPlot->graph()->setData(x, y);
  
  itemDemoPhaseTracer->setGraphKey((8*M_PI+fmod(M_PI*1.5-phase, 6*M_PI))/k);
  
  ui->customPlot->replot();
  
  // calculate frames per second:
  double key = secs;
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
    ui->statusBar->showMessage(
          QString("%1 FPS, Total Data points: %2")
          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
          .arg(ui->customPlot->graph(0)->data()->size())
          , 0);
    lastFpsKey = key;
    frameCount = 0;
  }
}

void MainWindow::setupPlayground(QCustomPlot *customPlot)
{
  Q_UNUSED(customPlot)
}

MainWindow::~MainWindow()
{
  delete ui;
}







void MainWindow::handleReadyRead()
{

    m_readData.append(m_serialPort->readAll());

    if (!m_timer.isActive())
        m_timer.start(100);

}

void MainWindow::handleTimeout()
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


         // Something like this should work, using a data stream to read from the buffer:
         //
         // QDataStream ds(buffer);
         //  short size; // Since the size you're trying to read appears to be 2 bytes
         //  ds >> size;
         //

    }


    // QCoreApplication::quit();
}

void MainWindow::handleError(QSerialPort::SerialPortError serialPortError)
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

































