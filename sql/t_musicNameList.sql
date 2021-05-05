drop table if exists t_musicNameList;
create table t_musicNameList(
    id int primary key auto_increment,
    cardId int,
    musicName varchar(255),
    foreign key(cardId) references t_ecard(id) on delete cascade
);