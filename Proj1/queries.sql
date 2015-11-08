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
) alias; 		-- in testing, comes up as 4824 actors

-- Another query that you came up with
-- The titles and release years of all comedy movies released from 1999 to 2002 displayed in descending order by release year
SELECT title, year
FROM Movie, MovieGenre
WHERE Movie.id = MovieGenre.mid AND Movie.rating = 'PG-13' AND MovieGenre.genre = 'Comedy' AND (Movie.year >= 1999 AND Movie.year <= 2002)
ORDER BY Movie.year DESC; 