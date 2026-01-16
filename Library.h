#pragma once

#include <string>
#include <vector>
#include "Song.h"
#include "Playlist.h"

// using std::string;과 같은 네임스페이스 지시문은 헤더 파일에 포함하지 않는 것이 좋음

enum class PlaylistAddResult // 플레이리스트에 노래 추가 결과 열거형
{
	ADD_SUCCESS,// 성공
	ADD_NOSONG, // 노래 없음
	ADD_NOPLAYLIST, // 플레이리스트 없음
	ADD_DUPLICATE // 중복된 노래 추가 시도 결과
};
enum class PlaylistRemoveResult // 플레이리스트에 노래 제거 결과 열거형
{
	REMOVE_SUCCESS, // 성공
	REMOVE_NOSONG, // 노래 없음
	REMOVE_NOPLAYLIST // 플레이리스트 없음
};

struct SongView   // UI에 전달하기 위한 표시용 구조체
{
    std::string title;
    std::string singer;
    std::string filePath;
};


class Library
{
private:                            
	std::vector<Song> SongList;            // 노래를 담을 컨테이너
	std::vector<Playlist> Playlists;        // 플레이리스트를 담을 컨테이너

	std::string toLower(const string& str) const; // 문자열을 소문자로 변환하는 헬퍼 함수

	// 대소문자 구분 없이 포함 여부 확인 헬퍼 함수 선언
	bool containsIgnoreCase(const string& text, const string& keyword) const; // 대소문자 구분 없이 포함 여부 확인 헬퍼 함수

	// 노래 존재 여부 확인 헬퍼 함수
    bool hasSong(const std::string& title, const std::string& singer) const; //노래가 이미 존재하는지 확인

	// 플레이리스트 찾기 헬퍼 함수
	const Playlist* findPlaylist(const std::string& playlistName) const; //플레이리스트 이름으로 플레이리스트 찾기

	// 노래 찾기 헬퍼 함수
	const Song* findSong(const std::string& title, const std::string& singer) const; //노래 제목과 가수로 노래 찾기
public:
	// 노래 추가 및 제거
    bool addSong(const std::string& title, const std::string& singer, const std::string& filePath); // 멤버 함수 선언(노래추가)
	bool removeSong(const std::string& title, const std::string& singer); // 멤버 함수 선언(노래제거)

	// 플레이리스트 관리
	void createPlaylist(const std::string& playlistName); // 플레이리스트 생성
    void deletePlaylist(const std::string& playlistName); // 플레이리스트 삭제
	PlaylistAddResult addSongToPlaylist(const std::string& playlistName, const std::string& title, const std::string& singer); // 플레이리스트에 노래 추가
    PlaylistRemoveResult removeSongFromPlaylist(const std::string& playlistName, const std::string& title, const std::string& singer); // 플레이리스트에서 노래 제거

	// UI에 전달하기 위한 함수
	std::vector<SongView> getAllSongs() const;  /* 기존에 사용하던 형태는 const를 사용해 Song에대한 직접 접근은 막을 수 있어도 UI에게 불필요한 정보까지 전달될 수 있음.
                                                    따라서 SongView 구조체를 사용해 필요한 정보만 전달 (UI가 필요한 내용은 제목, 가수, 파일 경로 3가지면 충분함.)  */ 

    // 노래 검색
	std::vector<SongView> searchByTitleView(const std::string& title) const; // 멤버 함수 선언(제목 검색)
	std::vector<SongView> searchBySingerView(const std::string& singer) const; // 멤버 함수 선언(가수 검색)
};

