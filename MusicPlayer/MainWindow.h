#pragma once // 이 헤더파일을 한번만 포함하도록 지시함, 여러번 포함되면 클래스 중복 정의 됨 -> 클래스 함수 중복 오류 발생함

// 클래스 선언을 위해 필요한 헤더파일을 include함 / .cpp 파일에서는 구현만 하면 되므로 .cpp에서는 include하지 않아도 됨.
// 대신 .cpp 파일에서 .h 파일을 include 해야 .h 파일에 있는 클래스를 사용할 수 있음.

#include "Library.h" // Library 클래스 선언을 가져옴. ( MainWindow 클래스에서 Library 클래스 사용 가능하게 함 )
#include <QMainWindow> // MainWindow 클래스를 QMainWindow로 상속하려면 반드시 필요함. (6번째 줄)
#include <QMediaPlayer> // 음악 재생을 위해 QMediaPlayer를 사용 가능하게 함.
#include <QAudioOutput> // QMediaPlayer의 소리를 실제 스피커 등으로 출력 가능하게 함.
#include <QInputDialog>  // 추가: 입력 다이얼로그

QT_BEGIN_NAMESPACE // Qt 전용 네임스페이스 (클래스 이름 충돌 방지)
namespace Ui { class MainWindow; } // Ui 라는 네임스페이스안에 MainWindow 클래스가 있음을 선언하고 포인터 사용이 가능.
                                   //.h에서 포인터 선언만 하고 실제 구현은 .cpp에서 처리함. ( 컴파일 속도 상승 )
QT_END_NAMESPACE

class MainWindow : public QMainWindow // QMainWindow 클래스를 상속받아 MainWindow 클래스 정의
									  // QMainWindow 클래스는 메뉴바, 툴바, 상태바 등을 쉽게 만들 수 있는 윈도우 형태의 기본 틀을 제공하는 클래스
{
	Q_OBJECT // 시그널과 슬롯 기능을 사용하려면 클래스 안에 반드시 포함되어야 하는 매크로
			 // .cpp에서 사용되는 connect() 호출을 가능하게 함.

public: // 외부에서도 접근 가능한 접근 지정자
    explicit MainWindow(QWidget* parent = nullptr); // =nullptr으로 부모 위젯이 없음을 기본값으로 설정함.
													// QWidget을 부모 위젯으로 지정하지 않으며 MainWindow는 최상위 창이 되므로 삭제되지 않기 위해 nullptr로 설정함.
	~MainWindow(); // 프로그램 종료 시 정리, main.cpp에서 app.quit() 호출 시 MainWindow 소멸자 실행

private slots: // 접근 지정자 슬롯 클래스 내부에서만 사용 가능
	void onPlayClicked(); // 재생 버튼 클릭 시 실행되는 슬롯 함수 선언 (구현은 MainWindow.cpp에 있음)
	void onPauseClicked(); // 일시정지 버튼 클릭 시 실행되는 슬롯 함수 선언 (구현은 MainWindow.cpp에 있음)

private:
    Ui::MainWindow* ui; // MainWindow.ui 파일에 있는 모든 위젯 접근을 위해 선언

    QMediaPlayer* player; // 음악 파일 로드, 재생, 일시정지 제어
    QAudioOutput* audioOutput; // 소리 출력과 볼륨 제어
    // 위 두 줄은 MainWindow.cpp의 생성자에서 연결됨

	Library library; // Library 객체 추가
	QString currentFilePath;   // 객체 추가: 재생할 파일 경로(마지막 추가된 곡)

private slots:

	void onDurationChanged(qint64 duration); // 재생바 최대값 업데이트 슬롯 함수 선언 ( 구현은 MainWindow.cpp에 있음)
	void onPositionChanged(qint64 position); // 재생바 위치 실시간으로 이동하는 슬롯 함수 선언 ( 구현은 MainWindow.cpp에 있음)
	void onSliderMoved(int position); // 사용자가 이동한 위치로 음악이 이동되는 슬롯 함수 선언 ( 구현은 MainWindow.cpp에 있음)
	void onAddSongClicked(); // 사용자가 클릭하면 노래를 추가하는 슬롯 함수 선언 ( 구현은 MainWindow.cpp에 있음)
	void onSongSelected();   // 재생 목록 클릭 시
	void onAllSongsClicked();

};

// 슬롯 함수 - 시그널의 신호를 받고 실행되는 함수
// 시그널 - 특정 이벤트가 발생했다는 신호

/* .h 파일에서는 클래스 선언 ->.cpp 파일에서는 클래스를 불러와 구현을 하는데 만약.h 파일에서 MainWindow 클래스가 사라진다면
.cpp 파일에서 MainWindow 클래스를 사용할 수 없게 되어 컴파일 오류가 발생함. 그래서 .h 파일에서는 QWidget에 부모를 설정하지 않고, .cpp 파일에서는 QWidget에
부모를 설정해서 메모리 낭비를 막기위해 삭제되게 만듦. */