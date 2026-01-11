#pragma once
#include <string>

class Song
{
private:
    std::string title;
    std::string singer;
    std::string filePath;

public:
    Song(const std::string& title, const std::string& singer, const std::string& filePath);

    // getter (읽기 전용)
    const std::string& getTitle() const;
    const std::string& getSinger() const;
    const std::string& getFilePath() const;

    // 동일 곡 판별 (제목 + 가수 기준)
    bool isSame(const std::string& title,
        const std::string& singer) const;
};