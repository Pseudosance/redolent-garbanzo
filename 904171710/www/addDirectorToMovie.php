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
	
	// Get All Movies
	$get_movies_sql = "SELECT id, title, year
					  FROM Movie";
	// Get all Directors
	$get_directors_sql = "SELECT id, last, first, dob
						  FROM Director";
	
	// Run Queries
	$all_movies = mysql_query($get_movies_sql, $db_connection);
	$all_directors = mysql_query($get_directors_sql, $db_connection);
	
	mysql_close(databaseConnection);
	
?>

<link rel="stylesheet" href="bootstrap.css" type="text/css">
<link rel="stylesheet" href="style.css" type="text/css">

<!DOCTYPE html>
<body>

<a class="btn btn-success btn-lg center" href="./index.php" role="button">Go home</a>

<h1 class="center"> Add director-to-movie relation </h1>
<br/>
<h2>
	<form action="addDirectorToMovie.php" method="POST">
		Movie: <br/>
		<select name="movie">
			<?php
				while($row = mysql_fetch_row($all_movies)){
					echo "<option value=$row[0]>" . $row[1] . "</option>"; 
				}
			?>
		</select>
		<br/>
		Director: <br/>
		<select name="director">
			<?php
			while($row = mysql_fetch_row($all_directors)){
					echo "<option value=$row[0]>" . $row[1] . ", " . $row[2] . "</option>"; 
				}
			?>
		</select>
		<br/> <br/>
		<input type = "submit" name="submit">
	</form>
</h2>

</body>
</html>

<?PHP
	// Establish database connection and do basic error handling
	$db_connection = mysql_connect("localhost", "cs143", "");
	if(!$db_connection) {
		$errmsg = mysql_error($db_connection);
		print "Connection failed: $errmsg <br />";
		exit(1);
	}

	// Choose database
	mysql_select_db("CS143", $db_connection);
	
	// Submit clicked:
	if (isset($_POST["submit"])) {
		$movie_id = $_POST[movie];
		$director_id = $_POST[director];
		echo $movie_id;
		echo '<br/>';
		echo $director_id;
		echo '<br/>';
			
		$add_check_sql = "SELECT * 
		 			FROM MovieDirector 
					WHERE mid = $movie_id AND did = $director_id";
					
		$add_sql = "INSERT INTO MovieDirector (mid, did) 
					VALUES ($movie_id, $director_id)";
		
		$add_check = mysql_query($add_check_sql, $db_connection);
	
		if(mysql_num_rows($add_check) == 0){
			if (mysql_query($add_sql, $db_connection)){
				echo "added director to movie ";
			} 
			else { 
				echo "Failed "; 
			}
		}
		else 
			echo "This Director Movie relation already exists";
		
	}
	
	mysql_close(databaseConnection);
?>