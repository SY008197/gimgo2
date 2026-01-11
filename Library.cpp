#include "Library.h"

using std::string;      // string 타입 사용(namespace보다 이런식으로 습관화하기)
using std::vector;      // vector 타입 사용(네임스페이스 지시문은 헤더가 아닌 cpp파일에서 사용 권장)

//UI에 전달하기 위한 함수 정의
vector<SongView> Library::getAllSongs() const //모든 노래 정보를 SongView 형태로 반환
{
    vector<SongView> views; //SongView 컨테이너 생성
    for (const auto& song : SongList) //SongList 컨테이너를 순회
    {
        SongView view;               //SongView 구조체 생성
        view.title = song.getTitle();   //제목 설정
        view.singer = song.getSinger(); //가수 설정
        view.filePath = song.getFilePath(); //파일 경로 설정
        views.push_back(view);       //컨테이너에 추가
    }
    return views;                   //모든 노래 정보 반환
}



// 노래 검색 멤버 함수 정의
vector<SongView> Library::searchByTitleView(const string& title) const
{
    vector<SongView> results; //검색 결과를 담을 컨테이너

    string keyword = toLower(title); // 검색어 표준화

    for (const auto& song : SongList) //노래 목록 순회
    {
        if (toLower(song.getTitle()) == keyword) //제목이 검색어와 일치하는지 확인
        {
            results.push_back({ song.getTitle(), song.getSinger(),song.getFilePath() }); // 일치하면 결과에 추가
        }
    }
    return results; //검색 결과 반환
}

vector<SongView> Library::searchBySingerView(const string& singer) const //Library 클래스의 멤버 함수 정의(가수 검색)
{
    vector<SongView> results; //검색 결과를 담을 컨테이너

    string keyword = toLower(singer); // 검색어 표준화

    for (const auto& song : SongList) //노래 목록 순회
    {
        if (toLower(song.getSinger()) == keyword) //제목이 검색어와 일치하는지 확인
        {
            results.push_back({ song.getTitle(), song.getSinger(),song.getFilePath() }); // 일치하면 결과에 추가
        }
    }
    return results; //검색 결과 반환
}



// 노래 추가 및 제거 멤버 함수 정의
bool Library::addSong(const string& title, const string& singer, const string& filePath)
{
	if (hasSong(title, singer)) //노래가 이미 존재하는지 확인(중복등록 방지용)
        return false;

	SongList.emplace_back(title, singer, filePath); //SongList 컨테이너에 새로운 Song 객체 추가(생성자 매개변수로 제목, 가수, 파일 경로 전달)
    return true;
	// 열거형 반환으로 변경 예정
}
bool Library::removeSong(const string& title, const string& singer)
{
	for (auto it = SongList.begin(); it != SongList.end(); ++it) //SongList 컨테이너를 순회
    {
		if (it->getTitle() == title && it->getSinger() == singer) //노래 제목과 가수가 일치하는지 확인
        {
			SongList.erase(it); //일치하면 해당 노래 삭제
			return true; //삭제 성공
        }
    }
	return false; //삭제 실패(노래 없음)
	// 열거형 반환으로 변경 예정
}
// 헬퍼 함수 (부분 일치 검색 기능, 띄어쓰기 무시 기능 등 추가 필요)

// 문자열을 소문자로 변환하는 헬퍼 함수 정의
string Library::toLower(const string& str) const
{
    string lowerStr = str; // 입력 문자열을 복사
    for (char& ch : lowerStr) // 문자열의 각 문자에 대해
    {
        ch = tolower(static_cast<unsigned char>(ch)); // 문자를 소문자로 변환
    }
    return lowerStr; // 변환된 문자열 반환
}
// 노래 존재 여부 확인 헬퍼 함수
bool Library::hasSong(const string& title, const string& singer) const //노래가 이미 존재하는지 확인(중복등록 방지용)
{
    for (const auto& song : SongList)
    {
        if (song.isSame(title, singer))
            return true;
    }
    return false;
}
// 노래 찾기 헬퍼 함수 정의
const Song* Library::findSong(const string& title, const string& singer) const
{
    for (const auto& song : SongList)
    {
        if (song.isSame(title, singer))
            return &song;
    }
    return nullptr;
}
// 플레이리스트 찾기 헬퍼 함수 정의
const Playlist* Library::findPlaylist(const string& playlistName) const
{
    for (const auto& playlist : Playlists)
    {
        if (playlist.getName() == playlistName)
            return &playlist;
    }
    return nullptr;
}



// 플레이리스트 관리 멤버 함수 정의
void Library::createPlaylist(const string& playlistName) //플레이리스트 생성
{
    Playlists.emplace_back(playlistName);
}
//중복된 플레이리스트 생성 방지 기능 추가 필요

void Library::deletePlaylist(const string& playlistName) //플레이리스트 삭제
{
    for (auto it = Playlists.begin(); it != Playlists.end(); ++it) //Playlists 컨테이너를 순회
    {
        if (it->getName() == playlistName) //플레이리스트 이름이 일치하는지 확인
        {
            Playlists.erase(it); //일치하면 해당 플레이리스트 삭제
            break;               //삭제 후 반복문 종료
        }
    }
}
//플레이리스트가 없을때 예외처리 기능 추가 필요

// 플레이리스트에 노래 추가 및 제거 멤버 함수 정의(클래스 분리 후)
PlaylistAddResult Library::addSongToPlaylist(const string& playlistName, const string& title, const string& singer)
{
    Playlist* playlist = findPlaylist(playlistName);
    if (!playlist)
		return PlaylistAddResult::ADD_NOPLAYLIST; // 이름에 맞는 플레이리스트 없음

    const Song* song = findSong(title, singer);
    if (!song)
		return PlaylistAddResult::ADD_NOSONG; // 노래 없음

    bool added = playlist->addSong(*song);
    if (added)
		return PlaylistAddResult::ADD_SUCCESS; // 성공
    else
		return PlaylistAddResult::ADD_DUPLICATE; // 중복된 노래 추가 시도 결과
}

PlaylistRemoveResult Library::removeSongFromPlaylist(const string& playlistName, const string& title, const :string& singer)
{
    Playlist* playlist = findPlaylist(playlistName);
    if (!playlist)
		return PlaylistRemoveResult::REMOVE_NOPLAYLIST; // 플레이리스트 없음

    bool removed = playlist->removeSong(title, singer);
    if (removed)
		return PlaylistRemoveResult::REMOVE_SUCCESS; // 성공
    else
		return PlaylistRemoveResult::REMOVE_NOSONG;// 노래 없음
}
