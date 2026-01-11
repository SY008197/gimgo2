#include "Song.h"

// 생성자
Song::Song(const std::string& title, const std::string& singer, const std::string& filePath)
    : title(title), singer(singer), filePath(filePath)
{}

// getter
const std::string& Song::getTitle() const
{
    return title;
}

const std::string& Song::getSinger() const
{
    return singer;
}

const std::string& Song::getFilePath() const
{
    return filePath;
}

// 동일 곡 판별
bool Song::isSame(const std::string& t,
    const std::string& s) const
{
    return title == t && singer == s;
}