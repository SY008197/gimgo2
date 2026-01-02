#include "MainWindow.h" // MainWindow.h에서 선언한 MainWindow 클래스 선언을 가져옴. ( MainWindow 클래스의 정의는 MainWindow.cpp에 있음 ) 선언 =/= 정의
#include "ui_MainWindow.h" // MainWindow.ui 파일을 컴파일하면 자동으로 생성됨. 주로 버튼, 슬라이더 같은 UI 객체들이 들어 있음.
                           // MainWindow.ui 안에 있는 버튼들의 오브젝트 이름과 속성들을 불러오는 헤더 구현부(MainWindow.cpp)에서만 접근 하기때문에 컴파일 속도가 올라가고 구조가 깔끔해짐.

MainWindow::MainWindow(QWidget* parent) // #include "MainWindow.h"에서 가져온 MainWindow 클래스로 MainWindow::MainWindow 생성자를 정의
                                        // QWidget*은 QWidget 클래스의 주소(포인터) parent는 변수 ( QWidget 은 주로 부모, 자식 관계에서 부모 역할을 하는 클래스임. QWidget <- 상위 폴더 )

    : QMainWindow(parent) // QMainWindow 클래스의 생성자를 호출하여 QWidget의 자식이 되고, MainWindow에게 QMainWindow의 기능을 상속해줌.
    , ui(new Ui::MainWindow) // 프로그램에 Ui::MainWindow 객체들을 생성 하고 ui 포인터가 이를 가리키도록 함.
{
	ui->setupUi(this); // setupUi() 함수는 MainWindow.ui 파일에서 만든 모든 UI 요소들을 실제로 생성하고 배치 해주는 함수 ( this 는 MainWindow 객체 자신을 가리킴 )

    // 오디오 출력, 플레이어 생성
    audioOutput = new QAudioOutput(this); // 소리를 내는 장치를 제어하는 클래스 ( 볼륨, 출력 장치, 상태 등을 관리 가능)
	player = new QMediaPlayer(this); // 오디오를 재생하는 클래스, 소리 출력을 위해 QAudioOutput 객체와 연결이 필요함.
	player->setAudioOutput(audioOutput); // QAudioOutput 객체를 QMediaPlayer 객체와 연결. player 재생하면 audioOutput 통해 소리가 출력됨.
    // player->play(); 노래 재생.
    // audioOutput->setVolume(0.5); 볼륨 50%로 설정.

    // 음악 파일 주소에 한글 포함 시 오류 발생할 수 있음. (테스트)
    player->setSource(QUrl::fromLocalFile(
        "C:/Users/jaemy/Downloads/test.mp3"   // 본인 PC 경로
    ));
    // player->setSource(QUrl::fromLocalFile("파일 경로")); 재생할 파일

    // connect() 함수는 사용자가 특정 동작을 했을 때 호출되는 시그널과 슬롯을 연결해주는 Qt에서 제공하는 함수.
	// 재생 버튼 클릭 시
    connect(ui->btnPlay, &QPushButton::clicked, // &QPushButton::clicked(사용자의 특정 동작 시그널)
        this, &MainWindow::onPlayClicked); // &MainWindow::onPlayClicked(사용자의 동작에 반응하는 슬롯)
	// btnPlay 버튼을 클릭하면 $PushButton::clicked 알림이 발생하고, $MainWindow::onPlayClicked 슬롯이 실행된다.

	// 일시정지 버튼 클릭 시
	connect(ui->btnPause, &QPushButton::clicked, // $QPushButton::clicked(사용자의 특정 동작 시그널)
		this, &MainWindow::onPauseClicked); // &MainWindow::onPauseClicked(사용자의 동작에 반응하는 슬롯)
	// btnPause 버튼을 클릭하면 $QPushButton::clicked 알림이 발생하고, $MainWindow::onPauseClicked 슬롯이 실행된다.

    // 재생 길이 변경 시
	connect(player, &QMediaPlayer::durationChanged, // &QMediaPlayer::durationChanged(재생하는 노래의 길이를 파악하는 시그널)
		this, &MainWindow::onDurationChanged); // $MainWindow::onDurationChanged(파악한 노래 길이로 재생바의 길이를 변경하는 슬롯)
	// 노래를 재생하거나 바꾸면 $QMediaPlayer::durationChanged 시그널이 신호를 주고, $MainWindow::onDurationChanged 슬롯이 실행된다.
    
	// 노래가 재생될 때 노래의 위치 변경 시
	connect(player, &QMediaPlayer::positionChanged, // &QMediaPlayer::positionChanged(현재 재생되는 노래의 위치를 파악하는 시그널)
		this, &MainWindow::onPositionChanged); // &MainWindow::onPositionChanged(시그널의 정보를 받고 재생바의 위치를 바꾸는 슬롯)
	// 노래가 재생될 때 노래의 위치를 계속 파악하여 $QMediaPlayer::positionChanged 시그널이 신호를 주고, $MainWindow::onPositionChanged 슬롯이 재생바의 위치를 바꿔준다.

    // 슬라이더를 사용자가 움직였을 때
	connect(ui->sliderPosition, &QSlider::sliderMoved, // &QSlider::sliderMoved(사용자가 재생바를 움직일 때 실시간으로 바뀌는 슬라이더의 위치를 알려주는 시그널)
		this, &MainWindow::onSliderMoved); // &MainWindow::onSliderMoved(시그널의 정보를 받고 노래 재생 위치를 바꾸는 슬롯 )
	// 사용자가 재생바를 움직이면 $QSlider::sliderMoved 시그널이 신호를 주고, $MainWindow::onSliderMoved 슬롯이 노래 재생 위치를 바꿔준다.
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onPlayClicked() // 28번째 줄에서 사용자의 신호를 시그널로 받고 실행되는 슬롯 함수
{                                // play()함수는 QMediaPlayer 클래스의 함수로 음악 재생을 시작함.
    player->play();
}

void MainWindow::onPauseClicked() // 33번째 줄에서 사용자의 신호를 시그널로 받고 실행되는 슬롯 함수
{                                 // pause()함수는 QMediaPlayer 클래스의 함수로 음악 재생을 일시정지함.
    player->pause();
}

void MainWindow::onDurationChanged(qint64 duration) // 38번째 줄에서 재생 길이 변경 시그널을 받고 실행되는 슬롯 함수
{                                                   // setMaximum(static_cast<int>())은 재생바의 최대값을 설정함.
    ui->sliderPosition->setMaximum(static_cast<int>(duration));
}

void MainWindow::onPositionChanged(qint64 position) // 43번째 줄에서 노래 재생 위치 변경 시그널을 받고 실행되는 슬롯 함수
{                                                   // setValue(static_cast<int>())는 재생바의 현재 위치를 설정함.
    ui->sliderPosition->setValue(static_cast<int>(position));
}

void MainWindow::onSliderMoved(int position) // 48번째 줄에서 사용자가 재생바를 움직였을 때 시그널을 받고 실행되는 슬롯 함수
{                                            // setPosition()는 노래 재생 위치를 설정함.
    player->setPosition(position);
}


// 8번째 줄에서 굳이 QMainWindow가 QWidget의 자식이 되며 MainWindow에게 기능을 상속해주는 이유는 모든 GUI 위젯들이 QWidget을 기반으로 만들어지기 때문에 부모 성격인 QWidget을 부모로 가진다.

/* 53번째 줄은 9번째 줄에서 객체들을 만들고 이를 ui가 가리키도록 하고, 11번째 줄에서 setupUi() 함수로 실제 UI 요소들을 만들었으니 필요없어진 메모리만 차지하는 ui는 다시 삭제함.
이미 생성된 객체들은 부모가 MainWindow이므로 ui를 삭제한다고 해도 객체들은 사라지지 않음. */


