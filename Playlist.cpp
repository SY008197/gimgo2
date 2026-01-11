#include "Playlist.h"


// 생성자
Playlist::Playlist(const std::string& name) 
    : name(name) {}

// 플레이리스트 이름 반환
const std::string& Playlist::getName() const 
{
    return name;
}

// 노래 추가
bool Playlist::addSong(const Song& song) 
{
    for (const auto& s : songs)
    {
        if (s.isSame(song.getTitle(), song.getSinger()))
        {
            return false; // 이미 존재
        }
    }
    songs.push_back(song);
	return true;  // 성공
}
/* 
	 false 반환:
        - 이미 플레이리스트에 존재하는 노래
    true 반환:
        - 플레이리스트에 정상적으로 노래가 추가됨

    이후 결과의 의미 해석과 UI 대응은 Library가 담당한다.
*/

// 노래 제거
bool Playlist::removeSong(const std::string& title, const std::string& singer) 
{
	for (auto it = songs.begin(); it != songs.end(); ++it) // 노래 찾기
    {
		if (it->isSame(title, singer)) //노래 제목과 가수가 일치하는지 확인
        {
            songs.erase(it);
			return true; // 성공
        }
    }
	return false; // 노래 없음
}