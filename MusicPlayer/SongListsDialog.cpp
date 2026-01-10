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
            if (!library.hasSong(title.toStdString(), singer.toStdString())) {
                library.addSong(title.toStdString(),
                    singer.toStdString(),
                    fullPath.toStdString());
            }
        };

    //  music 폴더 안 파일명만 적으면 됨
    addIfExists("Lose My Mind", "Don Toliver", "Lose My Mind - Don Toliver.mp3");
    addIfExists("Baddest", "K/DA", "Baddest - KDA.mp3");
    addIfExists("High Hopes", "Panic! At the Disco", "Panic! At the Disco - High Hopes.mp3");
    addIfExists("See You Again ft.Charlie Puth", "Wiz Khalifa", "See You Again ft. Charlie Puth - Wiz Khalifa.mp3");
    addIfExists("MILLION DOLLAR BABY", "Tommy Richman", "MILLION DOLLAR BABY - Tommy Richman.mp3");
    addIfExists("Moonlight", "Kali Uchis", "Moonlight - Kali Uchis.mp3");
    addIfExists("A Bar Song", "Shaboozey", "A Bar Song - Shaboozey.mp3");
    addIfExists("Dangerously", "Charlie Puth", "Dangerously - Charlie Puth.mp3");
    addIfExists("Attention", "Charlie Puth", "Attention - Charlie Puth.mp3");
    addIfExists("POP/STARS", "K/DA", "POPSTARS - KDA.mp3");

    //ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ임시



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
    int index = ui->listWidgetSongs->currentRow();
    if (index < 0) return;

    if (index >= static_cast<int>(currentDisplayedSongs.size())) return;

    currentFilePath = QString::fromStdString(currentDisplayedSongs[index].filePath); // 가져온 노래 파일을 QMedia가 읽을 수 있게 파일을 변환
    player->setSource(QUrl::fromLocalFile(currentFilePath));



    // 선택하면 바로 재생하고 싶으면 아래 줄 켜면 됨
    // player->play();
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

    const bool byTitle = (ui->comboSearchType->currentText() == QStringLiteral("Title")); // 검색 타입이 제목인지, 가수인지 인식하는 단계
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