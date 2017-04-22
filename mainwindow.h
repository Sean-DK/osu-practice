#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    std::vector<QString> mapNames;
    std::vector<double> speedRating, jumpRating, overallRating;

    ~MainWindow();

private slots:
    void on_pushButton_Generate_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<int> getDifficultyRating(int n);
    void loadFiles();
};

#endif // MAINWINDOW_H
