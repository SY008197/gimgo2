#pragma once

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QString>
#include <vector>

#include "Library.h"

QT_BEGIN_NAMESPACE
namespace Ui { class PlayerWindow; }
QT_END_NAMESPACE

class PlayerWindow : public QMainWindow // PlayerWindow를 MainWindow 형태로 지정.
{
    Q_OBJECT // 이게 있어야 signals, slots, connect, emit, QObject 기반 기능이 제대로 동작.

public: // 외부에서 접근 가능한 멤버

    // 생성자 선언. allSongs(전체 곡 목록) startIndex(처음 재생할 곡의 index번호) parent(Qt 부모 위젯. 기본값은 null로 )
    explicit PlayerWindow(const std::vector<SongView>& allSongs, int startIndex, QWidget* parent = nullptr);
    ~PlayerWindow(); // 소멸자 선언.

signals: // 시그널 선언 구역

    void songChangedByPlayer(const QString& filePath, int fullIndex); // 재생창에서 곡이 바뀌었을 때 파일 경로와 전체 목록 기준으로 index를 전달한다.

private slots: // 슬롯 함수 선언 구역

    void onBackClicked(); // 사용자가 클릭하면 재생창을 닫는 함수 선언 ( 구현은 PlayerWindow.cpp에 있음)
    void onPlayPauseClicked(); // 사용자가 클릭하면 재생하거나 일시정지 하는 함수 선언 ( 구현은 PlayerWindow.cpp에 있음)
    void onPrevClicked(); // 사용자가 클릭하면 이전 노래를 불러오는 함수 선언 ( 구현은 PlayerWindow.cpp에 있음)
    void onNextClicked(); // 사용자가 클릭하면 다음 노래를 불러오는 함수 선언 ( 구현은 PlayerWindow.cpp에 있음)

    void onDurationChanged(qint64 duration); // 노래의 총 길이를 인식하는 함수 선언 ( 구현은 PlayerWindow.cpp에 있음)
    void onPositionChanged(qint64 position); // 재생바에서 노래의 재생 위치가 바뀌었을 때 노래의 재생 위치를 바꾸는 함수 선언 ( 구현은 PlayerWindow.cpp에 있음)
    void onSliderMoved(int position); // 사용자가 재생바를 움직였을 때 재생바를 움직이는 함수 선언 ( 구현은 PlayerWindow.cpp에 있음)

private: // 클래스 내부에서만 사용하는 함수
    void playIndex(int idx); // idx 번째 곡을 재생하도록 하는 함수
    void updatePlayPauseIcon(); // 현재 상태에 맞춰 아이콘을 바꾸는 함수 ( ex 재생이면 일시정지 / 일시정지면 재생 )
    static QString formatTime(qint64 ms); // ms를 mm:ss로 바꿔주는 함수

private:
    Ui::PlayerWindow* ui = nullptr; // Qt Designer로 만든 UI 클래스 포인터

    std::vector<SongView> playlistAll; // 플레이어가 가지고 있는 전체목록
    int currentIndex = -1; // 현재 재생 중인 곡의 index 행 번호 ( 초기값으로 -1 지정 )

    QMediaPlayer* player = nullptr; // 실제 오디오 재생을 담당하는 Qt 멀티미디어 객체.
    QAudioOutput* audioOutput = nullptr; // 소리 출력(볼륨/디바이스 등)을 담당하는 객체.

    bool isPlaying = false; // 현재 재생중인지 판단. 일단 false로 지정
};
