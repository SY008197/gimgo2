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
        view.title = song.S_title;   //제목 설정
        view.singer = song.S_singer; //가수 설정
        view.filePath = song.S_filePath; //파일 경로 설정
        views.push_back(view);       //컨테이너에 추가
    }
    return views;                   //모든 노래 정보 반환
}
vector<SongView> Library::searchByTitleView(const string& title) const //제목으로 검색한 노래 정보를 SongView 형태로 반환
{
    vector<SongView> views; //SongView 컨테이너 생성
    vector<Song> results = searchByTitle(title); //제목으로 검색한 노래들 가져오기
    for (const auto& song : results) //검색 결과를 순회
    {
        SongView view;               //SongView 구조체 생성
        view.title = song.S_title;   //제목 설정
        view.singer = song.S_singer; //가수 설정
        view.filePath = song.S_filePath; //파일 경로 설정
        views.push_back(view);       //컨테이너에 추가
    }
    return views;                   //검색된 노래 정보 반환
}
vector<SongView> Library::searchBySingerView(const string& singer) const //가수로 검색한 노래 정보를 SongView 형태로 반환
{
    vector<SongView> views; //SongView 컨테이너 생성
    vector<Song> results = searchBySinger(singer); //가수로 검색한 노래들 가져오기
    for (const auto& song : results) //검색 결과를 순회
    {
        SongView view;               //SongView 구조체 생성
        view.title = song.S_title;   //제목 설정
        view.singer = song.S_singer; //가수 설정
        view.filePath = song.S_filePath; //파일 경로 설정
        views.push_back(view);       //컨테이너에 추가
    }
    return views;                   //검색된 노래 정보 반환
}


// 노래 추가 및 제거 멤버 함수 정의
void Library::addSong(const string& title, const string& singer, const string& filePath)  // 매개변수로 노래제목, 가수명, 파일 경로를 받음(이때 값은 상수로 고정시키고 참조만 한다)
{
    Song newSong;                    // 새로운 노래 구조체 생성
    newSong.S_title = title;        // 제목 설정
    newSong.S_singer = singer;    // 가수 설정
    newSong.S_filePath = filePath;        // 파일 경로 설정
    SongList.push_back(newSong);    // 컨테이너에 노래 추가
}
void Library::removeSong(const std::string& title, const std::string& singer)
{
    for (auto it = SongList.begin(); it != SongList.end(); ++it) //SongList 컨테이너를 순회
    {
        if (it->S_title == title && it->S_singer == singer) //노래 제목과 가수가 일치하는지 확인
        {
            SongList.erase(it); //일치하면 해당 노래 삭제
            break;               //삭제 후 반복문 종료
        }
	}
}
//중복된 노래 추가 방지 기능 추가
bool Library::hasSong(const string& title, const string& singer) const //노래가 이미 존재하는지 확인(중복등록 방지용)
{
    for (const auto& song : SongList)
    {
        if (song.S_title == title && song.S_singer == singer)
            return true;
    }
    return false;
}

// 플레이리스트 관리 멤버 함수 정의
void Library::createPlaylist(const string& playlistName) //플레이리스트 생성
{
    Playlist newPlaylist;            // 새로운 플레이리스트 구조체 생성
    newPlaylist.P_name = playlistName; // 플레이리스트 이름 설정
    Playlists.push_back(newPlaylist); // 컨테이너에 플레이리스트 추가
}
//중복된 플레이리스트 생성 방지 기능 추가 필요

void Library::deletePlaylist(const string& playlistName) //플레이리스트 삭제
{
    for (auto it = Playlists.begin(); it != Playlists.end(); ++it) //Playlists 컨테이너를 순회
    {
        if (it->P_name == playlistName) //플레이리스트 이름이 일치하는지 확인
        {
            Playlists.erase(it); //일치하면 해당 플레이리스트 삭제
            break;               //삭제 후 반복문 종료
        }
    }
}
//플레이리스트가 없을때 예외처리 기능 추가 필요

PlaylistAddResult Library::addSongToPlaylist(const string& playlistName, const std::string& title, const std::string& singer) //플레이리스트에 노래 추가
{
    for (auto& playlist : Playlists)    // 플레이리스트 찾기         
    {
        if (playlist.P_name == playlistName) //플레이리스트 이름이 일치하는지 확인
        {
            for (const auto& song : SongList)   // 노래 찾기
            {
                if (song.S_title == title && song.S_singer == singer)   //노래 제목과 가수가 일치하는지 확인
                {
                    playlist.P_songs.push_back(song);   //플레이리스트에 노래 추가
                    return PlaylistAddResult::ADD_SUCCESS; // 성공 (UI가 이걸 입력받으면 노래 추가 완료)
                }
            }
			return PlaylistAddResult::ADD_NOSONG; // 노래 없음 (UI가 이걸 입력받으면 파일 경로 입력 UI띄우기) 
        }
    }
	return PlaylistAddResult::ADD_NOPLAYLIST; // 이름에 맞는 플레이리스트 없음 (UI가 이걸 입력받으면 플레이리스트를 생성할건지 묻고 띄우기)
}


PlaylistRemoveResult Library::removeSongFromPlaylist(const string& playlistName, const std::string& title, const std::string& singer) //플레이리스트에서 노래 제거
{
	for (auto& playlist : Playlists)    // 플레이리스트 찾기
    {
		if (playlist.P_name == playlistName)    //플레이리스트 이름이 일치하는지 확인
        {
			auto& songs = playlist.P_songs; // 해당 플레이리스트의 노래들에 대한 참조

			for (auto it = songs.begin(); it != songs.end(); ++it)  // 노래 찾기
            {
				if (it->S_title == title && it->S_singer == singer) //노래 제목과 가수가 일치하는지 확인
                {
					songs.erase(it);    //플레이리스트에서 노래 제거
					return PlaylistRemoveResult::REMOVE_SUCCESS; // 성공 (UI가 이걸 입력받으면 노래 제거 완료)
                }
            }
			return PlaylistRemoveResult::REMOVE_NOSONG; // 노래 없음 (UI가 이걸 입력받으면 노래 목록 다시 보여주기)
        }
    }
	return PlaylistRemoveResult::REMOVE_NOPLAYLIST; // 플레이리스트 없음 (UI가 이걸 입력받으면 플레이리스트 목록 다시 보여주기)
}




// 노래 검색 멤버 함수 정의
vector<SongView> Library::searchByTitleView(const string& title) const
{
	vector<SongView> results; //검색 결과를 담을 컨테이너

	string keyword = toLower(title); // 검색어 표준화

	for (const auto& song : SongList) //노래 목록 순회
    {
		if (toLower(song.S_title) == keyword) //제목이 검색어와 일치하는지 확인
        {
			results.push_back({ song.S_title, song.S_singer,song.S_filePath }); // 일치하면 결과에 추가
        }
    }
	return results; //검색 결과 반환
}

vector<SongView> Library::searchBySingerView(const string& singer) //Library 클래스의 멤버 함수 정의(가수 검색)
{
    vector<SongView> results; //검색 결과를 담을 컨테이너

    string keyword = toLower(singer); // 검색어 표준화

    for (const auto& song : SongList) //노래 목록 순회
    {
        if (toLower(song.S_singer) == keyword) //제목이 검색어와 일치하는지 확인
        {
            results.push_back({ song.S_title, song.S_singer,song.S_filePath }); // 일치하면 결과에 추가
        }
    }
    return results; //검색 결과 반환
}



// 문자열을 소문자로 변환하는 헬퍼 함수 정의
string Library::toLower(const string& str) const //문자열을 소문자로 변환하는 헬퍼 함수
{
    string lowerStr = str; // 입력 문자열을 복사
    for (char& ch : lowerStr) // 문자열의 각 문자에 대해
    {
        ch = std::tolower(static_cast<unsigned char>(ch)); // 문자를 소문자로 변환
    }
    return lowerStr; // 변환된 문자열 반환
}
// 부분 일치 검색 기능, 띄어쓰기 무시 기능 등 추가 필요