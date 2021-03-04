<?php
// mysqli Database utility file
session_start();	//possibly need to remove because called in two places

$mysqli = null; // mysqli database connection object
$mysqli_response = array(); // global response/error variable
$mysqli_status = "";				// global status variable

mysqliConnect(); // Start Connection, initiates the connection assigning $myslqi object

// Attempt connection and populate mysqli connection object, used later.
//  This will be executed once per page request, preparing possible connection object
function mysqliConnect() {

	global $mysqli, $mysqli_response, $mysqli_status; // register the globals here for use.. odd but it works that way
	
	//$mysqli=new mysqli( DB_HOST       DB_USER,    DB_PASS,    DB_NAME,  [ DB_PORT ], [ BP_UNIX_SOCKET ]);	

    // Fill me in for your connection... // 127.0.0.1
    $mysqli = new mysqli( "localhost", "coolstx5_PHPUser", "sTrOnG!P@ss", "coolstx5_Rc_Safety_Suite_Capstone" );

	if ($mysqli -> connect_error) 			// if connection failed, update response with error codes and bail out
	{
		$mysqli_response[] = 'Connect Error (' . $mysqli -> connect_errno . ') ' . $mysqli -> connect_error;
		echo json_encode($mysqli_response);
		die();		// exit(), but close up any outstanding "open" things
	}
}

// Regular Query function, will expect a resultset to bind to result object
function mysqliQuery($q)
{
	global $mysqli, $mysqli_response, $mysqli_status;// register the globals
	
	$result = false;					// initialize our result 
	if ($mysqli == null) {		// if our mysqli connection object is bad, bail out.
		$mysqli_status = "no mysqli";
		return $result;
	}
	if (!($result = $mysqli -> query($q))) // Execute query, assign result, determine success
	{
		// NOT successful, update response with error message
		$mysqli_response[] = "mysqliQuery:Error {$mysqli -> errno} : {$mysqli -> error}";
	}

	return $result;		// result object returned to process, could be false
}

// NON-Query function
//   will NOT expect a resultset to bind to result object, instead TRUE/FALSE
function mysqliNonQuery($q)
{
	global $mysqli, $mysqli_response, $mysqli_status;	// register the globals
	
	$result = 0; // initialize our result, representing number of affected rows = 0
	
	if (!($result = $mysqli->query($q))) // Execute query, assign result, determine success
	{
		// NOT successful, update response with error message 
		$mysqli_response[] = "mysqliNonQuery:Error {$mysqli -> errno} : {$mysqli -> error}";
		return -1; // error condition
	}
	return $mysqli->affected_rows; // OK, return the rows affected
}
?>