#ifndef DAO_H
#define DAO_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDateTime>
#include <QFont>
#include <QListWidget>
#include <QVariantList>
#include <QStringList>
#include "ecard.h"
#include "music.h"

class Dao
{
public:
    Dao();

    int getCardId();            //获取最新贺卡的id

    bool t_color_hasCardId(int cardId);    //判断数据库中是否存在了贺卡id的颜色

    bool t_font_hasCardId(int cardId);     //判断数据库中是否存在了贺卡id的字体

    bool t_music_hasCardId(int cardId);    //判断数据库中是否存在了贺卡id的音乐信息

    bool t_musicNameList_hasCardId(int cardId);  //判断数据库中是否存在了贺卡id的音乐名列表

    bool table_hasCardId(QString tableName, int cardId);  //判断表tableName中是否存在了贺卡id

    bool insertECard(const ECard& card);

    bool updateECard(const ECard& card);

    bool insertColor(int cardId, int colorId, QString colorName);

    bool updateColor(int cardId, int colorId, QString colorName);

    bool insertFont(int cardId, int pageId, const QFont& font);

    bool updateFont(int cardId, int pageId, const QFont& font);

    bool insertMusic(const Music& music);

    bool updateMusic(const Music& music);

    bool insertMusicNameList(int cardId, const QListWidget* list);

    bool deleteMusicNameList(int cardId);

    ECard getECard();

    QStringList getColorNames(int cardId);

    QFont getFont(int cardId,int pageId);

    Music getMusic(int cardId);

    QStringList getMusicNames(int cardId);

private:
    QSqlDatabase db;
    QSqlQuery query;
};

#endif // DAO_H
