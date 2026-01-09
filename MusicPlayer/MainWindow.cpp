#include <QListWidgetItem> // QListWidgetItem 클래스 (재생목록)
#include <QFileDialog>  // 파일 선택 다이얼로그
#include "SongListsDialog.h" //
#include <QMessageBox> // 메시지 박스
#include "MainWindow.h" // MainWindow.h에서 선언한 MainWindow 클래스 선언을 가져옴. ( MainWindow 클래스의 정의는 MainWindow.cpp에 있음 ) 선언 =/= 정의
#include "ui_MainWindow.h" // MainWindow.ui 파일을 컴파일하면 자동으로 생성됨. 주로 버튼, 슬라이더 같은 UI 객체들이 들어 있음.
                           // MainWindow.ui 안에 있는 버튼들의 오브젝트 이름과 속성들을 불러오는 헤더 구현부(MainWindow.cpp)에서만 접근 하기때문에 컴파일 속도가 올라가고 구조가 깔끔해짐.

MainWindow::MainWindow(QWidget* parent) // #include "MainWindow.h"에서 가져온 MainWindow 클래스로 MainWindow::MainWindow 생성자를 정의
                                        // QWidget*은 QWidget 클래스의 주소(포인터) parent는 변수 ( QWidget 은 주로 부모, 자식 관계에서 부모 역할을 하는 클래스임. QWidget <- 상위 폴더 )

    : QMainWindow(parent) // QMainWindow 클래스의 생성자를 호출하여 QWidget의 자식이 되고, MainWindow에게 QMainWindow의 기능을 상속해줌.
    //굳이 QMainWindow가 QWidget의 자식이 되며 MainWindow에게 기능을 상속해주는 이유는 모든 GUI 위젯들이 QWidget을 기반으로 만들어지기 때문에 부모 성격인 QWidget을 부모로 가진다.
    , ui(new Ui::MainWindow) // 프로그램에 Ui::MainWindow 객체들을 생성 하고 ui 포인터가 이를 가리키도록 함.
{
	ui->setupUi(this); // setupUi() 함수는 MainWindow.ui 파일에서 만든 모든 UI 요소들을 실제로 생성하고 배치 해주는 함수 ( this 는 MainWindow 객체 자신을 가리킴 )




    // 리스트 안에 들어가는 전체 목록 버튼은 왜 item으로 구분하여서 만들었나?
    // 리스트 안에 들어가는 객체들은 모두 변함이 가능한 객체들임..
    // 전체 목록은 변함이 없지만 ( 이름 변경 x 삭제 x 추가 x ) 왜 리스트에 넣었나? 
    // 일단 전체 목록과 재생 목록은 모두 index로 구분할 수 있는 이점이 있고,
    // 전체 목록만 item 으로 하면 프로그램에 비효율적이며, 나중에 최근재생이나 즐겨찾기 같은 노래들을
    // item으로 선언할지, 버튼으로 구현할지 갈림. 그리고 item과 버튼으로 나뉘면 프로그램에 일관성이 사라지고 보수와 운영에 오류가 생길 수 있음. 오류는 절대 안된다. 진짜로
    // 
    // 생성되는 재생 목록
    ui->listWidgetLibrary->clear(); // listWidgetLibrary 안에 이미 들어 있던 모든 항목을 제거

    QListWidgetItem* allItem = new QListWidgetItem(QStringLiteral(u"전체 목록")); // listwidget에 들어갈 아이템 생성 이름은 ( 전체 목록 )
    allItem->setTextAlignment(Qt::AlignCenter);                                   // 아이템 안의 텍스트를 가운데 정렬
    allItem->setSizeHint(QSize(0, 60));                                           // 아이템의 권장 크기(높이)를 60px로 지정 가로(0)는 리스트 폭을 자동 사용

    QFont f = ui->listWidgetLibrary->font();                                      // 리스트 기본 폰트 복사
    f.setPointSize(14);                                                           // 글 크기 14pt 설정
    f.setBold(true);                                                              // bold 굵게 설정
    allItem->setFont(f);                                                          // 이 폰트를 전체 목록에만 설정

    ui->listWidgetLibrary->addItem(allItem);                                      // 방금 만든 아이템을 listWidgetLibrary 에 추가 index 0번재 항목부터 추가


    // ui 템플릿 ( border 테두리 : 1 px / borer-radius 둥근 모서리 : 10 px /margin 아이템 사이 여백 : 6 px ) , 선택되면 어두운 배경색으로 변경 item:selected { background: #2b2b2b; }
    ui->listWidgetLibrary->setStyleSheet(R"(
QListWidget::item { border: 1px solid #444; border-radius: 10px; margin: 6px; }
QListWidget::item:selected { background: #2b2b2b; }
)");




    setWindowTitle("뮤직 플레이어"); // 윈도우 타이틀 이름 설정


    // connect() 함수는 사용자가 특정 동작을 했을 때 호출되는 시그널과 슬롯을 연결해주는 Qt에서 제공하는 함수.


    // 전체 노래 목록 객체 생성 ( item )
    // listWidgetLibrary에 재생 목록 생성하는 &QListWidget::itemClicked 시그널이 신호를 주고, &MainWindow::onLibraryItemClicked 슬롯 함수가 실행된다.
    connect(ui->listWidgetLibrary, &QListWidget::itemClicked, // &QListWidget::itemClicked ( 생성 시그널 )
        this, &MainWindow::onLibraryItemClicked); // &MainWindow::onLibraryItemClicked ( 생성 슬롯 밑에 함수 연결 )


}

MainWindow::~MainWindow()
{
    delete ui;
}
/* ui(new Ui::MainWindow)로 객체들을 만들고 이를 ui가 가리키도록 하고, ui->setupUi(this); 함수로 실제 UI 요소들을 만들었으니 필요없어진 메모리만 차지하는 ui는 다시 삭제함.
이미 생성된 객체들은 부모가 MainWindow이므로 ui를 삭제한다고 해도 객체들은 사라지지 않음. */


// 재생 목록 띄우는 함수
void MainWindow::onLibraryItemClicked(QListWidgetItem* item)
{
    int row = ui->listWidgetLibrary->row(item); // 클릭한 항목이 몇 번째 줄인지 확인 ( 재생 목록 index 형태 )

    if (row == 0) // 0번째 = 전체 목록
    {
        this->hide();                       // MainWindow 창 숨기기
        SongListsDialog dlg(library, this); // 전체 노래 목록 창(SongListsDialog) 객체 생성
        dlg.exec();                         // SongListsDialog를 창으로 띄움 닫기 버튼 누르면 그때 다음 줄로 넘어감
        this->show();                       // 숨겨놨던 MainWindow를 다시 화면에 표시
    }
}






