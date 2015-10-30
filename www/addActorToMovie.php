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
	// Get all actors
	$get_actors_sql = "SELECT id, last, first, dob
						  FROM Actor";
	
	// Run Queries
	$all_movies = mysql_query($get_movies_sql, $db_connection);
	$all_actors = mysql_query($get_actors_sql, $db_connection);
	
	mysql_close(databaseConnection);
	
?>

<link rel="stylesheet" href="bootstrap.css" type="text/css">
<link rel="stylesheet" href="style.css" type="text/css">

<!DOCTYPE html>
<body>

<a class="btn btn-success btn-lg center" href="./index.php" role="button">Go home</a>

<h1 class="center"> Add actor-to-movie relation </h1>
<br/>
<h2>
	<form action="addActorToMovie.php" method="POST">
		Movie: <br/>
		<select name="movie">
			<?php
				while($row = mysql_fetch_row($all_movies)){
					echo "<option value=$row[0]>" . $row[1] . "</option>"; 
				}
			?>
		</select>
		<br/>
		Actor: <br/>
		<select name="actor">
			<?php
			while($row = mysql_fetch_row($all_actors)){
					echo "<option value=$row[0]>" . $row[1] . ", " . $row[2] . "</option>"; 
				}
			?>
		</select>
		<br/>
		Role: <br/>
			 <input type = "text" name = "role">
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
		$actor_id = $_POST[actor];
		$role = $_POST[role];
		echo $movie_id;
		echo '<br/>';
		echo $actor_id;
		echo '<br/>';
		echo $role;
		echo '<br/>';
		
			
		$add_check_sql = "SELECT * 
		 			FROM MovieActor 
					WHERE mid = $movie_id AND aid = $actor_id";
					
		$add_sql = "INSERT INTO MovieActor (mid, aid, role) 
					VALUES ($movie_id, $actor_id,\"$role\")
					";
					
		$update_sql = "UPDATE MovieActor 
						SET role = \"$role\"
						WHERE mid = $movie_id AND aid = $actor_id";
		
		$add_check = mysql_query($add_check_sql, $db_connection);
	
		if(mysql_num_rows($add_check) == 0){
			if (mysql_query($add_sql, $db_connection)){
				echo "added actor to movie ";
			} 
			else { 
				echo "Failed "; 
			}
		}
		else {
			if (mysql_query($update_sql, $db_connection)){
				echo "updated actor to movie relation ";
			} 
			else { 
				echo "Failed "; 
			}
			
		}
		
	}
	
	mysql_close(databaseConnection);
?>