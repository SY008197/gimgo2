#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MusicPlayer.h"

class MusicPlayer : public QMainWindow
{
    Q_OBJECT

public:
    MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();

private:
    Ui::MusicPlayerClass ui;
};

