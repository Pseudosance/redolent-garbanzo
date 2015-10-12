-- Now explore the dataset by running the SELECT statements that answer the following questions from your database:

--  Give me the names of all the actors in the movie 'Die Another Day'. Please also make sure actor names are in this format:  <firstname> <lastname>   (seperated by single space). 
--  Movie(id, title, year, rating, company)
--  Actor(id, last, first, sex, dob, dod)
--  MovieActor(mid, aid, role)

SELECT DISTINCT first, last
FROM Movie, Actor, MovieActor
WHERE Movie.id = MovieActor.mid AND Actor.id = MovieActor.aid

