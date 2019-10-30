#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QStandardPaths>
#include "paintwidget.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_fontChooser_clicked()
{
    m_fontLocation = QFileDialog::getOpenFileName(
                this, ("Choose Font"),
                QStandardPaths::standardLocations(QStandardPaths::FontsLocation)[1],
                ("Fonts (*.ttf *.otf)"));

    if(!m_fontLocation.isEmpty())
        ui->fontChooser->setText(QFileInfo(m_fontLocation).fileName());

    m_layoutEngine.setFont(m_fontLocation);
}

void MainWindow::on_updateText_clicked()
{
    QString text = ui->textbox->text();

    if(text.isEmpty() || m_fontLocation.isEmpty())
        return;

    m_layoutEngine.setText(text);
    PaintWidget *p = qobject_cast<PaintWidget *>(ui->canvas);
    p->setProperties(m_layoutEngine.calculate());
    p->update();
}
