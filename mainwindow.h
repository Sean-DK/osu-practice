#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    std::vector<QString> mapNames, clipboardNames, blacklist, greylist;
    std::vector<double> speedRating, jumpRating, overallRating;

    ~MainWindow();

private slots:
    void on_pushButton_Generate_clicked();

    void on_listWidget_Speed_customContextMenuRequested(const QPoint &pos);

    void listWidget_Speed_blacklistItem();

    void listWidget_Speed_copyToClipboard();

    void on_listWidget_Jump_customContextMenuRequested(const QPoint &pos);

    void listWidget_Jump_blacklistItem();

    void listWidget_Jump_copyToClipboard();

    void on_listWidget_General_customContextMenuRequested(const QPoint &pos);

    void listWidget_General_blacklistItem();

    void listWidget_General_copyToClipboard();

private:
    Ui::MainWindow *ui;
    std::vector<int> getDifficultyRating(int);
    bool searchList(std::vector<QString>, QString);
    int getIndex(std::vector<QString>, QString);
    void loadFiles();
};

#endif // MAINWINDOW_H
