#include "datasimulator.h"
#include <QDataStream>
#include <QDebug>

DataSimulator::DataSimulator(int interval, int pointsPerUpdate,
                             const QString &multicastAddress, int port,
                             QObject *parent)
    : QObject(parent)
    , m_multicastAddress(multicastAddress)
    , m_port(port)
    , m_pointsPerUpdate(pointsPerUpdate)
    , m_randomGenerator(QDateTime::currentMSecsSinceEpoch())
{
    m_timer.setInterval(interval);
    connect(&m_timer, &QTimer::timeout, this, &DataSimulator::generateAndSendData);

    qDebug() << "Data Simulator initialized with:";
    qDebug() << "- Interval:" << interval << "ms";
    qDebug() << "- Points per update:" << pointsPerUpdate;
    qDebug() << "- Multicast address:" << multicastAddress;
    qDebug() << "- Port:" << port;
}

DataSimulator::~DataSimulator()
{
    stop();
}

void DataSimulator::start()
{
    m_timer.start();
    qDebug() << "Data Simulator started";
}

void DataSimulator::stop()
{
    m_timer.stop();
    qDebug() << "Data Simulator stopped";
}

void DataSimulator::generateAndSendData()
{
    QVector<double> values;
    values.reserve(m_pointsPerUpdate);

    // Generate random values
    for (int i = 0; i < m_pointsPerUpdate; ++i) {
        values.append(generateValue());
    }

    // Get current timestamp
    qint64 timestamp = QDateTime::currentMSecsSinceEpoch();

    // Pack the data
    QByteArray datagram = packData(values, timestamp);

    // Send via UDP multicast
    qint64 bytesSent = m_socket.writeDatagram(datagram, m_multicastAddress, m_port);

    if (bytesSent != datagram.size()) {
        qWarning() << "Failed to send all data:" << m_socket.error();
    } else {
        qDebug() << "Sent" << values.size() << "data points at"
                 << QDateTime::fromMSecsSinceEpoch(timestamp).toString("hh:mm:ss.zzz");
    }
}

double DataSimulator::generateValue()
{
    // Generate values between 0 and 100 with some realistic patterns
    // This is a simple sine wave with some noise to make it look realistic
    static double phase = 0.0;
    phase += 0.1;

    double baseValue = 50.0 + 40.0 * sin(phase);
    double noise = m_randomGenerator.bounded(-5, 5);

    return qBound(0.0, baseValue + noise, 100.0);
}

QByteArray DataSimulator::packData(const QVector<double> &values, qint64 timestamp)
{
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::WriteOnly);

    // Set the serialization format version
    stream.setVersion(QDataStream::Qt_6_5);

    // Write timestamp
    stream << timestamp;

    // Write number of values
    stream << values.size();

    // Write each value
    for (const double &value : values) {
        stream << value;
    }

    return datagram;
}
