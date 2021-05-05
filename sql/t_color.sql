drop table if exists t_color;
create table t_color(
    id int primary key auto_increment,
    cardId int,
    colorId int,
    colorName char(7),
    foreign key(cardId) references t_ecard(id) on delete cascade
);
/*
colorId:
    1 标题背景
    2 标题字体
    3 贺卡问候语

 */