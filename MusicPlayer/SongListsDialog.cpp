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
 

    // 재생 버튼 클릭 시
    // btnPlay 버튼을 클릭하면 $PushButton::clicked 알림이 발생하고, $SongListsDialog::onPlayClicked 슬롯이 실행된다.
    connect(ui->btnPlay, &QPushButton::clicked, // &QPushButton::clicked(사용자의 특정 동작 시그널)
        this, &SongListsDialog::onPlayClicked); // &SongListsDialog::onPlayClicked(사용자의 동작에 반응하는 슬롯)


    // 일시정지 버튼 클릭 시
    // btnPause 버튼을 클릭하면 $QPushButton::clicked 알림이 발생하고, $SongListsDialog::onPauseClicked 슬롯이 실행된다.
    connect(ui->btnPause, &QPushButton::clicked, // $QPushButton::clicked(사용자의 특정 동작 시그널)
        this, &SongListsDialog::onPauseClicked); // &SongListsDialog::onPauseClicked(사용자의 동작에 반응하는 슬롯)
    

    // 재생 길이 변경 시
    // 노래를 재생하거나 바꾸면 $QMediaPlayer::durationChanged 시그널이 신호를 주고, $SongListsDialog::onDurationChanged 슬롯이 실행된다.
    connect(player, &QMediaPlayer::durationChanged, // &QMediaPlayer::durationChanged(재생하는 노래의 길이를 파악하는 시그널)
        this, &SongListsDialog::onDurationChanged); // $SongListsDialog::onDurationChanged(파악한 노래 길이로 재생바의 길이를 변경하는 슬롯)


    // 노래가 재생될 때 노래의 위치 변경 시
    // 노래가 재생될 때 노래의 위치를 계속 파악하여 $QMediaPlayer::positionChanged 시그널이 신호를 주고, $SongListsDialog::onPositionChanged 슬롯이 재생바의 위치를 바꿔준다.
    connect(player, &QMediaPlayer::positionChanged, // &QMediaPlayer::positionChanged(현재 재생되는 노래의 위치를 파악하는 시그널)
        this, &SongListsDialog::onPositionChanged); // &SongListsDialog::onPositionChanged(시그널의 정보를 받고 재생바의 위치를 바꾸는 슬롯)


    // 슬라이더를 사용자가 움직였을 시
    // 사용자가 재생바를 움직이면 $QSlider::sliderMoved 시그널이 신호를 주고, $SongListsDialog::onSliderMoved 슬롯이 노래 재생 위치를 바꿔준다.
    connect(ui->sliderPosition, &QSlider::sliderMoved, // &QSlider::sliderMoved(사용자가 재생바를 움직일 때 실시간으로 바뀌는 슬라이더의 위치를 알려주는 시그널)
        this, &SongListsDialog::onSliderMoved); // &SongListsDialog::onSliderMoved(시그널의 정보를 받고 노래 재생 위치를 바꾸는 슬롯 )


    // 노래를 사용자가 직접 선택 했을 때
    // 재생 목록에서 사용자가 노래를 선택하면 $QListWidget::itemClicked 시그널이 신호를 주고, $MainWindow::onSongSelected 슬롯이 선택된 노래로 재생한다.
    connect(ui->listWidgetSongs, &QListWidget::itemClicked, // &QListWidget::itemClicked(재생 목록에서 사용자가 노래를 선택했을 때 발생하는 시그널)
        this, &SongListsDialog::onSongSelected); // &SongListsDialog::onSongSelected(시그널의 정보를 받고 선택된 노래로 재생하는 슬롯)


    // 닫기 버튼을 눌렀을 시
    // 사용자가 닫기 버튼을 누르면 &QPushButton::clicked 시그널이 신호를 주고, &SongListsDialog::onCloseClicked 슬롯이 윈도우를 닫는다.
    connect(ui->btnClose, &QPushButton::clicked, // &QPushButton::clicked(닫기 버튼을 눌렀을 때 발생하는 시그널)
        this, &SongListsDialog::onCloseClicked); // &SongListsDialog::onCloseClicked( 시그널의 정보를 받고 윈도우를 닫는 슬롯)


    // 노래 추가 버튼을 눌렀을 시
    // 사용자가 노래 추가 버튼을 누르면 &QPushButton::clicked 시그널이 신호를 주고, &SongListsDialog::onAddSongClicked 슬롯이 라이브러리에 노래 정보를 추가 한다.
    connect(ui->btnAddSong, &QPushButton::clicked, // &QPushButton::clicked( 노래 추가 버튼을 눌렀을 때 발생하는 시그널)
        this, &SongListsDialog::onAddSongClicked); // &SongListsDialog::onAddSongClicked( 시그널의 정보를 받고 노래 정보를 입력받고 라이브러리에 추가하는 슬롯 함수 )
    

    refreshList();
}

SongListsDialog::~SongListsDialog()
{
    delete ui;
}

// 재생 버튼 함수
void SongListsDialog::onPlayClicked() // 사용자의 신호를 시그널로 받고 실행되는 슬롯 함수
{
    if (currentFilePath.isEmpty()) {
        QMessageBox::warning(this,
            QStringLiteral(u"\uC624\uB958"),                 // "오류"
            QStringLiteral(u"\uBA3C\uC800 \uB178\uB798\uB97C \uC120\uD0DD\uD574\uC8FC\uC138\uC694.")); // "먼저 노래를 선택해주세요."
        return;
    }
    player->play(); // play()함수는 QMediaPlayer 클래스의 함수로 음악 재생을 시작함.
}

// 일시 정지 함수
void SongListsDialog::onPauseClicked() // 사용자의 신호를 시그널로 받고 실행되는 슬롯 함수
                                       // pause()함수는 QMediaPlayer 클래스의 함수로 음악 재생을 일시정지함.
{
    if (currentFilePath.isEmpty()) { // 만약 노래가 추가되지 않았다면 띄우는 메시지
        QMessageBox::warning(this,
            QStringLiteral(u"\uC624\uB958"),                 // "오류"
            QStringLiteral(u"\uBA3C\uC800 \uB178\uB798\uB97C \uC120\uD0DD\uD574\uC8FC\uC138\uC694.")); // "먼저 노래를 선택해주세요."
        return;
    }
    player->pause();
}

// 재생바 길이 값 설정 함수
void SongListsDialog::onDurationChanged(qint64 duration) // 재생 길이 변경 시그널을 받고 실행되는 슬롯 함수
                                                         // setMaximum(static_cast<int>())은 재생바의 최대값을 설정함.
{
    ui->sliderPosition->setMaximum(static_cast<int>(duration));
}

// 재생바 이동 기능 함수
void SongListsDialog::onPositionChanged(qint64 position) // 노래 재생 위치 변경 시그널을 받고 실행되는 슬롯 함수
                                                         // setValue(static_cast<int>())는 재생바의 현재 위치를 설정함.
{
    ui->sliderPosition->setValue(static_cast<int>(position));
}

// 재생바 움직일 때 노래 재생 위치 변경하는 함수
void SongListsDialog::onSliderMoved(int position) // 사용자가 재생바를 움직였을 때 시그널을 받고 실행되는 슬롯 함수
                                                  // setPosition()는 노래 재생 위치를 설정함.
{
    player->setPosition(position);
}

// 사용자가 노래를 선택했을 때 노래의 행 번호를 가져오는 함수
void SongListsDialog::onSongSelected()
{
    int index = ui->listWidgetSongs->currentRow(); //index에서 사용자가 선택한 노래의 행 번호를 가져옴 (0부터 시작)
    if (index < 0) return;

    const auto& songs = library.getAllSongs(); // Library 클래스의 getSongs() 멤버 함수를 호출하여 노래 목록을 가져옴
	// private 에 넣으면 접근 불가. 그래서 public 으로 바꿈.
    if (index >= static_cast<int>(songs.size())) return; // 사용자가 index 범위를 벗어난 노래를 선택 못하게 함

    currentFilePath = QString::fromStdString(songs[index].filePath); //ㅡㅡㅡㅡㅡ songview 클래스에 s.filePath 없음
    // 사용자가 선택한 노래 파일 경로를 Qt에서 사용하기위해 변환하는 과정
    player->setSource(QUrl::fromLocalFile(currentFilePath)); 
    // 로컬 파일에서 노래를 재생하기 위해 QMediaPlayer 객체에 노래를 넘겨줌



    // 선택하면 바로 재생하고 싶으면 아래 줄 켜면 됨
    // player->play();
}

// 라이브러리에 들어있는 노래들을 리스트에 띄우는 함수
void SongListsDialog::refreshList() // 노래를 추가하고 메인화면으로 나갈 때 기존에 추가한 노래 삭제 - >
                                    // 다시 재생 목록 들어갔을 때 라이브러리를 순회 하며 이미 추가한 노래는 바로 리스트에 띄우기
{
    ui->listWidgetSongs->clear();                            // 기존 화면 목록 전부 삭제
    const auto& songs = library.getAllSongs();                  // 라이브러리의 노래 목록 가져오기

    for (const auto& s : songs)                              // 모든 노래 순회
    {
        ui->listWidgetSongs->addItem(                        // 화면에 한 줄씩 추가
            QString::fromStdString(s.title) + " - " + // S.title 에서 s.title로 변경
            QString::fromStdString(s.singer)
        );
    }
}

// 사용자에게 노래 정보를 입력 받고 라이브러리에 추가하는 함수
void SongListsDialog::onAddSongClicked()
{
    bool ok;

    // 1? 노래 제목 입력
    QString title = QInputDialog::getText(this, "Add Title", "Title", QLineEdit::Normal, "", &ok);
    if (!ok || title.isEmpty()) return;

    // 2? 가수 입력
    QString singer = QInputDialog::getText(this, "Add Singer", "Singer", QLineEdit::Normal, "", &ok);
    if (!ok || singer.isEmpty()) return;

    // 3? 파일 경로 선택
    QString filePath = QFileDialog::getOpenFileName(this, "choose file path", "", "Audio Files (*.mp3 *.wav *.ogg)");
    if (filePath.isEmpty()) return;

    library.addSong(                    // 사용자가 정보를 입력하면 라이브러리에 곡 추가
        title.toStdString(),
        singer.toStdString(),
        filePath.toStdString());

    refreshList();

    // 방금 추가한 곡을 현재 곡으로 세팅하고 싶으면
    currentFilePath = filePath;
    player->setSource(QUrl::fromLocalFile(currentFilePath));
}

// 닫기 버튼 기능 함수
void SongListsDialog::onCloseClicked()
{
    accept();
}