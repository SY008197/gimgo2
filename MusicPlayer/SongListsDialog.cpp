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


    //ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ임시
    const QString baseDir = QCoreApplication::applicationDirPath(); // exe 폴더
    const QString musicDir = QDir(baseDir).filePath("music");       // exe/music

    auto addIfExists = [&](const QString& title,
        const QString& singer,
        const QString& fileName)
        {
            const QString fullPath = QDir(musicDir).filePath(fileName);

            if (!QFileInfo::exists(fullPath)) {
                qWarning() << "Music file missing:" << fullPath;
                return;
            }

            // 중복 방지 (title/singer 기준)
            if (!library.addSong(title.toStdString(),
                singer.toStdString(),
                fullPath.toStdString()))
            {
                // 이미 존재하거나 추가 실패
                qWarning() << "Duplicate or failed to add song:"
                    << title << "-" << singer;
            }

        };

    //  music 폴더 안 파일명만 적으면 됨
    addIfExists("Lose My Mind", "Don Toliver", "Lose My Mind - Don Toliver.mp3");
    addIfExists("Baddest", "K/DA", "Baddest - KDA.mp3");
    addIfExists("High Hopes", "Panic! At the Disco", "Panic! At the Disco - High Hopes.mp3");
    addIfExists("See You Again", "Wiz Khalifa (ft. Charlie Puth)", "See You Again - Wiz Khalifa (ft. Charlie Puth).mp3");
    addIfExists("MILLION DOLLAR BABY", "Tommy Richman", "MILLION DOLLAR BABY - Tommy Richman.mp3");
    addIfExists("Moonlight", "Kali Uchis", "Moonlight - Kali Uchis.mp3");
    addIfExists("A Bar Song", "Shaboozey", "A Bar Song - Shaboozey.mp3");
    addIfExists("Dangerously", "Charlie Puth", "Dangerously - Charlie Puth.mp3");
    addIfExists("Attention", "Charlie Puth", "Attention - Charlie Puth.mp3");
    addIfExists("POP/STARS", "K/DA", "POPSTARS - KDA.mp3");
    addIfExists("I Ain't Worried", "OneRepublic", "I Ain't Worried - OneRepublic.mp3");
    addIfExists("Sunflower", "Post Malone", "POPSTARS - Sunflower - Post Malone.mp3");

    //ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ임시



    // connect() 함수는 사용자가 특정 동작을 했을 때 호출되는 시그널과 슬롯을 연결해주는 Qt에서 제공하는 함수.
 

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

    // 검색 버튼을 눌렀을 시
    // 사용자가 검색 버튼 누르면 &PushButton::clicked 시그널이 신호를 주고, &SongListsDialog::onSearchClicked 슬롯이 사용자가 입력한 정보를 검색한다.
    connect(ui->btnSearch, &QPushButton::clicked, // &QPushButton::clicked( 검색 버튼을 눌렀을 때 발생하는 시그널 )
        this, &SongListsDialog::onSearchClicked); // &SongListsDialog::onSearchClicked( 시그널의 정보를 받고 타입에서 반환된 노래를 추가하는 슬롯 함수 )

    // 초기화 버튼을 눌렀을 시
    // 사용자가 초기화 버튼을 누르면 &QPushButton::clicked 시그널이 신호를 주고, &SongListsDialog::onClearSearchClicked 슬롯이 검색을 초기화 시킨다.
    connect(ui->btnClearSearch, &QPushButton::clicked, // &QPushButton::clicked ( 초기화 버튼을 눌렀을 때 발생하는 시그널 )
        this, &SongListsDialog::onClearSearchClicked); // &SongListsDialog::onClearSearchClicked ( 시그널의 정보를 받고 검색을 초기화 시킨다. )

    // 타이핑 즉시 검색되게 만드는 커넥트
    // 사용자가 문자를 입력할 때마다 &QLineEdit::textChanged 시그널이 신호를 주고, &SongListsDialog::onSearchTextChanged 슬롯이 검색을 한다.
    connect(ui->lineEditSearch, &QLineEdit::textChanged, // &QLineEdit::textChanged ( 사용자가 타이핑을 했을 때 발생하는 시그널 )
        this, &SongListsDialog::onSearchTextChanged); // &SongListsDialog::onSearchTextChanged ( 시그널의 정보를 받고 검색하는 함수 호출 )
    

    refreshList();
}

SongListsDialog::~SongListsDialog()
{
    delete ui;
}

// 사용자가 리스트에서 노래를 선택했을 때 실행되는 함수
void SongListsDialog::onSongSelected()
{
    int index = ui->listWidgetSongs->currentRow(); // 현재 노래목록 리스트에서 사용자가 선택한 노래의 행 번호를 index에 저장함.
    if (index < 0 || index >= (int)currentDisplayedSongs.size()) return; // 아무것도 선택안했을때와 선택범위가 index 범위를 벗어나면 크랙을 방지하기 위해 return으로 함수를 종료시킴.

    const auto selected = currentDisplayedSongs[index]; // 지금 보여지고 있는 리스트에서 사용자가 선택한 노래의 index번호를 selected에 저장함. selected는 songview(제목, 가수 파일 경로) 타입

    const auto allSongs = library.getAllSongs(); // 라이브러리에 저장된 전체 곡 목록을 allSongs에 저장함.
    int fullIndex = -1; // allSongs에서 사용자가 선택한 노래의 index 번호를 찾기위해 선언한 함수. 일단 -1로 저장.
    for (int i = 0; i < (int)allSongs.size(); ++i) { // 전체 목록 allSongs에서 0부터  끝까지 순회하며 일치하는 곡을 찾기 시작.
        if (allSongs[i].filePath == selected.filePath) { // 전체 목록 allSongs와 사용자가 선택한 노래의 index 번호가 맞는지 확인.
            fullIndex = i; // 만약 일치한다면 fullIndex에 1로 저장.
            break;
        }
    }
    if (fullIndex < 0) return; // 끝까지 못찾으면 fullIndex는 아직 초기 값인 -1이니까 안전하게 종료.

    // 기존 재생창 닫기 (자동 삭제)
    if (playerWin) { // 기존에 playerwin이 열려잇는지 확인.
        playerWin->close();   // 기존의 재생창을 닫는다.
        playerWin = nullptr;  // 포인터 즉시 끊기(포인트를 null로 만들어서 이미 삭제될 창에 접근 하는걸 방지함.)
    }

    playerWin = new PlayerWindow(allSongs, fullIndex, this); // 새로운 재생창을 전체목록allSongs과 시작 인덱스fullIndex로 생성함
    playerWin->setAttribute(Qt::WA_DeleteOnClose); // 창이 닫힐때 객채를 자동으로 삭제시킴.

    // playerwin 객체를 닫을 때 실행되는 슬롯으로 객채를 닫을 때 null로 만들어서 초기화를 진행함.
    connect(playerWin, &QObject::destroyed, this, [this]() {
        playerWin = nullptr;
        });

    // PlayerWindow에서 이전/다음으로 곡이 바뀔때 songChangedByPlayer시그널이 발생하면, SongListsDialog의 onPlayerSongChanged가 호출되도록 연결.
    // 이 connect가 있으면 재생창에서 노래를 바꿀 때 목록창에서도 선택된 노래가 바뀐다.
    connect(playerWin, &PlayerWindow::songChangedByPlayer,
        this, &SongListsDialog::onPlayerSongChanged);

    playerWin->show(); // 새로 만든 재생창을 띄움.
}

// 리스트에 전체 노래를 띄워주는 함수
void SongListsDialog::refreshList()
{
    currentDisplayedSongs = library.getAllSongs(); // SongListsDialog를 처음 열었거나 초기화 버튼을 눌렀을 때, 검색 창이 비어있을 때 초기상태로 돌려준다.
    refreshList(currentDisplayedSongs);
}

// 라이브러리에 들어있는 노래들을 리스트에 띄우는 함수 ( 렌더링 전용 함수로 라이브러리에 접근 X )
void SongListsDialog::refreshList(const std::vector<SongView>& songsToShow)
{
    ui->listWidgetSongs->clear();

    for (const auto& s : songsToShow)
    {
        ui->listWidgetSongs->addItem(
            QString::fromStdString(s.title) + " - " +
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

    if (!library.addSong(title.toStdString(),
        singer.toStdString(),
        filePath.toStdString())) {
        qWarning() << "Duplicate song:" << title << singer;
    }


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

// 검색 버튼 기능 함수
void SongListsDialog::onSearchClicked()
{
    QString q = ui->lineEditSearch->text().trimmed(); // 검색어에 공백을 제거한다.

    // 검색창이 비어 있으면 초기화 한다.
    if (q.isEmpty())
    {
        refreshList();
        return;
    }

    std::vector<SongView> results; //노래가 담겨있는 SongList 준비

    const bool byTitle = (ui->comboSearchType->currentIndex() == 0); // 검색 타입이 제목인지, 가수인지 인식하는 단계
    if (byTitle)
        results = library.searchByTitleView(q.toStdString()); // 제목 검색으로 Library의 SongList를 검색해서 vector<SongView>로 반환
    else
        results = library.searchBySingerView(q.toStdString()); // 가수 검색으로 Library의 SongList를 검색해서 vector<SongView>로 반환

    currentDisplayedSongs = results;      // 현재 보이는 화면으로 갱신한다. 이게 없으면 갱신안된 검색전 index로 노래를 불러옴.
    refreshList(currentDisplayedSongs);   // 검색 결과만 리스트에 표시
}

//초기화 버튼 기능 함수
void SongListsDialog::onClearSearchClicked()
{
    ui->lineEditSearch->clear();
    refreshList(); // 초기화 버튼을 눌렀을 시 refreshList() 함수를 불러오며 초기화 시킴
}

// 타이핑 즉시 필터링 되는 함수
void SongListsDialog::onSearchTextChanged(const QString& text)
{
    Q_UNUSED(text); //텍스트 매개변수는 사용하지 않는다고 선언
    onSearchClicked(); // 사용자가 타이핑을 할때마다 바로 사용됨
}

// 재생창에서 노래가 바뀔때 실행되는 함수
void SongListsDialog::onPlayerSongChanged(const QString& filePath, int fullIndex)
{
    Q_UNUSED(filePath);

    ui->lineEditSearch->clear(); // 검색창에 들어가있는 텍스트를 지움.
    refreshList(); // 전체 노래 목록을 표시함.

    if (fullIndex >= 0 && fullIndex < ui->listWidgetSongs->count()) // fullIndex가 유효한 범위인지 검사. 음수거나 범위 밖은 모두 포함 안함.
    {
        ui->listWidgetSongs->setCurrentRow(fullIndex); // 리스트에서 현재 선택된 노래의 행 번호를 index로 fullIndex에 저장
        ui->listWidgetSongs->scrollToItem(ui->listWidgetSongs->currentItem()); // 선택된 노래가 리스트 밖에 있을 수 있으니 스크롤을 자동으로 이동시킴.
    }
}
