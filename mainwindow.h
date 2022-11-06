#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <filesystem>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QPixmap>
#include <QGridLayout>
#include <sstream>
#include "previewwidget.h"
#include <fstream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    std::vector<QLabel *> labellist;
    std::string config="config.json";
    QJsonObject topsettings;
    QWidget *labelgrid;
    QGridLayout *labelgridlayout;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_previewWidget_checked(bool check);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
