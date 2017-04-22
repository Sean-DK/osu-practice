#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <vector>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <ctime>
#include <QtGlobal>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(527, 60);
    loadFiles();
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
                        speedMaps.push_back(mapNames[index]);
                        speedFound++;
                    }
                }
                else if (jump && jumpRating[index] > overallRating[index] * 0.5 && jumpFound < numOfMaps) {
                    if (std::find(jumpMaps.begin(), jumpMaps.end(), mapNames[index]) == jumpMaps.end()) {
                        jumpMaps.push_back(mapNames[index]);
                        jumpFound++;
                    }
                }
                else if (general && generalFound < numOfMaps) {
                    if (std::find(generalMaps.begin(), generalMaps.end(), mapNames[index]) == generalMaps.end()) {
                        generalMaps.push_back(mapNames[index]);
                        generalFound++;
                    }
                }
            }
            if (++tries > 10000) break;
        } while ((speed && speedFound < numOfMaps) || (jump && jumpFound < numOfMaps) || (general && generalFound < numOfMaps));
    QString content;
    if (speed) {
        content += "\nSpeed:\n\n";
        for (int i = 0; i < speedMaps.size(); i++) {
            content += speedMaps[i] + "\n";
        }
    }
    if (jump) {
        content += "\nJump:\n\n";
        for (int i = 0; i < jumpMaps.size(); i++) {
            content += jumpMaps[i] + "\n";
        }
    }
    if (general) {
        content += "\nGeneral:\n\n";
        for (int i = 0; i < generalMaps.size(); i++) {
            content += generalMaps[i] + "\n";
        }
    }
    ui->textBrowser_Output->setText(content);
    if (speed || jump || general) {
        resize(527, 500);
    }
    else {
        resize(527, 60);
    }
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

void MainWindow::loadFiles() {
    QFile inFile("E:\\Users\\Sean\\Documents\\Difficulties.txt");
    inFile.open(QIODevice::ReadOnly);
    QTextStream stream(&inFile);
    QString content;
    if (inFile.isOpen()) {
        content = stream.readLine();
        while (content != NULL) {
            mapNames.push_back(content);
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
}
