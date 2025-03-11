#ifndef DATASIMULATOR_H
#define DATASIMULATOR_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <QHostAddress>
#include <QRandomGenerator>
#include <QByteArray>
#include <QDateTime>

class DataSimulator : public QObject
{
    Q_OBJECT

public:
    explicit DataSimulator(int interval = 1000,
                           int pointsPerUpdate = 5,
                           const QString &multicastAddress = "224.0.0.1",
                           int port = 50000,
                           QObject *parent = nullptr);
    ~DataSimulator();

    void start();
    void stop();

private slots:
    void generateAndSendData();

private:
    QTimer m_timer;
    QUdpSocket m_socket;
    QHostAddress m_multicastAddress;
    int m_port;
    int m_pointsPerUpdate;
    QRandomGenerator m_randomGenerator;

    // Generate a random value within a reasonable range for the chart
    double generateValue();

    // Pack data into a binary format for transmission
    QByteArray packData(const QVector<double> &values, qint64 timestamp);
};

#endif // DATASIMULATOR_H
