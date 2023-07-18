-- Write a SQL query to list the names of all people who starred in Toy Stroy.
SELECT name FROM people, stars, movies
WHERE title = "Toy Story"
AND movies.id = stars.movie_id
AND stars.person_id = people.id;
