CREATE TABLE Movie(
	id INT, title VARCHAR(100), year INT, rating VARCHAR(10), company VARCHAR(50), 
	PRIMARY KEY(id) -- Each movie id must be unique
	) ENGINE = INNODB;
CREATE TABLE Actor(
	id INT, last VARCHAR(20), first VARCHAR(20), sex VARCHAR(6), dob DATE, dod DATE, 
	PRIMARY KEY(id), -- Each actor id must be unique
	CHECK (dob < dod), -- The actor's date of death must be AFTER the date of birth
	CHECK (sex = 'Male' OR sex = 'Female')
	) ENGINE = INNODB;
CREATE TABLE Director(
	id INT, last VARCHAR(20), first VARCHAR(20), dob DATE, dod DATE, 
	PRIMARY KEY(id), -- Each director id must be unique
	CHECK (dob < dod) -- The director's date of death must be AFTER the date of birth
	) ENGINE = INNODB;
CREATE TABLE MovieGenre(
	mid INT, genre VARCHAR(20), FOREIGN KEY(mid) references Movie(id) -- movie ID must be same as that in Movie 
	) ENGINE = INNODB;
CREATE TABLE MovieDirector(
	mid INT, did INT, 
	FOREIGN KEY(mid) references Movie(id), -- movie ID must be same as that in Movie
	FOREIGN KEY(did) references Director(id) -- director ID must be same as that in Director
	) ENGINE = INNODB;
CREATE TABLE MovieActor(
	mid INT, aid INT, role VARCHAR(50),
	FOREIGN KEY(mid) references Movie(id), -- movie ID must be same as that in Movie
	FOREIGN KEY(aid) references Actor(id) -- actor ID must be same as that in Actor
	) ENGINE = INNODB;
CREATE TABLE Review(
	name VARCHAR(20), time TIMESTAMP, mid INT, rating INT, comment VARCHAR(500),
	FOREIGN KEY(mid) references Movie(id) -- movie ID must be same as that in Movie
	) ENGINE = INNODB; 
CREATE TABLE MaxPersonID(
	id INT
	) ENGINE = INNODB;
CREATE TABLE MaxMovieID(
	id INT
	) ENGINE = INNODB;

INSERT INTO MaxPersonID VALUES(69000);
INSERT INTO MaxMovieID VALUES(4750);
