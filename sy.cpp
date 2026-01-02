//Day1_상영
/*
* 목표설계
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

//Day2_상영
/*
라이브러리 클래스 기본 틀 구축
라이브러리 클래스 내부에 노래 구조체 설계
벡터를 이용해 노래를 담을 컨테이너 생성
*/

//Day3_상영
/*
using namespace std; -> using std::string; 로 변경
라이브러리 클래스 내부에 노래 추가 멤버 함수 선언
라이브러리 클래스 외부에 노래 추가 멤버 함수 정의
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

    void addSong(const string& title, const string& singer) // 멤버 함수 선언(노래추가)

    vector<Song> searchByTitle(const string& title)                // 멤버 함수 선언(제목 검색)
};
//Library 클래스의 멤버 함수 정의(노래 추가)
void Library::addSong(const string& title, const string& singer)  // 매개변수로 노래제목과 가수명을 받음(이때 값은 상수로 고정시키고 참조만 한다)
{
    Song newSong;                    // 새로운 노래 구조체 생성
    newSong.S_title = title;        // 제목 설정
    newSong.S_singer = singer;    // 가수 설정
    SongList.push_back(newSong);    // 컨테이너에 노래 추가
}   

vector<Song> Library::searchByTitle(const string& title) //Library 클래스의 멤버 함수 정의(제목 검색)
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
// 검색할때 대소문자 구분 없이 검색하는 기능 추가하기