SELECT people.name
FROM people, directors
WHERE directors.person_id = people.id
AND directors.movie_id IN
(
    SELECT id FROM movies, ratings WHERE movie_id = id AND rating >= 9
)
GROUP BY people.name