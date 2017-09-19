#ifndef BITCOIN_QTMININGPAGE_H
#define BITCOIN_QTMININGPAGE_H

#include <QWidget>

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QTime>
#include <QTimer>
#include <QStringList>
#include <QMap>
#include <QSettings>
#include <QDebug>

// Log types
#define STARTED 0
#define SHARE_SUCCESS 1
#define SHARE_FAIL 2
#define ERROR 3
#define LONGPOLL 4

namespace Ui {
    class MiningPage;
}

class MiningPage : public QWidget
{
    Q_OBJECT
public:
    explicit MiningPage(QWidget *parent = 0);
    ~MiningPage();

    bool minerActive;
    QProcess *minerProcess;
    QMap<int, double> threadSpeed;
    QTimer *readTimer;
    int initThreads;

public Q_SLOTS:
    void startPressed();
    void startMining();
    void stopMining();
    void reportToList(QString, int);
    void minerStarted();
    void minerError(QProcess::ProcessError);
    void minerFinished();
    void readProcessOutput();
    void on_startButton_clicked();

private Q_SLOTS:

private:
    Ui::MiningPage *ui;
    void resetMiningButton();
};

#endif // BITCOIN_QTMININGPAGE_H
