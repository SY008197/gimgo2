#pragma once
#include <string>
#include <vector>
#include "Song.h"


class Playlist
{
private:
	std::string name;                     // 플레이리스트 이름
	std::vector<Song> songs;              // 플레이리스트에 포함된 노래들

public:
	explicit Playlist(const std::string& name); // 자동	변환 방지를 위한 explicit 생성자
	// 문자열이 암묵적으로 Playlist로 변환되는 것을 방지

	const std::string& getName() const; // 플레이리스트 이름 반환
	// 플레이리스트 이름을 읽기 전용으로 반환
	

	// Playlist는 내부 상태만 관리하고 성공 / 실패 여부만 Library에 전달한다
	bool addSong(const Song& song); // 플레이리스트에 노래 추가
	bool removeSong(const std::string& title, const std::string& singer); // 플레이리스트에서 노래 제거
	//정보 처리 순서: UI입력 -> Library -> Playlist -> Library -> UI출력
};

/*	
	Playlist 클래스의 함수는 플레이리스트 내부 상태만 변경하고, 그 결과를 단순한 성공 / 실패로 알려주는 역할을 한다.
	따라서 복잡한 결과 처리는 Library 클래스에서 담당한다.
*/