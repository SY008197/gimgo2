//Day1상영
/*
저장된 노래
검색 (제목 or 가수명으로 찾기)
전체 목록

재생목록
저장된 재생 목록이 있으면 출력
없으면 "저장된 재생목록이 없습니다."같은 안내문 출력

재생목록 추가
이름설정
추가하기
저장하기

프로그램 종료
*/

//Day2상영
/*
라이브러리 (관점: 책임자)
전체 곡 데이터 관리
곡 추가 / 검색 / 목록 제공
즉, 노래에 대한 정보와 목록 관리 / 파일 경로 탐색, mp3파일 열기 등의 작업 X

콘솔UI (관점:사용자)
메뉴 출력
사용자 선택 처리
라이브러리에 명령 전달
*/

#include <iostream>
#include <string>
#include <vector>
using std::string;                  // string 타입 사용(namespace보다 이런식으로 습관화하기)
class Library
{
private:                            // 노래에 대한 정보는 라이브러리 클래스에서만 접근
    struct Song                        // 노래를 구성하는 요소를 묶기 위한 구조체 선언
    {
        string S_title;                // 제목
        string S_singer;            // 가수
    };
    vector<Song> SongList;            // 노래를 담을 컨테이너

	void addSong(const string& title, const string& singer); // 노래 추가 함수
};

// SongList.push_back({ title, singer });         // 노래 추가