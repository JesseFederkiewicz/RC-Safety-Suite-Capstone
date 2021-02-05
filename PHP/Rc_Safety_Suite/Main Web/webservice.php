<?php
// // do requires
// // ?? function/session stuff
require_once "./inc/db.php";
require_once "./functions.php";
// // if we make it here.. connection was good

$gblData = array();   //gbl array?
$status = "Failed Ajax Call";

//////////////////////////////////////////////
//  function Done()
//  package up data
//////////////////////////////////////////////
function Done()
{
    global $gblData, $status;

    $jsonRep['data'] = $gblData;
    $jsonRep['status'] = $status;
    echo json_encode($jsonRep);
    die();
}

//Send XY data into db
if (isset($_POST['action']) && $_POST['action'] == 'web_to_car_Data')
{
    //Insert query and row response
    $query = mysqliNonQuery("UPDATE web_to_car
    SET xCoord = {$_POST['xCoord']},
        yCoord = {$_POST['yCoord']},
        timeStamp = {$_POST['timeStamp']}
        WHERE carID = {$_POST['carID']}");    

    $_SESSION['StopAccount'] = 0;

    $status = "$query : records inserted";
}

if (isset($_POST['action']) && $_POST['action'] == 'AddUser')
{
    global $status;

    //Lowercase the usn
    $userName = strtolower($mysqli->real_escape_string($_POST['user']));

    //Get real password and hash it (sql protection)
    $password = password_hash($mysqli->real_escape_string($_POST['pass']), PASSWORD_DEFAULT);
    
    //Insert query and row response
    $query = mysqliNonQuery("INSERT INTO users (username, password)
                VALUES (\"$userName\",\"$password\")");

    $status = "$query : records inserted";

    //Pack up data
    Done();
}

if (isset($_GET['action']) && $_GET['action'] == 'GetUsers')
{
    //Get all users
    $allUserData = mysqliQuery("select * from users");

    //Check if query was good
    if ($allUserData)
    {
        //Foreach through all users and add to gblArr
        foreach ($allUserData as $key => $value)
            array_push($gblData, $value);
        

        //Get rows in table
        $status = $allUserData->num_rows;

        //Pack up data
        Done();
    }

    //Bad query
    else
    {
        error_log("GET Query Failed");
        $status = "GET Query Failed";
    }
}



//Delete user
if (isset($_POST['action']) && $_POST['action'] == 'DeleteUser')
{
    global $status;
    
    //Filter userId to delete
    $userID = $mysqli->real_escape_string($_POST['userID']);

    //Dont let them delete themselves
    if ($_SESSION['userID'] != $userID)
    {
        //Deletion query
        $query = mysqliNonQuery("DELETE from users where userID like \"$userID\"");
        $status = "$query : records deleted";
    }
    
    //Dont delete yourself
    else
        $status = "Don't delete yourself :( -facebook";

    //Pack up
    Done();
}


//////////////////////////////////////////////
//  Request from car
//  Get Current data in db and send to car
//////////////////////////////////////////////
if (isset($_POST['action']) && $_POST['action'] == 'GrabXYTimeStamp')
{
    //Get all users
    //$carRequest = mysqliQuery("SELECT * from web_to_car where carID like {$_POST['carID']}");
    $carRequest = mysqliQuery("SELECT * from web_to_car where carID like 1");

    //Get row
    $row = $carRequest->fetch_assoc();

    //Check if query was good
    if ($row)
    {     
        //Send status back to car if successful
        $status = "Hello Car {$row['carID']} from webservice.php!";

        //Throw data to log
        //error_log("Car ID: {$row['carID']} X: {$row['xCoord']} Y: {$row['yCoord']} Stamp: {$row['timeStamp']}");

        //Json encode data back to car
        echo json_encode($row);
        die();  //in a hole
    }

    //Bad query
    else
    {
        error_log("POST Query Failed FROM Car {$row['carID']}");
        $status = "POST Query Failed FROM Car {$row['carID']}";
    }
}

Done();
