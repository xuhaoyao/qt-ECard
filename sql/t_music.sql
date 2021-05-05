drop table if exists t_music;
create table t_music(
    id int primary key auto_increment,
    cardId int,
    musicPath varchar(255),
    playTime char(5),
    allTime char(5),
    playingProgress int,
    volume int,
    currentMusic int,
    foreign key(cardId) references t_ecard(id) on delete cascade
);
/*
musicPath 音乐存放路径
playTime 音乐现在播放的时长
allTime  该音乐的总时长
playingProgress 播放进度条
volume   音量
 */