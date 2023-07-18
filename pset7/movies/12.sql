-- Write a SQL query to list the titles of all movies in which both Johnny Depp and Helena Bonham Carter starred.
SELECT title FROM movies, stars, people
WHERE name = "Johnny Depp"
AND people.id = stars.person_id
AND stars.movie_id = movies.id

INTERSECT
SELECT title FROM movies, stars, people
WHERE name = "Helena Bonham Carter"
AND people.id = stars.person_id
AND stars.movie_id = movies.id;
