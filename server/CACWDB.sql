create table CUser(
	userid bigint auto_increment,
	username varchar(20) NOT NULL,
	password varchar(15) NOT NULL,
	phonenumber varchar(20),
	shortphonenumber varchar(10),
	email varchar(20) NOT NULL,
	description text,
	sex varchar(2),
	address varchar(20),
	CONSTRAINT UQ_CUser UNIQUE(email),
	CONSTRAINT PK_CUser PRIMARY KEY(userid),
	CONSTRAINT CK_CUser CHECK(sex IN('男', '女'))
)engine=innodb, default charset=utf8;

create table CTeam(
	teamid bigint auto_increment,
	teamname varchar(30),
	description text,
	CONSTRAINT PK_CTeam PRIMARY KEY(teamid)
)engine=innodb, default charset=utf8;

create table CProject(
	projectid bigint auto_increment,
	projectname varchar(30),
	description text,
	teamid bigint,
	CONSTRAINT PK_CProject PRIMARY KEY(projectid),
	CONSTRAINT FK_CProject_CTeam FOREIGN KEY(teamid) REFERENCES CTeam(teamid) ON DELETE CASCADE ON UPDATE CASCADE
)engine=innodb, default charset=utf8;

create table CTask(
	taskid bigint auto_increment not null,
	title varchar(30) NOT NULL,
	description text,
	starttime datetime NOT NULL,
	endtime datetime NOT NULL,
	projectid bigint NOT NULL,
	CONSTRAINT PK_CTask PRIMARY KEY(taskid),
	CONSTRAINT FK_CTask_CProject FOREIGN KEY(projectid) REFERENCES CProject(projectid) ON DELETE CASCADE ON UPDATE CASCADE
)engine=innodb, default charset=utf8;

create table CUser_CTeam(
	userid bigint,
	teamid bigint,
	identity int,
	CONSTRAINT PRIMARY KEY(userid, teamid),
	CONSTRAINT UQ_CUser_CTeam CHECK(identity IN(0, 1, 2, 3)),
	CONSTRAINT FK_CU_CT_userid FOREIGN KEY(userid) REFERENCES CUser(userid) ON DELETE CASCADE ON UPDATE CASCADE,	
	CONSTRAINT FK_CU_CT_teamid FOREIGN KEY(teamid) REFERENCES CTeam(teamid) ON DELETE CASCADE ON UPDATE CASCADE
)engine=innodb, default charset=utf8;

create table CUser_CProject(
	userid bigint,
	projectid bigint,
	CONSTRAINT PRIMARY KEY(userid, projectid),
	CONSTRAINT FK_CU_CP_userid FOREIGN KEY(userid) REFERENCES CUser(userid) ON DELETE CASCADE ON UPDATE CASCADE,
	CONSTRAINT FK_CU_CP_projectid FOREIGN KEY(projectid) REFERENCES CProject(projectid) ON DELETE CASCADE ON UPDATE CASCADE
)engine=innodb, default charset=utf8;

create table CUser_CTask(
	userid bigint,
	taskid bigint,
	status int,
	CONSTRAINT PK_CUser_CTask PRIMARY KEY(userid, taskid),
	CONSTRAINT UQ_CUser_CTeam CHECK(identity IN(0, 1, 2)),
	CONSTRAINT FK_CU_CTask_userid FOREIGN KEY(userid) REFERENCES CUser(userid) ON DELETE CASCADE ON UPDATE CASCADE,	
	CONSTRAINT FK_CU_CTask_teamid FOREIGN KEY(taskid) REFERENCES CTask(taskid) ON DELETE CASCADE ON UPDATE CASCADE
)engine=innodb, default charset=utf8;
