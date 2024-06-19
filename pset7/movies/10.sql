/*
Write a SQL query to list the names of all people who have directed a movie that received a rating of at least 9.0.

    - Your query should output a table with a single column for the name of
        each person.
    - If a person directed more than one movie that received a rating of at
        least 9.0, they should only appear in your results once.
*/
SELECT DISTINCT name FROM people, ratings, directors, movies
WHERE movies.id = ratings.movie_id
AND rating >= 9.0
AND movies.id = directors.movie_id
AND directors.person_id = people.id;
