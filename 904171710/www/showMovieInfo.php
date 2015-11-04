<!-- Get all data to be displayed in the HTML -->
<?php
	// Establish database connection and do basic error handling
	$db_connection = mysql_connect("localhost", "cs143", "");
	if(!$db_connection) {
		$errmsg = mysql_error($db_connection);
		print "Connection failed: $errmsg <br />";
		exit(1);
	}

	// Choose database
	mysql_select_db("CS143", $db_connection);
	
	// Get the id of movie to display info for
	// 2632 is matrix
	// 2 is the lowest number mid
	$m_id = $_GET['id'] ? $_GET['id'] : '2';
	
	// SQL Statments
		// Movie query statement
		$get_movie_sql = "SELECT title, year, rating, company
						FROM Movie
						WHERE id = $m_id ";
		// Director query Statement
		$get_directors_sql = "SELECT last, first, dob
							FROM Director DT, MovieDirector MDT
							WHERE MDT.mid = $m_id AND MDT.did = DT.id
							ORDER BY last ASC";
		// Movie Genre Query Statement
		$get_genre_sql = "SELECT genre 
						FROM MovieGenre
						WHERE mid = $m_id
						ORDER BY genre ASC";
		// Actors query statement
		$get_actors_sql = "SELECT id, last, first, role
							FROM MovieActor MAT, Actor AT
							WHERE MAT.mid = $m_id AND MAT.aid = AT.id
							ORDER BY last ASC";
		// Reviews query statement
		$get_reviews_sql = "SELECT name, time, rating, comment 
							FROM Review 
							WHERE mid = $m_id
							ORDER BY time DESC";
			// AVG Score/Num reviews
			$get_rating_sql = "SELECT COUNT(rating) as `count`, AVG(rating) as `average` 
							   FROM Review
							   WHERE mid = $m_id";
							   
	// Query Database
	$mMovieTable = mysql_query($get_movie_sql, $db_connection);
	$mDirectorTable = mysql_query($get_directors_sql, $db_connection);
	$mGenreTable = mysql_query($get_genre_sql, $db_connection);
	$mActorsTable = mysql_query($get_actors_sql, $db_connection);
	$mReviewsTable = mysql_query($get_reviews_sql, $db_connection);
	$mRatingTable = mysql_query($get_rating_sql, $db_connection);
	
	if($mMovieTable && mysql_num_rows($mMovieTable) > 0)
		$mMovieData = mysql_fetch_row($mMovieTable);
	/*if($mDirectorTable && mysql_num_rows($mDirectorTable) > 0)
		$mDirectorData = mysql_fetch_row($mMovieTable);
	if($mMovieTable && mysql_num_rows($mMovieTable) > 0)
		$mMovieData = mysql_fetch_row($mMovieTable);*/
		
		
	mysql_close($db_connection);		
?>

<!-- HTML To display data -->
<link rel="stylesheet" href="bootstrap.css" type="text/css">
<link rel="stylesheet" href="style.css" type="text/css">

<!DOCTYPE html>
<body>

<a class="btn btn-success btn-lg center" href="./index.php" role="button">Go home</a>

<h1 class="center"> Movie Information </h1>
<br/>
<strong>
	<h2 >
		Title: <?php 
					if($mMovieData && $mMovieData[0])
						echo $mMovieData[0];
					else
						echo "No Title Listed";
					if($mMovieData && $mMovieData[1])
						echo " (".$mMovieData[1].")";
						
				?>
		<br/>
		Producer: <?php 
					if($mMovieData && $mMovieData[3])
						echo $mMovieData[3];
					else
						echo "No Producer Listed";
				?>
		<br/>
		MPAA Rating: <?php 
					if($mMovieData && $mMovieData[2])
						echo $mMovieData[2];
					else
						echo "No Rating Listed";
				?>
		<br/>
		Director: <?php 
					if($mDirectorTable && mysql_num_rows($mDirectorTable) > 0){
						$not_first = false;
						while($row = mysql_fetch_row($mDirectorTable)){
							if($not_first){
								echo ", ";
							}
							echo $row[0] . ", " . $row[1] . " (" . $row[2] . ")";
							$not_first = true;
						}
						
					}
					else
						echo "No Directors Listed";
				?>
		<br/>
		Genre: <?php 
					if($mGenreTable && mysql_num_rows($mGenreTable) > 0){
						$not_first = false;
						while($row = mysql_fetch_row($mGenreTable)){
							if($not_first){
								echo ", ";
							}
							echo $row[0];
							$not_first = true;
						}
						
					}
					else
						echo "No Genres Listed";
				?>
		<br/>
	</h2>

</strong>
	
<h1 class="center"> Actors in this movie </h1>
<br/>
<strong>
	<h2 >
		<?PHP 
			if($mActorsTable && mysql_num_rows($mActorsTable) > 0)
			{
				while($row = mysql_fetch_row($mActorsTable)){
					echo "<a href = './showActorInfo.php?id=$row[0]'>$row[1], $row[2]</a>";
					echo " as \"" . $row[3] . "\"";
					echo '<br/>';
				}
			}
			else
				echo "No Actors Listed";
		?>
	</h2>
</strong>

<h1 class="center"> User Reviews </h1>
<br/>
<strong>
	<h2 >
		<?PHP 
			if($mRatingTable && mysql_num_rows($mRatingTable) > 0)
			{
				$row = mysql_fetch_row($mRatingTable);
				if($row[0] == 0)
					echo "Average Score: Be the first to rate this movie!";
				else
					echo "Average Score: " . $row[1] . "/5 by " . $row[0] . " reviewer(s)";
			}
			else
				echo "Average Score: Be the first to rate this movie!";
			
			echo '<br/>';
			echo "<a href = './addComments.php?id=$m_id'>Review This Movie!</a>";
			echo '<br/>';
			echo 'User comments: ' . '<br/>' . '<br/>';
			if($mReviewsTable && mysql_num_rows($mReviewsTable) > 0 ){
				while($row = mysql_fetch_row($mReviewsTable)){
					echo "On " . $row[1] . " " . '<br/>' . $row[0] . " scored this movie " . $row[2] . "/5 " . "and said: ";
					echo '<br/>';
					echo $row[3]; 
					echo '<br/> <br/>';
				}
			}	
			else{
				echo "No Comments";
			}
		?>
	</h2>
</strong>

</body>
</html>