#pragma once

#include <string>
#include <vector>
// using std::string;과 같은 네임스페이스 지시문은 헤더 파일에 포함하지 않는 것이 좋음

enum PlaylistAddResult // 플레이리스트에 노래 추가 결과 열거형
{
    Add_Success,
    Add_NoSong,
    Add_NoPlaylist
};
enum PlaylistRemoveResult // 플레이리스트에 노래 제거 결과 열거형
{
    Remove_Success,
    Remove_NoSong,
    Remove_NoPlaylist
};
struct SongView   // UI에 전달하기 위한 표시용 구조체
{
    std::string title;
    std::string singer;
    std::string filePath;
};
class Library
{
private:                            // 노래에 대한 정보는 라이브러리 클래스에서만 접근
    // 노래를 구성하는 요소를 묶기 위한 구조체 선언
    struct Song                        
    {
        std::string S_title;                // 제목
        std::string S_singer;            // 가수
        std::string S_filePath;            // 파일 경로
    };
    std::vector<Song> SongList;            // 노래를 담을 컨테이너
	// 플레이리스트를 구성하는 요소를 묶기 위한 구조체 선언
    struct Playlist
    {
        std::string P_name;                // 플레이리스트 이름
        std::vector<Song> P_songs;        // 플레이리스트에 담긴 노래들
	};
	std::vector<Playlist> Playlists;        // 플레이리스트를 담을 컨테이너
	// 헬퍼 함수 선언
	string toLower(const string& str) const; // 문자열을 소문자로 변환하는 헬퍼 함수
public:
	// 노래 추가 및 제거
    void addSong(const std::string& title, const std::string& singer, const std::string& filePath); // 멤버 함수 선언(노래추가)
	void removeSong(const std::string& title, const std::string& singer); // 멤버 함수 선언(노래제거)
	bool hasSong(const std::string& title, const std::string& singer) const; //노래가 이미 존재하는지 확인
    //예정 vector<SearchResult> searchByTitle(const string& keyword);

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

