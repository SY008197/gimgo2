#include "Library.h"

using std::string;      // string 타입 사용(namespace보다 이런식으로 습관화하기)
using std::vector;      // vector 타입 사용(네임스페이스 지시문은 헤더가 아닌 cpp파일에서 사용 권장)
void Library::addSong(const string& title, const string& singer, const string& filePath)  // 매개변수로 노래제목, 가수명, 파일 경로를 받음(이때 값은 상수로 고정시키고 참조만 한다)
{
    Song newSong;                    // 새로운 노래 구조체 생성
    newSong.S_title = title;        // 제목 설정
    newSong.S_singer = singer;    // 가수 설정
    newSong.S_filePath = filePath;        // 파일 경로 설정
    SongList.push_back(newSong);    // 컨테이너에 노래 추가
}   

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
    for (const auto& song : SongList)
    {
        if (song.S_singer == singer)
        {
			results.push_back(song);
        }
    }
	return results;
}

const vector<Library::Song>& Library::getSongs() const //Library 클래스의 멤버 함수 정의(노래 목록 반환)
{
    return SongList; // 노래 목록 컨테이너를 상수 참조로 반환
}
// 사용자가 입력한 노래 제목이나 가수 이름과 일치하는 노래를 검색하는 기능을 제공함. ( const로 묶었기 때문에 Library 내부에서만 접근 가능함. )


// 검색할때 대소문자 구분 없이 검색하는 기능 추가필요
