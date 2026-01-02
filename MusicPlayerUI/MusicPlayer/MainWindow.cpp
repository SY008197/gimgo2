//#include "MainWindow.h"

//MainWindow::MainWindow(QWidget *parent)
//    : QMainWindow(parent)
//{
//    ui.setupUi(this);
//}

//MainWindow::~MainWindow()
//{}




#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // 오디오 출력 & 플레이어 생성
    audioOutput = new QAudioOutput(this);
    player = new QMediaPlayer(this);
    player->setAudioOutput(audioOutput);

    // 음악 파일 지정 (테스트용)
    player->setSource(QUrl::fromLocalFile(
        "C:\Users\jaemy\Downloads\test.mp3"   // ⚠️ 본인 PC 경로로 수정
    ));

    // 버튼 클릭 시그널 연결
    connect(ui->btnPlay, &QPushButton::clicked,
        this, &MainWindow::onPlayClicked);

    connect(ui->btnPause, &QPushButton::clicked,
        this, &MainWindow::onPauseClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPlayClicked()
{
    player->play();
}

void MainWindow::onPauseClicked()
{
    player->pause();
}
