#pragma once

#include <string>
#include <vector>

// using std::string;과 같은 네임스페이스 지시문은 헤더 파일에 포함하지 않는 것이 좋음

class Library
{
private:                            // 노래에 대한 정보는 라이브러리 클래스에서만 접근
    struct Song                        // 노래를 구성하는 요소를 묶기 위한 구조체 선언
    {
        std::string S_title;                // 제목
        std::string S_singer;            // 가수
        std::string S_filePath;            // 파일 경로
    };
    std::vector<Song> SongList;            // 노래를 담을 컨테이너

public:
    void addSong(const std::string& title, const std::string& singer, const std::string& filePath); // 멤버 함수 선언(노래추가)
    std::vector<Song> searchByTitle(const std::string& title);                // 멤버 함수 선언(제목 검색)
	std::vector<Song> searchBySinger(const std::string& singer);                // 멤버 함수 선언(가수 검색)

	const std::vector<Song>& getSongs() const; // 멤버 함수 선언(노래 목록 반환)
    // const로 묶어서 외부에서 수정 불가능하게 함. 그러면 public으로 열어놔도 안전함.
};

