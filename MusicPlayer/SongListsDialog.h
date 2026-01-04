#pragma once // 이 헤더파일을 한번만 포함하도록 지시함, 여러번 포함되면 클래스 중복 정의 됨 -> 클래스 함수 중복 오류 발생함

#include <QString>
#include <QDialog> // SongListsDialog는 Dialog 형태기 때문에 QDialog로 가져옴
#include "Library.h" // Library 클래스 선언을 가져옴. ( SongListsDialog 클래스에서 Library 클래스 사용 가능하게 함 )
#include <QMediaPlayer> // 음악 재생을 위해 QMediaPlayer를 사용 가능하게 함.
#include <QAudioOutput> // QMediaPlayer의 소리를 실제 스피커 등으로 출력 가능하게 함.

QT_BEGIN_NAMESPACE // Qt 전용 네임스페이스 (클래스 이름 충돌 방지)
namespace Ui { class SongListsDialog; } // Ui 라는 네임스페이스안에 SongListsDialog 클래스가 있음을 선언하고 포인터 사용이 가능.
                                   //.h에서 포인터 선언만 하고 실제 구현은 .cpp에서 처리함. ( 컴파일 속도 상승 )
QT_END_NAMESPACE

class SongListsDialog : public QDialog // SongListsDialog 메인 프로그램 흐름을 잠시 멈췄다가 결과를 받고 돌아오는 용도
									  // 설정 창 , 추가 / 수정 창 , 확인 / 선택 창
{
    Q_OBJECT // 시그널과 슬롯 기능을 사용하려면 클래스 안에 반드시 포함되어야 하는 매크로
			 // .cpp에서 사용되는 connect() 호출을 가능하게 함.

public:
    explicit SongListsDialog(Library& libraryRef, QWidget* parent = nullptr); // =nullptr으로 부모 위젯이 없음을 기본값으로 설정함.
													                          // QWidget을 부모 위젯으로 지정하지 않으며 SongListsDialog 는 최상위 창이 되므로 삭제되지 않기 위해 nullptr로 설정함.
    ~SongListsDialog(); // 프로그램 종료 시 정리, main.cpp에서 app.quit() 호출 시 SongListsDialog 소멸자 실행

    void refreshList(); // 현재 Library에 들어 있는 노래 목록을 UI(QListWidget)에 다시 그려주는 함수 선언

private slots: // 접근 지정자 슬롯 클래스 내부에서만 사용 가능

    // 재생 관련
    void onPlayClicked(); // 재생 버튼 클릭 시 실행되는 슬롯 함수 선언 (구현은 SongListsDialogw.cpp에 있음)
    void onPauseClicked(); // 일시정지 버튼 클릭 시 실행되는 슬롯 함수 선언 (구현은 SongListsDialog.cpp에 있음)
    void onDurationChanged(qint64 duration); // 재생바 최대값 업데이트 슬롯 함수 선언 ( 구현은 SongListsDialog.cpp에 있음)
    void onPositionChanged(qint64 position); // 재생바 위치 실시간으로 이동하는 슬롯 함수 선언 ( 구현은 SongListsDialog.cpp에 있음)
    void onSliderMoved(int position); // 사용자가 이동한 위치로 음악이 이동되는 슬롯 함수 선언 ( 구현은 SongListsDialog.cpp에 있음)

    // 목록 추가 관련
    void onAddSongClicked(); // 사용자가 클릭하면 노래를 추가하는 슬롯 함수 선언 ( 구현은 SongListsDialog.cpp에 있음)
    void onCloseClicked(); // 사용자가 클리하면 SongListsDialog 윈도우를 닫은 슬롯 함수 선언 ( 구현은 SongListsDialog.cpp 에 있음 )
    void onSongSelected(); // 사용자가 노래를 클릭했을 때 노래의 행 번호를 가져오는 슬롯 함수 선언 ( 구현은 SongListsDialog.cpp 에 있음)


private:

    Ui::SongListsDialog* ui; // SongListsDialog.ui 파일에 있는 모든 위젯 접근을 위해 선언
    Library& library; // Library 객체 추가
    QString currentFilePath;      // 현재 선택된 곡 경로

    QMediaPlayer* player = nullptr;
    QAudioOutput* audioOutput = nullptr;

};