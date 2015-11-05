<?php
	// Establish database connection and do basic error handling
	$db_connection = mysql_connect("localhost", "cs143", "");
	if(!$db_connection) {
		$errmsg = mysql_error($db_connection);
		print "Connection failed: $errmsg <br />";
		exit(1);
	}
	
	// Get Movie
	$m_id = $_GET['id'] ? $_GET['id'] : '2';
		// Movie query statement
		//$get_movie_sql = "SELECT title, year
		//				FROM Movie
		//				WHERE id = $m_id ";

	// Choose database
	mysql_select_db("CS143", $db_connection);
	
	// Get All Movies
	$get_movies_sql = "SELECT id, title, year
					  FROM Movie";	
	// Run Queries
	$all_movies = mysql_query($get_movies_sql, $db_connection);
	//$preset_movie = mysql_query()
		
	mysql_close(databaseConnection);
	
?>

<link rel="stylesheet" href="bootstrap.css" type="text/css">
<link rel="stylesheet" href="style.css" type="text/css">

<!DOCTYPE html>
<body>

<a class="btn btn-success btn-lg center" href="./index.php" role="button">Go home</a>

<h1 class="center"> Add Comments </h1>
<br/>
<h2>
	<form action="addComments.php" method="POST">
		Movie: <br/>
		<select name="movie">
			<?php
				while($row = mysql_fetch_row($all_movies)){
					if($row[0] == $m_id){
						echo "<option value=$row[0] selected='selected'>" . $row[1] . "</option>"; 
					}
					else
						echo "<option value=$row[0]>" . $row[1] . "</option>"; 
				}
			?>
		</select>
		<br/>
		Your Name: <br/>
				<input type = "text" name = "name" value = "Mr. Anonymous">
				<br/>
		Rating: <br/>
				<select name = "rating">
					<option value=5>5 - Excellent</option>
					<option value=4>4 - Good</option>
					<option value=3>3 - Its ok~</option>
					<option value=2>2 - Not worth</option>
					<option value=1>1 - I hate it</option>
				</select>
				<br/>
		Comments: <br/>
			<textarea name="comment" cols="60" rows="8"> </textarea>
		<br/> <br/>
		<input type = "submit" name="submit" value = "Rate it!">
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
		$name = $_POST[name];
		$rating = $_POST[rating];
		$comment = $_POST[comment];
		echo $movie_id;
		echo '<br/>';
		echo $name;
		echo '<br/>';
		echo $rating;
		echo '<br/>';
		echo $comment;
		echo '<br/>';
		
		$time = time();			
		$mysqldate = date( 'Y-m-d H:i:s', $time );
		$date = date("Y-m-d", $time);
		echo $date;
		
		$add_sql = "INSERT INTO Review (name, time, mid, rating, comment) 
					VALUES (\"$name\", '$mysqldate', $movie_id, $rating, \"$comment\")";
	
		if (mysql_query($add_sql, $db_connection)){
			echo "added review to movie ";
		} 
		else { 
			echo "Failed "; 
		}		
	}
	
	mysql_close(databaseConnection);
?>