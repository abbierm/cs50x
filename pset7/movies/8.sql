/*
Write a SQL query to list the names of all people who starred in Toy Story.

    -Your query should output a table with a single column for the name of each
        person.
    - You may assume that there is only one movie in the database with the
        title Toy Story.
*/
SELECT name FROM people, stars, movies
WHERE title = "Toy Story"
AND movies.id = stars.movie_id
AND stars.person_id = people.id;
