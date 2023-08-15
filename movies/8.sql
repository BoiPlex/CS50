SELECT people.name FROM people
    JOIN stars
    ON people.id = stars.person_id
WHERE stars.movie_id IN (
    SELECT movies.id FROM movies
    WHERE movies.title = "Toy Story");