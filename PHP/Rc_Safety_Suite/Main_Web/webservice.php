<?php
// // do requires
// // ?? function/session stuff
require_once "./inc/db.php";
require_once "./functions.php";
// // if we make it here.. connection was good

$gblData = array();
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

// //////////////////////////////////////////////
// //  Request from car
// //  Get Current data in db and send to car
// //////////////////////////////////////////////
// if (isset($_POST['GrabWebToCar']))
// {
//     global $status;
    
//     $carID = real_escape_string(strip_tags($_POST['GrabWebToCar']));

//     $carRequest = mysqliQuery("SELECT * from web_to_car where carID like {$carID}");

//     //Get row
//     $row = $carRequest->fetch_assoc();

//     //Check if query was good
//     if ($row)
//     {     
//         //Send status back to car if successful
//         $status = "Hello Car {$row['carID']} from webservice.php!";

//         //Throw data to log
//         //error_log("Car ID: {$row['carID']} X: {$row['xCoord']} Y: {$row['yCoord']} Stamp: {$row['timeStamp']}");

//         $shortResp = array();
//         $shortResp['a'] = $row['intendedAngle'];
//         $shortResp['s'] = $row['intendedSpeed'];
//         $shortResp['t'] = $row['timeStamp'];

//         //Json encode data back to car
//         echo json_encode($shortResp);
//         die();  //in a hole
//     }

//     //Bad query
//     else
//     {
//         error_log("POST Query Failed FROM Car {$row['carID']}");
//         $status = "POST Query Failed FROM Car {$row['carID']}";
//     }
// }

// if (isset($_GET['GrabWebToCar']))
// {
//     global $status;

//     $carID = real_escape_string(strip_tags($_POST['GrabWebToCar']));

//     $carRequest = mysqliQuery("SELECT * from web_to_car where carID like {$carID}");

//     //Get row
//     $row = $carRequest->fetch_assoc();

//     //Check if query was good
//     if ($row)
//     {     
//         //Send status back to car if successful
//         $status = "Hello Car {$row['carID']} from webservice.php!";

//         //Throw data to log
//         //error_log("Car ID: {$row['carID']} X: {$row['xCoord']} Y: {$row['yCoord']} Stamp: {$row['timeStamp']}");

//         $shortResp = array();
//         $shortResp['a'] = $row['intendedAngle'];
//         $shortResp['s'] = $row['intendedSpeed'];
//         $shortResp['t'] = $row['timeStamp'];

//         //Json encode data back to car
//         echo json_encode($shortResp);
//         //die();  //in a hole
//     }

//     //Bad query
//     else
//     {
//         error_log("POST Query Failed FROM Car {$row['carID']}");
//         $status = "POST Query Failed FROM Car {$row['carID']}";
//     }
// }

// if (isset($_POST['CarToWeb']))
// {
//     $carID = real_escape_string(strip_tags($_POST['CarToWeb']));

//     //All items we want to send up
//     $groundSpeed = real_escape_string(strip_tags($_POST['GS']));

//Delete a car
if (isset($_POST['action']) && $_POST['action'] == 'DeleteUser')
{
    global $status;

    $userID = $mysqli->real_escape_string(strip_tags($_POST['userID']));

    $query = mysqliNonQuery("DELETE FROM web_to_car where userID like {$userID}");

    error_log($query);

    if ($query)
    {
        $status = "DeleteCar: {$carID} deleted IN web to car DB";

        //Insert into other DB as well
        $query2 = mysqliNonQuery("DELETE FROM car_to_web where userID like {$userID}");

        if ($query2)
        {
            $status = "DeleteCar: {$carID} deleted from web_to_car and car_to_web";

            $query3 = mysqliNonQuery("DELETE FROM users where userID like {$userID}");

            if ($query3)
                $status = "DeleteCar: EVERYTHING DELETED FOR USER";

            else
                "Error deleting user";

        }

        else
            $status = "DeleteCar: {$carID} inserted";
    }
    
    else
        $status = "DeleteCar: failed";

    $data = array();
    $data['data'] = $query;
    $data['status'] = $status;

    echo json_encode($data);
    die();
}

//Delete a car
if (isset($_POST['action']) && $_POST['action'] == 'deleteCar')
{
    global $status;

    $carID = $mysqli->real_escape_string(strip_tags($_POST['carID']));
    
    $query = mysqliNonQuery("DELETE FROM web_to_car where carID like {$carID}");

    error_log($query);

    if ($query)
    {
        $status = "DeleteCar: {$carID} deleted IN web to car DB";

        //Insert into other DB as well
        $query2 = mysqliNonQuery("DELETE FROM car_to_web where carID like {$carID}");

        if ($query2)
            $status = "DeleteCar: {$carID} deleted from web_to_car and car_to_web";

        else
            $status = "DeleteCar: {$carID} inserted";
    }
    
    else
        $status = "DeleteCar: failed";

    $data = array();
    $data['data'] = $query;
    $data['status'] = $status;

    echo json_encode($data);
    die();
}

//Add new car
if (isset($_POST['action']) && $_POST['action'] == 'addNewCar')
{
    global $status;

    //Change this once login is required to car control page
    $_SESSION['userID'] = 5;

    $carID = $mysqli->real_escape_string(strip_tags($_POST['carID']));
    $userID = $mysqli->real_escape_string(strip_tags($_SESSION['userID']));

    $query = mysqliNonQuery("INSERT INTO web_to_car (carID, userID, intendedAngle, intendedSpeed, timeStamp)
                           VALUES ({$carID}, {$userID}, 0, 0, 1)");

    if ($query)
    {
        $status = "InsertCar: {$carID} inserted in web to car";

        //Insert into other DB as well
        $query2 = mysqliNonQuery("INSERT INTO car_to_web (carID, userID, FL_RPM, BL_RPM, FR_RPM, BR_RPM, Ground_Speed_Count, TC_ACTIVE, ABS_ACTIVE, BURNOUT_IN_PROGRESS)
        VALUES ({$carID}, {$userID}, 0, 0, 0, 0, 0, 0, 0, 0)");

        if ($query2)
            $status = "InsertCar: {$carID} inserted to both tables";

        else
            $status = "InsertCar: {$carID} inserted to both tables";
        
        
    }
    
    else
        $status = "InsertCar: failed";

    $data = array();
    $data['data'] = $query;
    $data['status'] = $status;

    echo json_encode($data);
    die();
}

//Send data into db
if (isset($_POST['action']) && $_POST['action'] == 'web_to_car_Data')
{
    //Insert query and row response
    $query = mysqliNonQuery("UPDATE web_to_car
                            SET intendedAngle = {$_POST['intendedAngle']},
                                intendedSpeed = {$_POST['intendedSpeed']},
                                timeStamp = {$_POST['timeStamp']}
                                WHERE carID = {$_POST['carID']}");    

    $status = "$query : records inserted";
}

//Dont execute if request comes from webserver
else if (isset($_POST['carID']))
{
    global $status;

    
    //Get data from database
    $carID = $mysqli->real_escape_string(strip_tags($_POST['carID']));

    //$status = "Failed Ajax Call '{$carID}' AFTER";

    $status = "CARID: '{$carID}' AFTER";

    $carRequest = mysqliQuery("SELECT * from web_to_car where carID like '{$carID}'");


    //Get row
    $row = $carRequest->fetch_assoc();

    //Check if query was good
    if ($row)
    {     
        //Send status back to car if successful
        $status = "Hello Car {$row['carID']} from webservice.php!";

        //Throw data to log
        //error_log("Car ID: {$row['carID']} X: {$row['xCoord']} Y: {$row['yCoord']} Stamp: {$row['timeStamp']}");

        $shortResp = array();
        $shortResp['a'] = $row['intendedAngle'];
        $shortResp['s'] = $row['intendedSpeed'];
        $shortResp['t'] = $row['timeStamp'];

        //Json encode data back to car
        echo json_encode($shortResp);
    }

    //Bad query
    else
    {
        // error_log("POST Query Failed FROM Car {$row['carID']}");
        // $status = "POST Query Failed FROM Car {$row['carID']}";
    }

    //Send data

    //Data to send
    $FL_RPM = $mysqli->real_escape_string(strip_tags($_POST['FL_RPM']));
    $BL_RPM = $mysqli->real_escape_string(strip_tags($_POST['BL_RPM']));
    $FR_RPM = $mysqli->real_escape_string(strip_tags($_POST['FR_RPM']));
    $BR_RPM = $mysqli->real_escape_string(strip_tags($_POST['BR_RPM']));
    $Ground_Speed_Count = $mysqli->real_escape_string(strip_tags($_POST['GSP']));
    $TC_ACTIVE = $mysqli->real_escape_string(strip_tags($_POST['TC']));
    $ABS_ACTIVE = $mysqli->real_escape_string(strip_tags($_POST['ABS']));
    $Burnout_In_Progress = $mysqli->real_escape_string(strip_tags($_POST['BIP']));

    $allUserData = mysqliNonQuery("UPDATE car_to_web 
                                    set FL_RPM = '{$FL_RPM}',
                                        BL_RPM = '{$BL_RPM}',
                                        FR_RPM = '{$FR_RPM}',
                                        BR_RPM = '{$BR_RPM}',
                                        Ground_Speed_Count = '{$Ground_Speed_Count}',
                                        TC_ACTIVE = '{$TC_ACTIVE}',
                                        ABS_ACTIVE = '{$ABS_ACTIVE}',
                                        Burnout_In_Progress = '{$Burnout_In_Progress}' 
                                        where carID = '{$carID}'");

    die(); //in a hole

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

//Get Car Count
if (isset($_POST['action']) && $_POST['action'] == 'GetCarCount')
{
    global $status;
    
    $query = mysqliNonQuery("SELECT carID from web_to_car");

    if ($query)
        $status = "GetCarcount: $query : records fetched for car count";
    
    else
        $status = "GetCarCount: failed";

    $data = array();
    $data['data'] = $query;
    $data['status'] = $status;

    echo json_encode($data);
    die();
}



Done();
