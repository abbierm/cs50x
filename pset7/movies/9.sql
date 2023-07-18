-- Write a SQL query to list the names of all people who starred in a movie released in 2004, ordered by birth year
SELECT DISTINCT name FROM people, stars, movies
WHERE year = 2004
AND movies.id = stars.movie_id
AND stars.person_id = people.id
ORDER BY birth;
