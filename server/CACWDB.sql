create table CACWUser(
	UserName varchar(20) not null,
	UserPassword varchar(20) not null,
	UserID bigint not null primary key auto_increment,
	RegCode	varchar(6),
	Sex enum("male", "female"),
	Email varchar(50),
	Address varchar(100),
	PhoneNumber char(10),
	ShortPhoneNumber varchar(10))engine=innodb, default charset=utf8;
create table CACWGroup(
	GroupID bigint primary key auto_increment,
	GroupName varchar(20) not null,
	GroupAdmin bigint null,
	GroupLeader bigint not null,
	FOREIGN KEY (GroupAdmin) REFERENCES CACWUser(UserID),
	FOREIGN KEY (GroupLeader) REFERENCES CACWUser(UserID)
	)engine=innodb, default charset=utf8;
create table CACWTask(
	UserID bigint not null,
	TaskID bigint primary key  auto_increment,
	TaskName varchar(50) not null,
	TaskDestription text,
	Starttime datetime not null,
	Endtime datetime not null,
	FOREIGN KEY (UserID) REFERENCES CACWUser(UserID),
	CONSTRAINT Task_UserID_FK FOREIGN KEY (UserID) REFERENCES CACWUser(UserID)
)engine=innodb, default charset=utf8;
