-- All Actor names in movie Die Another Day
SELECT CONCAT(first, " ", last)
FROM Movie, Actor, MovieActor
WHERE Movie.title = 'Die Another Day' AND Movie.id = MovieActor.mid AND MovieActor.aid = Actor.id;

-- Count of all Actors who acted in multiple movies
SELECT COUNT(*)
FROM (
	SELECT aid
	FROM MovieActor
	GROUP BY aid
	HAVING COUNT(*) > 1 -- could be >= 2 as well
); 

-- Another query that you came up with
-- All PG-13 comedy movies released after 1996 displayed in ascending order by release year
SELECT rating, genre, year
FROM Movie, MovieGenre
WHERE Movie.id = MovieGenre.mid AND Movie.rating = 'PG-13' AND MovieGenre = 'Comedy' AND Movie.year > 1996
ORDER BY Movie.year DESC; 