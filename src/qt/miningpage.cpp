#include "miningpage.h"
#include "ui_miningpage.h"

#include "guiutil.h"

MiningPage::MiningPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MiningPage)
{
    ui->setupUi(this);
    minerActive = false;
    minerProcess = new QProcess(this);
    minerProcess->setProcessChannelMode(QProcess::MergedChannels);
    readTimer = new QTimer(this);
    initThreads = 0;

    connect(readTimer, SIGNAL(timeout()), this, SLOT(readProcessOutput()));
    connect(minerProcess, SIGNAL(started()), this, SLOT(minerStarted()));
    connect(minerProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(minerError(QProcess::ProcessError)));
    connect(minerProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(minerFinished()));
    connect(minerProcess, SIGNAL(readyRead()), this, SLOT(readProcessOutput()));
}

MiningPage::~MiningPage()
{
    minerProcess->kill();
    delete ui;
}

void MiningPage::startPressed()
{
    initThreads = ui->threadsBox->value();
    if (!minerActive)
        startMining();
    else
        stopMining();
}

void MiningPage::startMining()
{
    QStringList args;
    QString urlLine = QString("stratum+tcp://poolclone.artbyte.me:3333");
    QString userpassLine = QString("%1:%2").arg(ui->usernameLine->text(), ui->passwordLine->text());
    args << "--algo" << "scrypt";
    args << "--scantime" << ui->scantimeBox->text().toLatin1();
    args << "--url" << urlLine.toLatin1();
    args << "--userpass" << userpassLine.toLatin1();
    args << "--threads" << ui->threadsBox->text().toLatin1();
    args << "--retries" << "-1"; // Retry forever.
    args << "-P"; // This is needed for this to work correctly on Windows. Extra protocol dump helps flush the buffer quicker.

    threadSpeed.clear();

    // If minerd is in current path, then use that. Otherwise, assume minerd is in the path somewhere.
    QString program = QDir::current().filePath("minerd");
    if (!QFile::exists(program))
        program = "minerd";

    minerProcess->start(program,args);
    minerProcess->waitForStarted(-1);
    readTimer->start(500);
}

void MiningPage::stopMining()
{
    minerProcess->kill();
    readTimer->stop();
}

void MiningPage::readProcessOutput()
{
    QByteArray output;

    minerProcess->reset();

    output = minerProcess->readAll();

    QString outputString(output);

    if (!outputString.isEmpty())
    {
        QStringList list = outputString.split("\n", QString::SkipEmptyParts);
        int i;
        for (i=0; i<list.size(); i++)
        {
            QString line = list.at(i);

            // Ignore protocol dump
            if (!line.startsWith("[") || line.contains("JSON protocol") || line.contains("HTTP hdr"))
                continue;

            if (line.contains("(yay!!!)"))
                reportToList("Share accepted", SHARE_SUCCESS);
            else if (line.contains("(booooo)"))
                reportToList("Share rejected", SHARE_FAIL);
            else if (line.contains("LONGPOLL detected new block"))
                reportToList("LONGPOLL detected a new block", LONGPOLL);
            else if (line.contains("Supported options:"))
                reportToList("Miner didn't start properly. Try checking your settings.", ERROR);
            else if (line.contains("The requested URL returned error: 403"))
                reportToList("Couldn't connect. Please check your username and password.", ERROR);
            else if (line.contains("HTTP request failed"))
                reportToList("Couldn't connect. Please check pool server and port.", ERROR);
            else if (line.contains("JSON-RPC call failed"))
                reportToList("Couldn't communicate with server. Retrying in 30 seconds.", ERROR);
            else if (line.contains("thread ") && line.contains("khash/s"))
            {
                QString threadIDstr = line.at(line.indexOf("thread ")+7);
                int threadID = threadIDstr.toInt();

                int threadSpeedindx = line.indexOf(",");
                QString threadSpeedstr = line.mid(threadSpeedindx);
                threadSpeedstr.chop(8);
                threadSpeedstr.remove(", ");
                threadSpeedstr.remove(" ");
                threadSpeedstr.remove('\n');
                double speed=0;
                speed = threadSpeedstr.toDouble();

                threadSpeed[threadID] = speed;
            }
        }
    }
}

void MiningPage::on_startButton_clicked()
{
    startPressed();
}

void MiningPage::minerError(QProcess::ProcessError error)
{
    if (error == QProcess::FailedToStart)
    {
        reportToList("Miner failed to start. Make sure you have the minerd executable and libraries in the same directory as ArtByte-Qt.", ERROR);
    }
}

void MiningPage::minerFinished()
{
    reportToList("Miner exited.", ERROR);
    ui->list->addItem("");
    minerActive = false;
    resetMiningButton();
}

void MiningPage::minerStarted()
{
    if (!minerActive)
        reportToList("Miner started. You might not see any output for a few minutes.", STARTED);
    minerActive = true;
    resetMiningButton();
}

void MiningPage::reportToList(QString msg, int type)
{
    QString message = QString("[%1] - %2").arg(QTime::currentTime().toString(), msg);

    switch(type)
    {
        case SHARE_SUCCESS:
            break;

        case SHARE_FAIL:
            break;

        case LONGPOLL:
            break;

        default:
            break;
    }

    ui->list->addItem(message);
    ui->list->scrollToBottom();
}

void MiningPage::resetMiningButton()
{
    ui->startButton->setText(minerActive ? "Stop Mining" : "Start Mining");
}
