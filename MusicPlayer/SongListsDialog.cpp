#include "SongListsDialog.h"
#include "ui_SongListsDialog.h"

#include <QPushButton>
#include <QFileDialog>
#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QUrl>

SongListsDialog::SongListsDialog(Library& libraryRef, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::SongListsDialog)
    , library(libraryRef)
{
    ui->setupUi(this);
    setWindowTitle(QStringLiteral(u"\uC804\uCCB4 \uBAA9\uB85D")); // 전체 목록 (한글 오류)

    audioOutput = new QAudioOutput(this); // 소리를 내는 장치를 제어하는 클래스 ( 볼륨, 출력 장치, 상태 등을 관리 가능)
    player = new QMediaPlayer(this); // 오디오를 재생하는 클래스, 소리 출력을 위해 QAudioOutput 객체와 연결이 필요함.
    player->setAudioOutput(audioOutput); // QAudioOutput 객체를 QMediaPlayer 객체와 연결. player 재생하면 audioOutput 통해 소리가 출력됨.

    // connect() 함수는 사용자가 특정 동작을 했을 때 호출되는 시그널과 슬롯을 연결해주는 Qt에서 제공하는 함수.
    // 
    // 재생 버튼 클릭 시
    connect(ui->btnPlay, &QPushButton::clicked, // &QPushButton::clicked(사용자의 특정 동작 시그널)
        this, &SongListsDialog::onPlayClicked); // &SongListsDialog::onPlayClicked(사용자의 동작에 반응하는 슬롯)
    // btnPlay 버튼을 클릭하면 $PushButton::clicked 알림이 발생하고, $SongListsDialog::onPlayClicked 슬롯이 실행된다.

    // 일시정지 버튼 클릭 시
    connect(ui->btnPause, &QPushButton::clicked, // $QPushButton::clicked(사용자의 특정 동작 시그널)
        this, &SongListsDialog::onPauseClicked); // &SongListsDialog::onPauseClicked(사용자의 동작에 반응하는 슬롯)
    // btnPause 버튼을 클릭하면 $QPushButton::clicked 알림이 발생하고, $SongListsDialog::onPauseClicked 슬롯이 실행된다.

    // 재생 길이 변경 시
    connect(player, &QMediaPlayer::durationChanged, // &QMediaPlayer::durationChanged(재생하는 노래의 길이를 파악하는 시그널)
        this, &SongListsDialog::onDurationChanged); // $SongListsDialog::onDurationChanged(파악한 노래 길이로 재생바의 길이를 변경하는 슬롯)
    // 노래를 재생하거나 바꾸면 $QMediaPlayer::durationChanged 시그널이 신호를 주고, $SongListsDialog::onDurationChanged 슬롯이 실행된다.

    // 노래가 재생될 때 노래의 위치 변경 시
    connect(player, &QMediaPlayer::positionChanged, // &QMediaPlayer::positionChanged(현재 재생되는 노래의 위치를 파악하는 시그널)
        this, &SongListsDialog::onPositionChanged); // &SongListsDialog::onPositionChanged(시그널의 정보를 받고 재생바의 위치를 바꾸는 슬롯)
    // 노래가 재생될 때 노래의 위치를 계속 파악하여 $QMediaPlayer::positionChanged 시그널이 신호를 주고, $SongListsDialog::onPositionChanged 슬롯이 재생바의 위치를 바꿔준다.

    // 슬라이더를 사용자가 움직였을 때
    connect(ui->sliderPosition, &QSlider::sliderMoved, // &QSlider::sliderMoved(사용자가 재생바를 움직일 때 실시간으로 바뀌는 슬라이더의 위치를 알려주는 시그널)
        this, &SongListsDialog::onSliderMoved); // &SongListsDialog::onSliderMoved(시그널의 정보를 받고 노래 재생 위치를 바꾸는 슬롯 )
    // 사용자가 재생바를 움직이면 $QSlider::sliderMoved 시그널이 신호를 주고, $SongListsDialog::onSliderMoved 슬롯이 노래 재생 위치를 바꿔준다.

    connect(ui->listWidgetSongs, &QListWidget::itemClicked, // &QListWidget::itemClicked(재생 목록에서 사용자가 노래를 선택했을 때 발생하는 시그널)
        this, &SongListsDialog::onSongSelected); // &SongListsDialog::onSongSelected(시그널의 정보를 받고 선택된 노래로 재생하는 슬롯)
    // 재생 목록에서 사용자가 노래를 선택하면 $QListWidget::itemClicked 시그널이 신호를 주고, $SongListsDialog::onSongSelected 슬롯이 선택된 노래로 재생한다.

    connect(ui->btnClose, &QPushButton::clicked, this, &SongListsDialog::onCloseClicked);

    connect(ui->btnAddSong, &QPushButton::clicked, this, &SongListsDialog::onAddSongClicked);

    refreshList();
}

SongListsDialog::~SongListsDialog()
{
    delete ui;
}

void SongListsDialog::onPlayClicked()
{
    if (currentFilePath.isEmpty()) {
        QMessageBox::warning(this,
            QStringLiteral(u"\uC624\uB958"),                 // "오류"
            QStringLiteral(u"\uBA3C\uC800 \uB178\uB798\uB97C \uC120\uD0DD\uD574\uC8FC\uC138\uC694.")); // "먼저 노래를 선택해주세요."
        return;
    }
    player->play();
}

void SongListsDialog::onPauseClicked()
{
    if (currentFilePath.isEmpty()) {
        QMessageBox::warning(this,
            QStringLiteral(u"\uC624\uB958"),
            QStringLiteral(u"\uBA3C\uC800 \uB178\uB798\uB97C \uC120\uD0DD\uD574\uC8FC\uC138\uC694."));
        return;
    }
    player->pause();
}

void SongListsDialog::onDurationChanged(qint64 duration)
{
    ui->sliderPosition->setMaximum(static_cast<int>(duration));
}

void SongListsDialog::onPositionChanged(qint64 position)
{
    ui->sliderPosition->setValue(static_cast<int>(position));
}

void SongListsDialog::onSliderMoved(int position)
{
    player->setPosition(position);
}

void SongListsDialog::onSongSelected()
{
    int index = ui->listWidgetSongs->currentRow();
    if (index < 0) return;

    const auto& songs = library.getSongs();
    if (index >= static_cast<int>(songs.size())) return;

    currentFilePath = QString::fromStdString(songs[index].S_filePath);
    player->setSource(QUrl::fromLocalFile(currentFilePath));

    // 선택하면 바로 재생하고 싶으면 아래 줄 켜면 됨
    // player->play();
}

void SongListsDialog::refreshList()
{
    ui->listWidgetSongs->clear();
    const auto& songs = library.getSongs();

    for (const auto& s : songs)
    {
        ui->listWidgetSongs->addItem(
            QString::fromStdString(s.S_title) + " - " +
            QString::fromStdString(s.S_singer)
        );
    }
}


void SongListsDialog::onAddSongClicked()
{
    bool ok;

    QString title = QInputDialog::getText(this, "Add Title", "Title", QLineEdit::Normal, "", &ok);
    if (!ok || title.isEmpty()) return;

    QString singer = QInputDialog::getText(this, "Add Singer", "Singer", QLineEdit::Normal, "", &ok);
    if (!ok || singer.isEmpty()) return;

    QString filePath = QFileDialog::getOpenFileName(this, "choose file path", "", "Audio Files (*.mp3 *.wav *.ogg)");
    if (filePath.isEmpty()) return;

    library.addSong(
        title.toStdString(),
        singer.toStdString(),
        filePath.toStdString());

    refreshList();

    // 방금 추가한 곡을 현재 곡으로 세팅하고 싶으면
    currentFilePath = filePath;
    player->setSource(QUrl::fromLocalFile(currentFilePath));
}

void SongListsDialog::onCloseClicked()
{
    accept();
}