#pragma once

#include <string>
#include <vector>

using std::string;                  // string 타입 사용(namespace보다 이런식으로 습관화하기)
using std::vector;                  // vector 타입 사용
class Library
{
private:                            // 노래에 대한 정보는 라이브러리 클래스에서만 접근
    struct Song                        // 노래를 구성하는 요소를 묶기 위한 구조체 선언
    {
        string S_title;                // 제목
        string S_singer;            // 가수
        string S_filePath;            // 파일 경로
    };
    vector<Song> SongList;            // 노래를 담을 컨테이너

public:
    void addSong(const string& title, const string& singer, const string& filePath); // 멤버 함수 선언(노래추가)
    vector<Song> searchByTitle(const string& title);                // 멤버 함수 선언(제목 검색)
};

