<?php

/*
 * File:    webservices.php
 * Authors: Jesse Federkiewicz & Timothy Hachey
 * TLDR:    Proctors connection between car, webservices, and db
*/

// // do requires
// // ?? function/session stuff
require_once "./inc/db.php";
require_once "./functions.php";
// // if we make it here.. connection was good

$gblData = array();                 // gbl data aarr
$status = "Failed Ajax Call";       // init status message

/////////////////////////////////////////
// function Done()
//
// package up data
//
// Returns: rows effected
/////////////////////////////////////////
function Done()
{
    global $gblData, $status;

    $jsonRep['data'] = $gblData;
    $jsonRep['status'] = $status;
    echo json_encode($jsonRep);
    die();
}

// if user wants to delete just one drive
if (isset($_POST['action']) && $_POST['action'] == 'deleteDrive')
{
    // santitize data
    $firstEntry = $mysqli->real_escape_string(strip_tags($_POST['startDriveTime']));
    $lastEntry = $mysqli->real_escape_string(strip_tags($_POST['endDriveTime']));

    // send query
    $query = mysqliNonQuery("DELETE FROM car_to_web where timeEntry between '{$firstEntry}' and '{$lastEntry}'");
    
    if ($query)
        $status = "Delete Car Drive: Completed";

    else
        $status = "Delete Car Drive: Failed for selected drive ";


    echo json_encode($status);

    die(); //in a hole
}

// if user wishes to delete all car data
if (isset($_POST['action']) && $_POST['action'] == 'deleteCarDrives')
{
    // get and sanitize carID
    $carID = $mysqli->real_escape_string(strip_tags($_POST['carID']));

    // send query
    $query = mysqliNonQuery("DELETE FROM car_to_web where carID = {$carID}");

    // good query
    if ($query)
        $status = "Delete Car Drives: {$query->num_rows} rows effected";

    // bad query
    else
        $status = "Delete Car Drives: Failed for car #{$carID} ";

    echo json_encode($status);

    die(); //in a hole
}

// get all cars registered to user
if (isset($_POST['action']) && $_POST['action'] == 'GetUserCars')
{
    // get and sanitize userID
    $userID = $mysqli->real_escape_string(strip_tags($_SESSION['userID']));

    // send query
    $query = mysqliQuery("SELECT carID from web_to_car where userID like '{$userID}'");

    // good query
    if ($query)
        $status = "GetUserCars: {$query->num_rows} cars retreived";
    
    // bad query
    else
    {
        $carID = $mysqli->real_escape_string(strip_tags($_POST['carID']));
        $status = "GetUserCars: {$carID} cars query failed";
    }

    // make arr to push data into
    $dataArr = array();

    // fill arr
    foreach ($query as $key => $value)
        array_push($dataArr, $value);
    
    // data obj to send back
    $data = array();
    $data['data'] = $dataArr;
    $data['status'] = $status;

    // send back data
    echo json_encode($data);

    die(); //in a hole
}

// get car drives for certain car
if (isset($_POST['action']) && $_POST['action'] == 'GetCarDrives')
{
    // get and sanitize carID
    $carID = $mysqli->real_escape_string(strip_tags($_POST['carID']));

    // init query
    $query = "";

    // send query if NOT from GUI
    if (!isset($_POST['fromGUI'])) 
        $query = mysqliQuery("SELECT * from car_to_web where carID = '{$carID}'");

    // send query if FROM GUI 
    else
    {
        $query = mysqliQuery("SELECT * from car_to_web where carID = {$carID} order by timeEntry desc LIMIT 1");
    }

    // good query
    if ($query)
        $status = "GetCarDrives: {$query->num_rows} entries retreived";

    // bad query
    else
        $status = "GetCarDrives: {$carID} cars query failed";

    // arr to fill response data
    $dataArr = array();

    // fill response data
    foreach ($query as $key => $value)
        array_push($dataArr, $value);
    
    // fill return obj
    $data = array();
    $data['data'] = $dataArr;
    $data['status'] = $status;

    // send back data
    echo json_encode($data);

    die(); //in a hole
}

// Delete a car
if (isset($_POST['action']) && $_POST['action'] == 'DeleteUser')
{
    global $status; //global status

    // get and sanitize userID
    $userID = $mysqli->real_escape_string(strip_tags($_POST['userID']));

    // send query
    $query = mysqliNonQuery("DELETE FROM web_to_car where userID like {$userID}");

    // if good
    if ($query)
    {
        $status = "DeleteCar: {$carID} deleted IN web to car DB";

        // delete from other DB as well
        $query2 = mysqliNonQuery("DELETE FROM car_to_web where userID like {$userID}");

        // if good
        if ($query2)
        {
            $status = "DeleteCar: {$carID} deleted from web_to_car and car_to_web";

            // delete from last DB
            $query3 = mysqliNonQuery("DELETE FROM users where userID like {$userID}");

            // if good
            if ($query3)
                $status = "DeleteCar: EVERYTHING DELETED FOR USER";

            // if bad
            else
                "Error deleting user";

        }

        // if bad
        else
            $status = "DeleteCar: {$carID} inserted";
    }
    
    // if bad
    else
        $status = "DeleteCar: failed";

    // prepare response obj
    $data = array();
    $data['data'] = $query;
    $data['status'] = $status;

    // send back data
    echo json_encode($data);

    die(); //in a hole
}

// delete a car
if (isset($_POST['action']) && $_POST['action'] == 'deleteCar')
{
    global $status; //global status msg

    // get and sanitize carID
    $carID = $mysqli->real_escape_string(strip_tags($_POST['carID']));
    
    // send query
    $query = mysqliNonQuery("DELETE FROM web_to_car where carID like {$carID}");

    // if good
    if ($query)
    {
        $status = "DeleteCar: {$carID} deleted IN web to car DB";

        // delete from other DB as well
        $query2 = mysqliNonQuery("DELETE FROM car_to_web where carID like {$carID}");

        // if good
        if ($query2)
            $status = "DeleteCar: {$carID} deleted from web_to_car and car_to_web";

        // if bad
        else
            $status = "DeleteCar: {$carID} inserted";
    }
    // if bad
    else
        $status = "DeleteCar: failed";

    // prepare response obj
    $data = array();
    $data['data'] = $query;
    $data['status'] = $status;

    // send response data
    echo json_encode($data);

    die(); //in a hole
}

// add new car
if (isset($_POST['action']) && $_POST['action'] == 'addNewCar')
{
    global $status;     // global status msg

    // get and sanitize carID and userID
    $carID = $mysqli->real_escape_string(strip_tags($_POST['carID']));
    $userID = $mysqli->real_escape_string(strip_tags($_SESSION['userID']));

    // send query
    $query = mysqliNonQuery("INSERT INTO web_to_car (carID, userID, intendedAngle, intendedSpeed, timeStamp)
                           VALUES ({$carID}, {$userID}, 0, 0, 1)");

    // if good
    if ($query)
        $status = "InsertCar: {$carID} inserted in web to car";
    
    // if bad
    else
        $status = "InsertCar: failed";

    // response obj
    $data = array();
    $data['data'] = $query;
    $data['status'] = $status;

    // send back data
    echo json_encode($data);

    die(); // in a hole
}

// send data into db
if (isset($_POST['action']) && $_POST['action'] == 'web_to_car_Data')
{
    // get and sanitize inputs
    $intendedAngle = $mysqli->real_escape_string(strip_tags($_POST['intendedAngle']));
    $intendedSpeed = $mysqli->real_escape_string(strip_tags($_POST['intendedSpeed']));
    $TC_Level = $mysqli->real_escape_string(strip_tags($_POST['TC_Level']));
    $ABS_Level = $mysqli->real_escape_string(strip_tags($_POST['ABS_Level']));
    $timeStamp = $mysqli->real_escape_string(strip_tags($_POST['timeStamp']));
    $carID = $mysqli->real_escape_string(strip_tags($_POST['carID']));
    $brakeStrength = $mysqli->real_escape_string(strip_tags($_POST['brakeStrength']));


    //Insert query and row response
    $query = mysqliNonQuery("UPDATE web_to_car
                            SET intendedAngle = {$intendedAngle},
                                intendedSpeed = {$intendedSpeed},
                                Brake_Strength = {$brakeStrength},
                                TC_Level = {$TC_Level},
                                ABS_Level = {$ABS_Level},
                                timeStamp = {$timeStamp}
                                WHERE carID = {$carID}");    

    $status = "$query : records inserted";
}


// dont execute if request comes from webserver
else if (isset($_POST['carID']))
{
    global $status; // global status msg

    // get data from database
    $carID = $mysqli->real_escape_string(strip_tags($_POST['carID']));

    // send query
    $carRequest = mysqliQuery("SELECT * from web_to_car where carID like '{$carID}'");


    // get row
    $row = $carRequest->fetch_assoc();

    // check if query was good
    if ($row)
    {     
        // send status back to car if successful
        $status = "Hello Car {$row['carID']} from webservice.php!";

        // package up short response for car -> shorter the better as serial is expensive, hence short names and custom response
        $shortResp = array();
        $shortResp['a'] = $row['intendedAngle'];
        $shortResp['s'] = $row['intendedSpeed'];
        $shortResp['t'] = $row['timeStamp'];
        $shortResp['tc'] = $row['TC_Level'];
        $shortResp['abs'] = $row['ABS_Level'];
        $shortResp['bs'] = $row['Brake_Strength'];

        // json encode data back to car
        echo json_encode($shortResp);
    }

    // bad query
    else
    {
        $status = "POST Query Failed FROM Car {$row['carID']}";
    }

    //Send data if request is from car 
    if (!isset($_POST["fromGUI"]))
    {
        //Data to send
        $userID = $mysqli->real_escape_string(strip_tags($_SESSION['userID']));
        $FL_RPM = $mysqli->real_escape_string(strip_tags($_POST['FL_RPM']));
        $BL_RPM = $mysqli->real_escape_string(strip_tags($_POST['BL_RPM']));
        $FR_RPM = $mysqli->real_escape_string(strip_tags($_POST['FR_RPM']));
        $BR_RPM = $mysqli->real_escape_string(strip_tags($_POST['BR_RPM']));
        $Ground_Speed_Count = $mysqli->real_escape_string(strip_tags($_POST['GSP']));
        $TC_ACTIVE = $mysqli->real_escape_string(strip_tags($_POST['TC']));
        $ABS_ACTIVE = $mysqli->real_escape_string(strip_tags($_POST['ABS']));
        $Burnout_In_Progress = $mysqli->real_escape_string(strip_tags($_POST['BIP']));

        // send query
        $allUserData = mysqliNonQuery("INSERT car_to_web 
                                        set carID = '{$carID}',
                                            userID = '{$userID}',
                                            FL_RPM = '{$FL_RPM}',
                                            BL_RPM = '{$BL_RPM}',
                                            FR_RPM = '{$FR_RPM}',
                                            BR_RPM = '{$BR_RPM}',
                                            Ground_Speed_Count = '{$Ground_Speed_Count}',
                                            TC_ACTIVE = '{$TC_ACTIVE}',
                                            ABS_ACTIVE = '{$ABS_ACTIVE}',
                                            Burnout_In_Progress = '{$Burnout_In_Progress}'");
    }
    die(); //in a hole

}


// add a user
if (isset($_POST['action']) && $_POST['action'] == 'AddUser')
{
    global $status;     // global status

    // lowercase the usn
    $userName = strtolower($mysqli->real_escape_string($_POST['user']));

    // get real password and hash it (sql protection)
    $password = password_hash($mysqli->real_escape_string($_POST['pass']), PASSWORD_DEFAULT);
    
    // insert query and row response
    $query = mysqliNonQuery("INSERT INTO users (username, password)
                VALUES (\"$userName\",\"$password\")");

    // update status
    $status = "$query : records inserted";

    // pack up data
    Done();
}

// get users from db (GET)
if (isset($_GET['action']) && $_GET['action'] == 'GetUsers')
{
    // get all users
    $allUserData = mysqliQuery("select * from users");

    // check if query was good
    if ($allUserData)
    {
        // foreach through all users and add to gblArr
        foreach ($allUserData as $key => $value)
            array_push($gblData, $value);
        

        // get rows in table
        $status = $allUserData->num_rows;

        // pack up data
        Done();
    }

    // bad query
    else
    {
        $status = "GET Query Failed";
    }
}

// get Car Count
if (isset($_POST['action']) && $_POST['action'] == 'GetCarCount')
{
    global $status;     // global status obj
    
    // send query
    $query = mysqliQuery("SELECT carID from web_to_car");               //TODO non query works

    // if good
    if ($query)
        $status = "GetCarCount: $query->num_rows : records fetched for car count";
    
    // if bad
    else
        $status = "GetCarCount: failed";

    // make arr to push data into
    $dataArr = array();

    // fill arr
    foreach ($query as $key => $value)
    {
        array_push($dataArr, $value);
        error_log($value);
    }

    // prep response obj
    $data = array();
    $data['data'] = $dataArr;
    $data['status'] = $status;

    // send response
    echo json_encode($data);

    die(); // in a hole
}



Done();
