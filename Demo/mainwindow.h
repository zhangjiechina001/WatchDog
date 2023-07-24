#pragma execution_character_set("utf-8")
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSharedMemory>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void CheckSharedMem();

    void on_checkBox_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QTimer *_timer;
    static QSharedMemory m_sharedMem;
};

#endif // MAINWINDOW_H
