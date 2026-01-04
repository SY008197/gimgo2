#include "Library.h"

using std::string;      // string 타입 사용(namespace보다 이런식으로 습관화하기)
using std::vector;      // vector 타입 사용(네임스페이스 지시문은 헤더가 아닌 cpp파일에서 사용 권장)

// 노래 추가 멤버 함수 정의
void Library::addSong(const string& title, const string& singer, const string& filePath)  // 매개변수로 노래제목, 가수명, 파일 경로를 받음(이때 값은 상수로 고정시키고 참조만 한다)
{
    Song newSong;                    // 새로운 노래 구조체 생성
    newSong.S_title = title;        // 제목 설정
    newSong.S_singer = singer;    // 가수 설정
    newSong.S_filePath = filePath;        // 파일 경로 설정
    SongList.push_back(newSong);    // 컨테이너에 노래 추가
}
//중복된 노래 추가 방지 기능 추가 필요



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

void Library::addSongToPlaylist(const string& playlistName, const std::string& title, const std::string& singer) //플레이리스트에 노래 추가
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
                    return true; // 성공
                }
            }
            return false; // 노래 못 찾음
        }
    }
    return false; // 플레이리스트 없음
}
//플레이리스트나 노래가 없을때 예외처리 기능 추가 필요

void Library::removeSongFromPlaylist(const string& playlistName, const std::string& title, const std::string& singer) //플레이리스트에서 노래 제거
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
                    return true; // 성공
                }
            }
            return false; // 노래 없음
        }
    }
    return false; // 플레이리스트 없음
}
//플레이리스트나 노래가 없을때 예외처리 기능 추가 필요



// 노래 검색 멤버 함수 정의
vector<Library::Song> Library::searchByTitle(const string& title) //Library 클래스의 멤버 함수 정의(제목 검색)
{
    vector<Song> results;            // 검색 결과를 담을 컨테이너 생성
    for (const auto& song : SongList) // SongList 컨테이너를 순회
    {
        if (song.S_title == title)    // 노래 제목이 검색어와 일치하는지 확인
        {
            results.push_back(song); // 일치하면 결과 컨테이너에 추가
        }
    }
    return results;                // 검색 결과 반환
}
vector<Library::Song> Library::searchBySinger(const string& singer) //Library 클래스의 멤버 함수 정의(가수 검색)
{
    vector<Song> results;
    for (const auto& song : Songlist)
    {
        if (song.S_singer == singer)
        {
			results.push_back(song);
        }
    }
	return results;
}
// 검색할때 대소문자 구분 없이 검색하는 기능 추가필요
