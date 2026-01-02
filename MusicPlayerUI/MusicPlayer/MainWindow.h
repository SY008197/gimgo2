//#pragma once

//#include <QtWidgets/QMainWindow>
//#include "ui_MainWindow.h"

//class MainWindow : public QMainWindow
//{
//    Q_OBJECT
//
//public:
//    MainWindow(QWidget *parent = nullptr);
//    ~MainWindow();
//
//private:
//    Ui::MainWindowClass ui;
//};



#pragma once

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onPlayClicked();
    void onPauseClicked();

private:
    Ui::MainWindow* ui;

    QMediaPlayer* player;
    QAudioOutput* audioOutput;
};
