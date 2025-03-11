#include <QCoreApplication>
#include "datasimulator.h"
#include <QCommandLineParser>
#include <QCommandLineOption>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCommandLineParser parser;
    parser.setApplicationDescription("Data Simulator for RealChartApp");
    parser.addHelpOption();

    QCommandLineOption intervalOption(QStringList() << "i" << "interval",
                                      "Data generation interval in milliseconds",
                                      "ms", "1000");
    parser.addOption(intervalOption);

    QCommandLineOption pointsOption(QStringList() << "p" << "points",
                                    "Number of data points to generate per update",
                                    "count", "5");
    parser.addOption(pointsOption);

    QCommandLineOption addressOption(QStringList() << "a" << "address",
                                     "Multicast address",
                                     "ip", "224.0.0.1");
    parser.addOption(addressOption);

    QCommandLineOption portOption(QStringList() << "port",
                                  "UDP port",
                                  "port", "5000");
    parser.addOption(portOption);

    parser.process(app);

    int interval = parser.value(intervalOption).toInt();
    int points = parser.value(pointsOption).toInt();
    QString address = parser.value(addressOption);
    int port = parser.value(portOption).toInt();

    DataSimulator simulator(interval, points, address, port);
    simulator.start();

    return app.exec();
}
