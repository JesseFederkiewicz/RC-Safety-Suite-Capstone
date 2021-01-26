<?php
// // do requires
// // ?? function/session stuff
require_once "./inc/db.php";
require_once "./functions.php";
// // if we make it here.. connection was good

$gblData = array();   //gbl array?
$status = "Failed Ajax Call";

// Redirect if not authenticated
if( !isset($_SESSION["user"])) // not authenticated, does exist
{
  // not logged in, turf him to login.php
  header("location:login.php");// send user to login.php
  die(); // in a hole
}

function Done()
{
    global $gblData, $status;

    $jsonRep['data'] = $gblData;
    $jsonRep['status'] = $status;
    echo json_encode($jsonRep);
    die();
}

if (isset($_GET['action']) && $_GET['action'] == 'GetUsers')
{
    $allUserData = mysqliQuery("select * from users");

    //Check if query was good
    if ($allUserData)
    {
        //$allUserData = $allUserData->fetsh_assoc();

        //Foreach through all users and add to gblArr
        foreach ($allUserData as $key => $value)
        {
            array_push($gblData, $value);
        }
        $status = $allUserData->num_rows;
        Done();
    }

    //Bad query
    else
    {
        error_log("GET Query Failed");
        $status = "GET Query Failed";
    }
}

Done();
