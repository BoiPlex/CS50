SELECT people.name FROM people
    JOIN stars
    ON people.id = stars.person_id
WHERE people.name != "Kevin Bacon"
    AND stars.movie_id IN (
    SELECT stars.movie_id FROM stars
    WHERE stars.person_id IN (
        SELECT people.id FROM people
        WHERE people.name = "Kevin Bacon"
        AND people.birth = 1958));