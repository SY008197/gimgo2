// MusicPlayer.exe 프로그램의 시동 코드

#include "MainWindow.h" // MainWindow 클래스 선언을 가져옴, MainWindow 타입을 사용하기 위해 필요함
#include <QtWidgets/QApplication> // QtWidgets은 QApplication을 가져오기 위해 선언함. QtWidgets은 QApplication을 포함하고 있음.


int main(int argc, char *argv[]) // 프로그램 시작점. argc - 전달된 인자 개수, argv - 인자 문자열 배열
{
    QApplication app(argc, argv); // Qt 객체 생성, 반드시 main()안에서 가장 먼저 생성, 프로그램당 하나만 존재
    MainWindow window; // MainWindow.ui에서 만든 메인 창 객체 생성, 이 시점에 생성자 실행, 초기화된 UI 준비
    window.show(); // 메인 창을 화면에 표시, show없으면 창 안보임 근데 실행은 됨.
	               // showMaximized() - 최대화된 상태로 표시, showFullScreen() - 전체화면 모드로 표시
    return app.exec(); // 이벤트 루프 시작, 사용자의 행동을 기다림.
}
// main()으로 시작 -> MainWindow 객체 생성 (UI 초기화) -> show()로 창 화면에 표시 -> app.exec()로 사용자의 행동을 대기


/* 7 번째 줄에서 int argc와 char *argv[]는 프로그램 실행 시 값을 받고 고정됨. 프로그램 실행 중에는 변경 불가능.
MusicPlayer.exe 파일 실행 시 argv[0]으로 값 전달 ( 변함 없음 )
보통 exe 파일만 실행하면 argv[0]만 전달되는데 mp3 파일을 선택해서 MusicPlayer.exe 파일을 연결 프로그램으로 열면 argv[1]에 mp3 파일 경로가 전달됨.
만약 mp3 파일을 여러개 선택해서 exe 파일로 열면 늘어남. 하지만 프로그램 실행 중에는 값이 변하지 않음. 프로그램 시작 시에만 바뀜*/

/* 프로그램 시작 시 새로 만든 argc와 argv값을 main()에 넘겨주고, 그 값을 기준으로 9번째 줄에서 QApplication클래스가 Qt 애플리케이션 실행 환경을 초기화 함. */

/* 13번째 줄에서 app이라는 변수안의 객체들을 .exec()안의 이벤트 루프로 돌림. 만약 이벤트가 없다면 대기 상태. 프로그램이 종료되면 이벤트 루프도 끝남.
이벤트가 없다면 대기 상태이기 때문에 cpu 사용률이 낮음. */

/* 4번째 줄에서 사용되는 #include <모듈명/헤더명>은 Qt가 권장하는 모듈단위의 include 방식임. c++에서도 쓰긴하는데 거의 안씀 */