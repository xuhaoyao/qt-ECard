drop table if exists t_ecard;
create table t_ecard(
    id int primary key auto_increment,
    title varchar(50),
    Image varchar(100),
    cardInfo varchar(255),
    date char(19),
    currentStep int,
);