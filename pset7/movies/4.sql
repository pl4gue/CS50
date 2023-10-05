SELECT COUNT(*) FROM movies, ratings WHERE movies.id = ratings.movie_id AND ratings.rating = 10;
