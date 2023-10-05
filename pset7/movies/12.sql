SELECT title
FROM movies
WHERE id IN
    (
        SELECT movie_id
        FROM stars, people
        WHERE person_id = people.id
        AND people.name IN ('Bradley Cooper', 'Jennifer Lawrence')
        GROUP BY movie_id
        HAVING COUNT(*) > 1
    )
