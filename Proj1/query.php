<!DOCTYPE html>
<html>
	<head>
		<title>
			CS143 Project 1B
		</title>

		<style>
			table, th, td {
			    border: 1px solid black;
			    border-collapse: collapse;
			}
			th, td {
			    padding: 5px;
			    text-align: left;
			}
		</style>	

	</head>
	<body>
		<p> Type an SQL Query into the following box </p>
		<p>
			<form method="get" action="<?php echo htmlspecialchars($_SERVER["PHP_SELF"]);?>" >
				<textarea name="query" cols="60" rows="8"></textarea><br />
				<input type="submit" value="Submit">
			</form>
		</p>

		<?php
		{
			if(empty($_GET["query"])){
				// No input
				echo 'No Input';
			}
			else{
				// Get user input
				$user_query = $_GET["query"];

				// Establish database connection and do basic error handling
				$db_connection = mysql_connect("localhost", "cs143", "");
				if(!$db_connection) {
				    $errmsg = mysql_error($db_connection);
				    print "Connection failed: $errmsg <br />";
				    exit(1);
				}

				// Choose database
				mysql_select_db("CS143", $db_connection);

				// Perform query on database
				$rs = mysql_query($user_query, $db_connection);
				
				// Check if query failed, rs is false on failure
				if(!$rs){
					die('Invalid query: ' . mysql_error());
				}

				// Create Table 
				createTable($rs);

				// End database connection
				mysql_close($db_connection);
			}

			}

			function createTable($query_result){
				// Create basic table
				echo '<table style="width:100%">';
				echo '<caption> Query Results </caption>';

				$data = array();
				while($row = mysql_fetch_assoc($query_result))
				{
				    $data[] = $row;
				}

				// Setup Columns
  				$column_names = array_keys(reset($data));
  				echo '<tr>';
				foreach($column_names as $col)
		        {
		          echo '<th>'.$col.'</th>';
		        }
		        echo '</tr>';

				//Setup rows
		       foreach($data as $row)
		       {
			        echo '<tr>';
			        foreach($column_names as $col)
			        {
			            if($row[$col] == NULL){
			            	echo '<td>NULL</td>';
			            }
			            else
			            	echo '<td>'.$row[$col].'</td>';
			        }
			        echo '</tr>';
		       }

		       // Finish table
		       echo '</table>';
			}

		?>
	
	</body>
</html>
