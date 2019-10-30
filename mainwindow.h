#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include "hbwrapper.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_updateText_clicked();
    void on_fontChooser_clicked();

private:
    Ui::MainWindow *ui;
    HBWrapper m_layoutEngine;
    QString m_fontLocation;
};

#endif // MAINWINDOW_H
