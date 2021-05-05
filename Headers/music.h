#ifndef MUSIC_H
#define MUSIC_H
#include <QString>

class Music
{
public:
    Music();

    int getVolume() const;
    void setVolume(int value);

    int getCardId() const;
    void setCardId(int value);

    int getPlayingProgress() const;
    void setPlayingProgress(int value);

    QString getMusicPath() const;
    void setMusicPath(const QString &value);

    QString getPlayTime() const;
    void setPlayTime(const QString &value);

    QString getAllTime() const;
    void setAllTime(const QString &value);

    int getCurrentMusic() const;
    void setCurrentMusic(int value);

private:
    int cardId,playingProgress,volume,currentMusic;
    QString musicPath,playTime,allTime;
};

#endif // MUSIC_H
