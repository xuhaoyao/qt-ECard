#include "music.h"

Music::Music()
{

}

int Music::getVolume() const
{
    return volume;
}

void Music::setVolume(int value)
{
    volume = value;
}

int Music::getCardId() const
{
    return cardId;
}

void Music::setCardId(int value)
{
    cardId = value;
}

int Music::getPlayingProgress() const
{
    return playingProgress;
}

void Music::setPlayingProgress(int value)
{
    playingProgress = value;
}

QString Music::getMusicPath() const
{
    return musicPath;
}

void Music::setMusicPath(const QString &value)
{
    musicPath = value;
}

QString Music::getPlayTime() const
{
    return playTime;
}

void Music::setPlayTime(const QString &value)
{
    playTime = value;
}

QString Music::getAllTime() const
{
    return allTime;
}

void Music::setAllTime(const QString &value)
{
    allTime = value;
}

int Music::getCurrentMusic() const
{
    return currentMusic;
}

void Music::setCurrentMusic(int value)
{
    currentMusic = value;
}
