SELECT people.name
FROM people
JOIN stars ON stars.person_id = people.id
JOIN movies ON stars.movie_id = movies.id WHERE year = 2004 GROUP BY people.name ORDER BY people.birth
