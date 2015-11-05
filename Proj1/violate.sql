-- *********** Three primary key constraints ***********

-- Every Movie ID has to be unique
-- The error we force is that we are trying to add a movie with id 3895, but that id is already in Movie
-- ERROR 1062 (23000): Duplicate entry '3895' for key 'PRIMARY'
INSERT INTO Movie(id, title, year, rating, company)
VALUES(3895, 'Example Title', 2015, 'PG-13', 'Example Company');

-- Every Actor ID has to be unique
-- The error we force is that we are trying to add an actor with id 67635, but that id is already in Actor
-- ERROR 1062 (23000): Duplicate entry '68635' for key 'PRIMARY'
INSERT INTO Actor(id, last, first, sex, dob, dod)
VALUES(68635, 'ExampleLastName', 'ExampleFirstName', 'Male', '1978-04-01', '1995-03-10');

-- Every Director ID has to be unique
-- The error we force is that we are trying to add a director with id 68626, but that id is already in Director
-- ERROR 1062 (23000): Duplicate entry '68626' for key 'PRIMARY'
INSERT INTO Director(id, last, first, dob, dod)
VALUES(68626, 'ExampleLastName', 'ExampleLastName', '1940-03-16', '1998-09-16');

-- *********** Six referential integrity constraints ***********

-- MovieGenre: mid is a foreign key of Movie.id
-- Violation of constraint: the mid provided for MovieGenre doesn't exist in the Movie table
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails 
-- (`CS143 . MovieGenre`, CONSTRAINT `MovieGenre_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`))
INSERT INTO MovieGenre(mid, genre)
VALUES(909090, 'Comedy');

-- MovieDirector: mid is a foreign key of Movie.id
-- Violation of constraint: the mid provided for MovieDirector doesn't exist in the Movie table
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails 
-- (`CS143 . MovieDirector`, CONSTRAINT `MovieDirector_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`))
INSERT INTO MovieDirector(mid, did)
VALUES(909091, 68626);

-- MovieDirector: did is a foreign key of Director.id
-- Violation of constraint: the did provided for MovieDirector doesn't exist in the Director table
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails 
-- (`CS143 . MovieDirector`, CONSTRAINT `MovieDirector_ibfk_2` FOREIGN KEY (`did`) REFERENCES `Director` (`id`))
INSERT INTO MovieDirector(mid, did)
VALUES(3895, 909092);

-- MovieActor: mid is a foreign key of Movie.id
-- Violation of constraint: the mid provided for MovieActor doesn't exist in the Movie table
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails 
-- (`CS143 . MovieActor`, CONSTRAINT `MovieActor_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`))
INSERT INTO MovieActor(mid, aid, role)
VALUES(909093, 68628, 'ExampleRole');

-- MovieActor: aid is a foreign key of Actor.id
-- Violation of constraint: the aid provided for MovieActor doesn't exist in the Actor table
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails 
-- (`CS143 . MovieActor`, CONSTRAINT `MovieActor_ibfk_2` FOREIGN KEY (`aid`) REFERENCES `Actor` (`id`))
INSERT INTO MovieActor(mid, aid, role)
VALUES(3895, 909094, 'ExampleRole');

-- Review: mid is a foreign key of Movie.id
-- Violation of constraint: the mid provided for Review doesn't exist in the Movie table
-- ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails 
-- (`CS143 . Review`, CONSTRAINT `Review_ibfk_1` FOREIGN KEY (`mid`) REFERENCES `Movie` (`id`))
INSERT INTO Review(name, time, mid, rating, comment)
VALUES('ExampleName', '2015-10-13 11:04:00', 909095, 4, 'ExampleComment');

-- *********** Three CHECK constraints ***********

-- Every actor has to have a date of death that is after his or her date of birth
-- Violation of check constraint: the dod is before the dob, which is impossible in reality
INSERT INTO Actor(id, last, first, sex, dob, dod)
VALUES(909096, 'ExampleLastName', 'ExampleFirstName', 'Male', '2015-10-13', '2012-12-12');

-- Every director has to have a date of death that is after his or her date of birth
-- Violation of check constraint: the dod is before the dob, which is impossible in reality
INSERT INTO Director(id, last, first, dob, dod)
VALUES(909097, 'ExampleLastName', 'ExampleFirstName', '2015-10-12', '2012-12-13');

-- Every actor is either male or female
-- Violation of check constraint: the sex given is neither 'Male' nor 'Female'
INSERT INTO Actor(id, last, first, sex, dob, dod)
VALUES(909098, 'ExampleLastName', 'ExampleFirstName', 'Neither', '1993-01-01', '2014-01-01');