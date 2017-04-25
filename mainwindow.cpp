#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <ctime>
#include <QtGlobal>
#include <QMenu>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    loadFiles();
    ui->setupUi(this);
    resize(527, 60);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_Generate_clicked()
{
    qsrand(time(NULL));
    std::vector<QString> speedMaps, jumpMaps, generalMaps;
    std::vector<int> ratingRange = getDifficultyRating(ui->horizontalSlider_StarRating->value());
    int index = 0, speedFound = 0, jumpFound = 0, generalFound = 0, tries = 0;
    int numOfMaps = ui->spinBox_NumOfMaps->value();
    bool speed = ui->checkBox_Speed->isChecked();
    bool jump = ui->checkBox_Jump->isChecked();
    bool general = ui->checkBox_General->isChecked();
    do {
            index = qrand() % overallRating.size();
            if (overallRating[index] >= ratingRange[0] && overallRating[index] <= ratingRange[1]) {
                if (speed && speedRating[index] > overallRating[index] * 0.6 && speedFound < numOfMaps) {
                    if (std::find(speedMaps.begin(), speedMaps.end(), mapNames[index]) == speedMaps.end()) {
                        if (!searchList(blacklist, mapNames[index])) {
                            if (!searchList(greylist, mapNames[index])) {
                                speedMaps.push_back(mapNames[index]);
                                greylist.push_back(mapNames[index]);
                                speedFound++;
                            }
                        }
                    }
                }
                else if (jump && jumpRating[index] > overallRating[index] * 0.5 && jumpFound < numOfMaps) {
                    if (std::find(jumpMaps.begin(), jumpMaps.end(), mapNames[index]) == jumpMaps.end()) {
                        if (!searchList(blacklist, mapNames[index])) {
                            if (!searchList(greylist, mapNames[index])) {
                                jumpMaps.push_back(mapNames[index]);
                                greylist.push_back(mapNames[index]);
                                jumpFound++;
                            }
                        }
                    }
                }
                else if (general && jumpRating[index] < overallRating[index] * 0.55 && speedRating[index] < overallRating[index] * 0.55 && generalFound < numOfMaps) {
                    if (std::find(generalMaps.begin(), generalMaps.end(), mapNames[index]) == generalMaps.end()) {
                        if (!searchList(blacklist, mapNames[index])) {
                            if (!searchList(greylist, mapNames[index])) {
                                generalMaps.push_back(mapNames[index]);
                                greylist.push_back(mapNames[index]);
                                generalFound++;
                            }
                        }
                    }
                }
            }
            if (++tries > 10000) break;
        } while ((speed && speedFound < numOfMaps) || (jump && jumpFound < numOfMaps) || (general && generalFound < numOfMaps));
    QString content;
    int frameHeight = 70;
    ui->frame_Speed->hide();
    ui->frame_Jump->hide();
    ui->frame_General->hide();
    if (speed) {
        ui->listWidget_Speed->clear();
        for (int i = 0; i < speedMaps.size(); i++) {
            ui->listWidget_Speed->addItem(speedMaps[i]);
        }
        ui->frame_Speed->show();
        ui->frame_Speed->setGeometry(10, frameHeight, 551, 141);
        frameHeight += 140;
    }
    if (jump) {
        ui->listWidget_Jump->clear();
        for (int i = 0; i < jumpMaps.size(); i++) {
            ui->listWidget_Jump->addItem(jumpMaps[i]);
        }
        ui->frame_Jump->show();
        ui->frame_Jump->setGeometry(10, frameHeight, 551, 141);
        frameHeight += 140;
    }
    if (general) {
        ui->listWidget_General->clear();
        for (int i = 0; i < generalMaps.size(); i++) {
           ui->listWidget_General->addItem(generalMaps[i]);
        }
        ui->frame_General->show();
        ui->frame_General->setGeometry(10, frameHeight, 551, 141);
    }
    if (speed && jump && general) resize(527, 500);
    else if ((speed && jump)|| (speed && general) || (jump && general)) resize(527, 360);
    else if (speed || jump || general) resize(527, 220);
    else resize(527, 60);
}

std::vector<int> MainWindow::getDifficultyRating(int n) {
    std::vector<int> range;
    switch (n) {
        case 4:
            range.push_back(70);
            range.push_back(102);
            break;
        case 5:
            range.push_back(110);
            range.push_back(154);
            break;
        case 6:
            range.push_back(160);
            range.push_back(212);
            break;
        case 7:
            range.push_back(210);
            range.push_back(270);
            break;
        case 8:
            range.push_back(300);
            range.push_back(380);
            break;
        }
    return range;
}

bool MainWindow::searchList(std::vector<QString> list, QString str){
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == str)
            return true;
    }
    return false;
}

int MainWindow::getIndex(std::vector<QString> list, QString str){
    for (int i = 0; i < list.size(); i++) {
        if (list[i] == str)
            return i;
    }
    return 0;
}

void MainWindow::loadFiles() {
    QFile inFile("Difficulties.map");
    inFile.open(QIODevice::ReadOnly);
    QTextStream stream(&inFile);
    QString content;
    if (inFile.isOpen()) {
        content = stream.readLine();
        while (content != NULL) {
            mapNames.push_back(content);
            content = stream.readLine();
            clipboardNames.push_back(content);
            content = stream.readLine();
            speedRating.push_back(content.toDouble());
            content = stream.readLine();
            jumpRating.push_back(content.toDouble());
            content = stream.readLine();
            overallRating.push_back(content.toDouble());
            content = stream.readLine();
        }
        inFile.close();
    }

    inFile.setFileName("Blacklist.map");
    inFile.open(QIODevice::ReadOnly);
    if (inFile.isOpen()) {
        content = stream.readLine();
        while (content != NULL) {
            blacklist.push_back(content);
            content = stream.readLine();
        }
        inFile.close();
    }
}

void MainWindow::on_listWidget_Speed_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextMenu(tr("Context Menu"), this);
    QAction blacklistItem("Blacklist", this);
    connect(&blacklistItem, SIGNAL(triggered()), this, SLOT(listWidget_Speed_blacklistItem()));
    contextMenu.addAction(&blacklistItem);
    QAction copyToClipboard("Copy to clipboard", this);
    connect(&copyToClipboard, SIGNAL(triggered()), this, SLOT(listWidget_Speed_copyToClipboard()));
    contextMenu.addAction(&copyToClipboard);
    contextMenu.exec(QCursor::pos());
}

void MainWindow::listWidget_Speed_blacklistItem() {
    if (!searchList(blacklist, ui->listWidget_Speed->currentItem()->text())) {
        blacklist.push_back(ui->listWidget_Speed->currentItem()->text());
    }
    QFile outFile("Blacklist.map");
    outFile.open(QIODevice::WriteOnly);
    if (outFile.isOpen()) {
        QString content;
        for (int i = 0; i < blacklist.size(); i++)
            content += blacklist[i] + "\n";
        QTextStream stream(&outFile);
        stream << content;
    }
}

void MainWindow::listWidget_Speed_copyToClipboard() {
    int index = getIndex(mapNames, ui->listWidget_Speed->currentItem()->text());
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(clipboardNames[index], QClipboard::Clipboard);
}

void MainWindow::on_listWidget_Jump_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextMenu(tr("Context Menu"), this);
    QAction blacklistItem("Blacklist", this);
    connect(&blacklistItem, SIGNAL(triggered()), this, SLOT(listWidget_Jump_blacklistItem()));
    contextMenu.addAction(&blacklistItem);
    QAction copyToClipboard("Copy to clipboard", this);
    connect(&copyToClipboard, SIGNAL(triggered()), this, SLOT(listWidget_Jump_copyToClipboard()));
    contextMenu.addAction(&copyToClipboard);
    contextMenu.exec(QCursor::pos());
}

void MainWindow::listWidget_Jump_blacklistItem() {
    if (!searchList(blacklist, ui->listWidget_Jump->currentItem()->text())) {
        blacklist.push_back(ui->listWidget_Jump->currentItem()->text());
    }
    QFile outFile("Blacklist.map");
    outFile.open(QIODevice::WriteOnly);
    if (outFile.isOpen()) {
        QString content;
        for (int i = 0; i < blacklist.size(); i++)
            content += blacklist[i] + "\n";
        QTextStream stream(&outFile);
        stream << content;
    }
}

void MainWindow::listWidget_Jump_copyToClipboard() {
    int index = getIndex(mapNames, ui->listWidget_Jump->currentItem()->text());
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(clipboardNames[index], QClipboard::Clipboard);
}

void MainWindow::on_listWidget_General_customContextMenuRequested(const QPoint &pos)
{
    QMenu contextMenu(tr("Context Menu"), this);
    QAction blacklistItem("Blacklist", this);
    connect(&blacklistItem, SIGNAL(triggered()), this, SLOT(listWidget_General_blacklistItem()));
    contextMenu.addAction(&blacklistItem);
    QAction copyToClipboard("Copy to clipboard", this);
    connect(&copyToClipboard, SIGNAL(triggered()), this, SLOT(listWidget_General_copyToClipboard()));
    contextMenu.addAction(&copyToClipboard);
    contextMenu.exec(QCursor::pos());
}

void MainWindow::listWidget_General_blacklistItem() {
    if (!searchList(blacklist, ui->listWidget_General->currentItem()->text())) {
        blacklist.push_back(ui->listWidget_General->currentItem()->text());
    }
    QFile outFile("Blacklist.map");
    outFile.open(QIODevice::WriteOnly);
    if (outFile.isOpen()) {
        QString content;
        for (int i = 0; i < blacklist.size(); i++)
            content += blacklist[i] + "\n";
        QTextStream stream(&outFile);
        stream << content;
    }
}

void MainWindow::listWidget_General_copyToClipboard() {
    int index = getIndex(mapNames, ui->listWidget_General->currentItem()->text());
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(clipboardNames[index], QClipboard::Clipboard);
}
