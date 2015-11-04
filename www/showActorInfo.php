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
	
	// Get the id of actor
	$a_id = $_GET['id'] ? $_GET['id'] : '1';
	
	//echo "aid = $a_id" . "<br/>";
	
	// SQL Statments
	$get_actor_sql = "SELECT last, first, sex, dob, dod
					  FROM Actor
					  Where id = $a_id";
	$get_movies_sql = "SELECT title, mid, role
						FROM Movie MT, MovieActor MAT
						WHERE MAT.aid = $a_id AND MAT.mid = MT.id";
						
	// Run query
	$mActorTable = mysql_query($get_actor_sql);
	$mMovieTable = mysql_query($get_movies_sql);
	
	if($mActorTable && mysql_num_rows($mActorTable) > 0)
		$mActorData = mysql_fetch_row($mActorTable);
	
	//echo $mActorData[0] . " " . $mActorData[1] . " " . $mActorData[2] . " " . $mActorData[3] . " " . $mActorData[4];
	
?>

<link rel="stylesheet" href="bootstrap.css" type="text/css">
<link rel="stylesheet" href="style.css" type="text/css">

<!DOCTYPE html>
<body>

<a class="btn btn-success btn-lg center" href="./index.php" role="button">Go home</a>

<h1 class="center"> Show Actor Information </h1>
<br/>
<strong>
	<h2 >
		Name: <?php 
					if($mActorData)
						echo $mActorData[0] . ", " . $mActorData[1];
					else
						echo "No Name Listed";
				?>
		<br/>
		Sex: <?php 
					if($mActorData )
						echo $mActorData[2];
					else
						echo "No Gender Listed";
				?>
		<br/>
		Date Of Birth: <?php 
					if($mActorData && $mActorData[3])
						echo $mActorData[3];
					else
						echo "No DOB Listed";
				?>
		<br/>
		Date Of Death: <?php 
					if($mActorData && $mActorTable[4]){
						echo $mActorData[4];
					}
					else
						echo "Still Alive";
				?>
		<br/>
	</h2>
</strong>

<h1 class="center"> Acted In </h1>
<br/>
<strong>
	<h2 >
		<?PHP 
			if($mMovieTable && mysql_num_rows($mMovieTable) > 0){
				while($row = mysql_fetch_row($mMovieTable)){
					echo "<a href = './showMovieInfo.php?id=$row[1]'>$row[0]</a>"; 
					echo " as " . "\"" . $row[2] . "\""; 
					echo '<br/>';
				}
			}
			else
				echo "No Roles Listed";
		?>
	</h2>
</strong>

</body>
</html>