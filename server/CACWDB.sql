create table CACWGroup(
	GroupName varchar(20) not null,
	groupid set("1","2","3","4","5","6","7", "8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32") not null primary key,
	groupAdmin bigint not null)engine=innodb, default charset=utf8;
create table CACWUser(
	groupid bigint not null,
	UserName varchar(20) not null,
	UserPassword varchar(20) not null,
	Userid bigint not null primary key auto_increment,
	MyGroup set("1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32") not null,
	sex enum("male", "female"),
	email varchar(50),
	address varchar(100),
	PhoneNumber char(10),
	shortPhoneNumber varchar(10)
)engine=innodb, default charset=utf8;
create table CACWTask(
	Userid bigint not null,
	TaskName varchar(50) not null primary key,
	TaskDestription text,
	starttime datetime not null,
	endtime datetime not null
)engine=innodb, default charset=utf8;
