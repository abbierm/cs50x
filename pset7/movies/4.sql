-- Write a SQL query to determine the numner of movies with an IMBd rating of 10.0.
SELECT COUNT(movie_id) FROM ratings WHERE rating = 10.0;
