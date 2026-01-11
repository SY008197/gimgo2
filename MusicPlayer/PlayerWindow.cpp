#include "PlayerWindow.h"
#include "ui_PlayerWindow.h"

#include <QFileInfo>
#include <QUrl>
#include <QStyle>
#include <QDebug>
#include <QDir>


PlayerWindow::PlayerWindow(const std::vector<SongView>& allSongs, int startIndex, QWidget* parent) // 재생창을 만들 때 호출되는 생성자

    : QMainWindow(parent) // PlayerWindow는 QMainWindow를 상속하므로, 부모 클래스 생성자 호출
    , ui(new Ui::PlayerWindow) // Designer가 만든 UI 클래스(Ui::PlayerWindow) 객체를 동적 생성 ( 이 ui가 버튼/라벨/슬라이더 같은 위젯 포인터들을 들고 있음. )
    , playlistAll(allSongs) // 멤버 변수 playlistAll에 allSongs를 복사해서 저장
    , currentIndex(startIndex) // 현재 재생 인덱스 멤버 currentIndex를 시작 인덱스로 초기화
{
    ui->setupUi(this); // .ui 내용을 실제 위젯으로 생성해서 this(PlayerWindow)에 붙여줌

    // 오디오 구성
    audioOutput = new QAudioOutput(this); // 오디오 출력 장치/볼륨을 담당하는 QAudioOutput 생성.
    player = new QMediaPlayer(this); // 실제로 음악을 재생하는 QMediaPlayer 생성.
    player->setAudioOutput(audioOutput); // player가 소리를 낼 때 사용할 출력 장치를 audioOutput으로 지정.


    ui->btnBack->setIcon(QIcon(":/icons/icons/back.svg")); // 뒤로 버튼에 Qt 기본 제공 아이콘을 설정
    ui->btnPrev->setIcon(QIcon(":/icons/icons/prev.svg")); // 이전 곡 버튼 아이콘을 이전 트랙 기본 아이콘으로 설정
    ui->btnNext->setIcon(QIcon(":/icons/icons/next.svg")); // 다음 곡 버튼 아이콘을 다음 트랙 기본 아이콘으로 설정
    ui->btnPlayPause->setIcon(QIcon(":/icons/icons/play.svg")); // 재생/일시정지 버튼의 초기 아이콘을 재생으로 설정

    // 아이콘 버튼(기본은 Qt 내장 아이콘 사용 리소스 없어도 동작)
    /*ui->btnBack->setIcon(style()->standardIcon(QStyle::SP_ArrowBack)); // 뒤로 버튼에 Qt 기본 제공 아이콘을 설정
    ui->btnPrev->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward)); // 이전 곡 버튼 아이콘을 이전 트랙 기본 아이콘으로 설정
    ui->btnNext->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward)); // 다음 곡 버튼 아이콘을 다음 트랙 기본 아이콘으로 설정
    ui->btnPlayPause->setIcon(style()->standardIcon(QStyle::SP_MediaPlay)); // 재생/일시정지 버튼의 초기 아이콘을 재생으로 설정*/

    const QSize iconSz(36, 36); // 아이콘 크기를 36x36으로 쓰기 위한 QSize 생성
    // 각 버튼에 표시될 아이콘 크기 지정
    ui->btnBack->setIconSize(iconSz);
    ui->btnPrev->setIconSize(iconSz);
    ui->btnPlayPause->setIconSize(QSize(44, 44)); // 재생버튼만 44x44로 크게 해서 강조
    ui->btnNext->setIconSize(iconSz);

    // 버튼 연결
    
    // 사용자가 뒤로가기 버튼을 눌렀을 시
    // 사용자가 뒤로가기 버튼을 누르면 &QToolButton::clicked 시그널이 신호를 주고, &PlayerWindow::onBackClicked 슬롯이 창을 닫는다.
    connect(ui->btnBack, &QToolButton::clicked, // &QToolButton::clicked(사용자가 뒤로가기 버튼을 눌렀을 때 발생하는 시그널)
        this, &PlayerWindow::onBackClicked); // &PlayerWindow::onBackClicked(시그널의 정보를 받고 뒤로 가는 슬롯)

    // 사용자가 이전 트랙 버튼을 눌렀을 시
    // 사용자가 이전 트랙 버튼을 누르면 &QToolButton::clicked 시그널이 신호를 주고, &PlayerWindow::onPrevClicked 슬롯이 이전 노래를 재생시킨다.
    connect(ui->btnPrev, &QToolButton::clicked, // &QToolButton::clicked(사용자가 이전트랙 버튼을 눌렀을 때 발생하는 시그널)
        this, &PlayerWindow::onPrevClicked); // &PlayerWindow::onPrevClicked(시그널의 정보를 받고 이전 트랙의 노래를 재생하는 슬롯)

    // 사용자가 다음 트랙 버튼을 눌렀을 시
    // 사용자가 다음 트랙 버튼을 누르면 &QToolButton::clicked 시그널이 신호를 주고, &PlayerWindow::onNextClicked 슬롯이 다음 노래를 재생시킨다.
    connect(ui->btnNext, &QToolButton::clicked, // &QToolButton::clicked(사용자가 다음트랙 버튼을 눌렀을 때 발생하는 시그널)
        this, &PlayerWindow::onNextClicked); // &PlayerWindow::onNextClicked(시그널의 정보를 받고 다음 트랙의 노래를 재생하는 슬롯)

    // 사용자가 재생/일시정지 버튼을 눌렀을 시
    // 사용자가 재생/일시정지 버튼을 누르면 &QToolButton::clicked 시그널이 신호를 주고, &PlayerWindow::onPlayPauseClicked 슬롯이 노래를 재생하거나 정지한다.
    connect(ui->btnPlayPause, &QToolButton::clicked, // &QToolButton::clicked(사용자가 다음트랙 버튼을 눌렀을 때 발생하는 시그널)
        this, &PlayerWindow::onPlayPauseClicked); // &PlayerWindow::onPlayPauseClicked(시그널의 정보를 받고 재생하거나 일시정지하는 슬롯)


    // 슬라이더/플레이어 연결

    // 노래의 총 길이를 인식
    // 노래가 선택되었을 때 &QMediaPlayer::durationChanged 시그널이 신호를 주고, &PlayerWindow::onDurationChanged 슬롯이 노래의 총 길이를 인식한다.
    connect(player, &QMediaPlayer::durationChanged, // &QMediaPlayer::durationChanged(사용자가 노래를 선택했을 때 발생하는 시그널)
        this, &PlayerWindow::onDurationChanged); // &PlayerWindow::onDurationChanged(시그널의 정보를 받고 재생바의 길이를 인식하고 바꾸는 슬롯)

    // 재생바의 위치가 바뀌었을 시 ( 재생바 위치 변경 )
    // 재생바의 위치가 바뀌었을 때 &QMediaPlayer::positionChanged 시그널이 신호를 주고, &PlayerWindow::onPositionChanged 슬롯이 바뀐 부분으로 재생바를 이동시킨다.
    connect(player, &QMediaPlayer::positionChanged, // &QMediaPlayer::positionChanged(사용자가 재생바를 움직였을 때 발생하는 시그널)
        this, &PlayerWindow::onPositionChanged); // &PlayerWindow::onPositionChanged(시그널의 정보를 받고 바뀐 부분의 재생바의 위치 정보를 움직이는 슬롯)

    // 재생바의 위치가 바뀌었을 시 ( 재생 위치 변경 )
    // 재생바의 위치가 바뀌었을 시 &QSlider::sliderMoved 시그널이 신호를 주고, &PlayerWindow::onSliderMoved 슬롯이 바뀐 부분의 재생위치로 정보를 바꾼다.
    connect(ui->sliderPosition, &QSlider::sliderMoved, // &QSlider::sliderMoved(사용자가 재생바를 움직였을 때 발생하는 시그널)
        this, &PlayerWindow::onSliderMoved); // &PlayerWindow::onSliderMoved(시그널의 정보를 받고 바뀐 부분으로 재생위치를 움직이는 슬롯)


    // 초기 상태
    ui->lblCurrentTime->setText("00:00"); // 재생 전이므로 시간 표시를 00:00으로 설정한다.
    ui->lblTotalTime->setText("00:00"); // 재생 전이므로 시간 표시를 00:00으로 설정한다.
    ui->sliderPosition->setRange(0, 0); // 노래가 선택되지 않았을 때에는 슬라이더를 움직일 수 없게 범위를 0 ~ 0 으로 지정한다.

    // 시작 곡 재생
    if (!playlistAll.empty() && currentIndex >= 0 && currentIndex < (int)playlistAll.size()) // 곡 목록이 비어있지 않고 인덱스가 유효하면 playIndex(currentIndex)로 즉시 재생 시작
        playIndex(currentIndex);

    qDebug() << "CWD =" << QDir::currentPath();
    qDebug() << "qrc play exists =" << QFile(":/icons/play.svg").exists();
    qDebug() << "qrc old-path exists =" << QFile(":/icons/icons/play.svg").exists();
}

PlayerWindow::~PlayerWindow()
{
    delete ui;
}

// 사용자가 뒤로가기 버튼을 눌렀을 때 실행되는 함수
void PlayerWindow::onBackClicked()
{
    close(); // 재생창만 닫고 목록창은 그대로
}

// 사용자가 재생/일시정지 버튼을 눌렀을 때 실행되는 함수
void PlayerWindow::onPlayPauseClicked()
{
    if (currentIndex < 0 || currentIndex >= (int)playlistAll.size()) return; // 현재 인덱스가 범위 밖이면 아무것도 하지 않고 종료 ( 안전 장치 )

    if (!isPlaying) { // 현재 재생중이 아니면 노래 시작 player->play();
        player->play();
        isPlaying = true; // 현재 재생중인 상태를 true 로 갱신
    }
    else { // 현재 재생중이면 pause(); 로 일시정지.
        player->pause();
        isPlaying = false; // 현재 재생중인 상태를 false로 갱신.
    }
    updatePlayPauseIcon(); // 상태(isPlaying)에 맞게 아이콘을 Play 또는 Pause로 갱신.
}

// 사용자가 이전 트랙 버튼을 눌렀을 때 실행되는 함수
void PlayerWindow::onPrevClicked()
{
    if (playlistAll.empty()) return; // 목록이 비어있으면 아무것도 안 함.

    int nextIndex = currentIndex - 1; // 이전 곡이므로 인덱스를 1 줄임
    if (nextIndex < 0) // 현재가 첫 곡이었다면, 이전을 누를 때 마지막 곡으로 이동(순환)
        nextIndex = static_cast<int>(playlistAll.size()) - 1; // 첫 곡이면 마지막으로

    playIndex(nextIndex); // 계산된 인덱스를 실제로 재생.
}

// 사용자가 다음 트랙 버튼을 눌렀을 때 실행되는 함수
void PlayerWindow::onNextClicked()
{
    if (playlistAll.empty()) return; // 목록이 비어있으면 아무것도 안 함.

    int nextIndex = currentIndex + 1; // 이전 곡이므로 인덱스를 1 늘림
    if (nextIndex >= static_cast<int>(playlistAll.size())) // 마지막 곡에서 다음을 누르면 첫 곡으로(순환)
        nextIndex = 0;

    playIndex(nextIndex); // 재생 실행
}

// 노래의 총 길이가 바뀌었을 때 실행되는 함수
void PlayerWindow::onDurationChanged(qint64 duration)
{
    ui->sliderPosition->setMaximum((int)duration); // 곡 전체 길이를 슬라이더 최댓값으로 설정
    ui->lblTotalTime->setText(formatTime(duration)); // 전체 길이를 mm:ss 문자열로 바꿔서 총 시간 라벨에 표시
}

// 사용자가 재생바의 위치를 바꾸었을 때 실행되는 함수 ( 재생바 위치 변경 )
void PlayerWindow::onPositionChanged(qint64 position)
{
    ui->sliderPosition->setValue((int)position); // 현재 재생 위치(ms)를 슬라이더 현재 값에 반영(재생바 움직임)
    ui->lblCurrentTime->setText(formatTime(position)); // 현재 시간을 mm:ss로 바꿔서 현재 시간 라벨에 표시
}

// 사용자가 재생바의 위치를 바꾸었을 때 실행되는 함수 ( 재생 위치 변경 )
void PlayerWindow::onSliderMoved(int position)
{
    player->setPosition(position); // 사용자가 슬라이더를 옮긴 위치(ms)로 재생 위치를 즉시 점프(시킹)
}

// 곡을 재생시키는 함수
void PlayerWindow::playIndex(int idx)
{
    if (idx < 0 || idx >= (int)playlistAll.size()) return; // 인덱스 유효성 검사. 범위 밖이면 종료

    currentIndex = idx; // 현재 곡 인덱스를 갱신.

    const auto& s = playlistAll[currentIndex]; // 현재 곡 정보를 참조로 가져옴(복사 X).
    ui->lblTitle->setText(QString::fromStdString(s.title)); // 제목을 라벨에 표시한다.
    ui->lblSinger->setText(QString::fromStdString(s.singer)); // 가수를 라벨에 표시한다.

    // 앨범아트는 아직 없으니 기본 아이콘/색 영역으로 두고, 나중에 이미지로 교체 가능
    // (원하면 mp3 메타데이터 앨범아트 읽는 것도 가능)
    // ui->lblAlbumArt->setPixmap(...);

    const QString path = QString::fromStdString(s.filePath); // 파일 경로도 QString으로 변환
    player->setSource(QUrl::fromLocalFile(path)); // 로컬 파일 경로를 QUrl로 만들고, 플레이어 재생 소스로 설정
    player->play(); // 재생 시작.

    isPlaying = true; // isPlaying을 true로 저장한다.
    updatePlayPauseIcon(); // 아이콘을 false로 바꾼다.

    emit songChangedByPlayer(path, currentIndex); // 시그널 발생 - 플레이어가 곡을 바꿨다. 를 외부에게 알림
}

// 재생/일시정지 아이콘을 업데이트 하는 함수
void PlayerWindow::updatePlayPauseIcon()
{
    /*ui->btnPlayPause->setIcon(style()->standardIcon( // isPlaying이 true면 [false]아이콘으로 변경하고, false면 [true]아이콘으로 변경한다.
        isPlaying ? QStyle::SP_MediaPause : QStyle::SP_MediaPlay
    ));*/

    ui->btnPlayPause->setIcon(
        isPlaying ? QIcon(":/icons/icons/pause.svg")
        : QIcon(":/icons/icons/play.svg")
    );
}

// 노래의 곡 시간을 ms로 변경하는 함수
QString PlayerWindow::formatTime(qint64 ms)
{
    const qint64 totalSec = ms / 1000; // ms를 초로 변환
    const qint64 min = totalSec / 60; // 전체 초에서 분 계산
    const qint64 sec = totalSec % 60; // 남는 초(0~59) 계산
    return QString("%1:%2") // "mm:ss" 형태 문자열 생성
        .arg((int)min, 2, 10, QChar('0')) // 두자리로 맞추고(00처럼), 빈자리는 0으로 채움
        .arg((int)sec, 2, 10, QChar('0'));
}

