drop table if exists t_font;
create table t_font(
    id int primary key auto_increment,
    cardId int,
    pageId int,
    Family varchar(30),
    PixelSize int,
    Italic TINYINT(1),
    PointSize int,
    Overline TINYINT(1),
    Underline TINYINT(1),
    StrikeOut TINYINT(1),
    foreign key(cardId) references t_ecard(id) on delete cascade
);
/*
pageId
    两个值
    3 第三页的字体
    4 第四页的字体
 */