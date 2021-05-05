#include "Dao.h"

Dao::Dao()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("333");
    db.setDatabaseName("ECard");
    db.open();
    query = QSqlQuery(db);
}

int Dao::getCardId(){
    QString sql = QString("select max(id) from t_ecard");
    query.exec(sql);
    if(query.next())
        return query.value(0).toInt();
}

bool Dao::t_color_hasCardId(int cardId){
    QString sql = QString("select id from t_color where cardId = %1").arg(cardId);
    query.exec(sql);
    return query.next();
}

bool Dao::t_font_hasCardId(int cardId)
{
    QString sql = QString("select id from t_font where cardId = %1").arg(cardId);
    query.exec(sql);
    return query.next();
}

bool Dao::t_music_hasCardId(int cardId)
{
    QString sql = QString("select id from t_music where cardId = %1").arg(cardId);
    query.exec(sql);
    return query.next();
}

bool Dao::t_musicNameList_hasCardId(int cardId)
{
    QString sql = QString("select id from t_musicNameList where cardId = %1").arg(cardId);
    query.exec(sql);
    return query.next();
}

bool Dao::table_hasCardId(QString tableName, int cardId)
{
    QString sql = QString("select cardId from %1 where cardId = %2").arg(tableName).arg(cardId);
    query.exec(sql);
    return query.next();
}

bool Dao::insertECard(const ECard &card){
    QString sql = QString("insert into t_ecard(title,Image,cardInfo,date,currentStep) values('%1','%2','%3','%4',%5)")
            .arg(card.getTitle()).arg(card.getImage()).arg(card.getCardInfo()).arg(card.getDate()).arg(card.getCurrentStep());
    return query.exec(sql);
}

bool Dao::updateECard(const ECard& card){
    QString sql = QString("update t_ecard set title = '%1',Image = '%2',cardInfo = '%3',date = '%4',currentStep = %5 where id = %6")
            .arg(card.getTitle()).arg(card.getImage()).arg(card.getCardInfo()).arg(card.getDate()).arg(card.getCurrentStep()).arg(card.getId());
    return query.exec(sql);
}

bool Dao::insertColor(int cardId, int colorId, QString colorName){
    QString sql = QString("insert into t_color(cardId, colorId, colorName) values(%1,%2,'%3')").arg(cardId).arg(colorId).arg(colorName);
    return query.exec(sql);
}

bool Dao::updateColor(int cardId, int colorId, QString colorName){
    QString sql = QString("update t_color set colorName = '%1' where cardId = %2 and colorId = %3").arg(colorName).arg(cardId).arg(colorId);
    return query.exec(sql);
}

bool Dao::insertFont(int cardId, int pageId, const QFont &font)
{
    QString sql = QString("insert into t_font(cardId,pageId,Family,PixelSize,Italic,PointSize,Overline,Underline,StrikeOut) "
                          "values(%1,%2,'%3',%4,%5,%6,%7,%8,%9)").arg(cardId).arg(pageId).arg(font.family())
                            .arg(font.pixelSize()).arg(font.italic()).arg(font.pointSize()).arg(font.overline())
                            .arg(font.underline()).arg(font.strikeOut());
    return query.exec(sql);
}

bool Dao::updateFont(int cardId, int pageId, const QFont &font)
{
    QString sql = QString("update t_font set Family = '%1',PixelSize = %2,Italic = %3,PointSize = %4,Overline = %5,"
                          "Underline = %6,StrikeOut = %7 where cardId = %8 and pageId = %9").arg(font.family())
                            .arg(font.pixelSize()).arg(font.italic()).arg(font.pointSize()).arg(font.overline())
                            .arg(font.underline()).arg(font.strikeOut()).arg(cardId).arg(pageId);
    return query.exec(sql);
}

bool Dao::insertMusic(const Music &music)
{
    QString sql = QString("insert into t_music(cardId,musicPath,playTime,allTime,playingProgress,volume,currentMusic) values(%1,'%2','%3','%4',%5,%6,%7)")
                        .arg(music.getCardId()).arg(music.getMusicPath()).arg(music.getPlayTime()).arg(music.getAllTime())
                        .arg(music.getPlayingProgress()).arg(music.getVolume()).arg(music.getCurrentMusic());
    return query.exec(sql);
}

bool Dao::updateMusic(const Music &music)
{
    QString sql = QString("update t_music set musicPath = '%1',playTime = '%2', allTime = '%3', playingProgress = %4, volume = %5, currentMusic = %6 where cardId = %7")
                    .arg(music.getMusicPath()).arg(music.getPlayTime()).arg(music.getAllTime()).arg(music.getPlayingProgress())
                    .arg(music.getVolume()).arg(music.getCurrentMusic()).arg(music.getCardId());
    return query.exec(sql);
}

bool Dao::insertMusicNameList(int cardId, const QListWidget* list)
{
    query.prepare("insert into t_musicNameList(cardId,musicName) values(?,?)");
    int len = list->count();
    QVariantList Id,musicName;
    for(int i = 0;i < len;i++){
        Id << cardId;
        musicName << list->item(i)->text();
    }
    query.addBindValue(Id);
    query.addBindValue(musicName);
    return query.execBatch();
}

bool Dao::deleteMusicNameList(int cardId)
{
    QString sql = QString("delete from t_musicNameList where cardId = %1").arg(cardId);
    return query.exec(sql);
}

ECard Dao::getECard()
{
    ECard card;
    QString sql = QString("select * from t_ecard where id = (select max(id) from t_ecard)");
    query.exec(sql);
    if (query.next()){
        card.setId(query.value(0).toInt());
        card.setTitle(query.value(1).toString());
        card.setImage(query.value(2).toString());
        card.setCardInfo(query.value(3).toString());
        card.setDate(query.value(4).toString());
        card.setCurrentStep(query.value(5).toInt());
    }
    return card;
}

QStringList Dao::getColorNames(int cardId)
{
    QString sql = QString("select colorName from t_color where cardId = %1").arg(cardId);
    query.exec(sql);
    QStringList list;
    while(query.next()){
        list << query.value(0).toString();
    }
    return list;
}

QFont Dao::getFont(int cardId, int pageId)
{
    QFont font;
    QString sql = QString("select * from t_font where cardId = %1 and pageId = %2").arg(cardId).arg(pageId);
    query.exec(sql);
    int cnt = 3;
    if(query.next()){
        font.setFamily(query.value(cnt++).toString());
        font.setPixelSize(query.value(cnt++).toInt());
        font.setItalic(query.value(cnt++).toBool());
        font.setPointSize(query.value(cnt++).toInt());
        font.setOverline(query.value(cnt++).toBool());
        font.setUnderline(query.value(cnt++).toBool());
        font.setStrikeOut(query.value(cnt).toBool());
    }
    return font;
}

Music Dao::getMusic(int cardId)
{
    Music music;
    QString sql = QString("select * from t_music where cardId = %1").arg(cardId);
    query.exec(sql);
    int cnt = 1;
    if(query.next()){
        music.setCardId(query.value(cnt++).toInt());
        music.setMusicPath(query.value(cnt++).toString());
        music.setPlayTime(query.value(cnt++).toString());
        music.setAllTime(query.value(cnt++).toString());
        music.setPlayingProgress(query.value(cnt++).toInt());
        music.setVolume(query.value(cnt++).toInt());
        music.setCurrentMusic(query.value(cnt++).toInt());
    }
    return music;
}

QStringList Dao::getMusicNames(int cardId)
{
    QStringList list;
    QString sql = QString("select musicName from t_musicNameList where cardId = %1").arg(cardId);
    query.exec(sql);
    while (query.next()){
        list << query.value(0).toString();
    }
    return list;
}
