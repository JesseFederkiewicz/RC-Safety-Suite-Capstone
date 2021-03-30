<?php

/*
 * File:    functions.php
 * Authors: Jesse Federkiewicz & Timothy Hachey
 * TLDR:    functions to validate user login
*/

require_once "./inc/db.php";

//////////////////////////////////////////////////////////////////////////////////////
// function Validate($params)
// Args: 
//			$params         : user input and resp data
//
// Validates user from database
//
// Returns: updated $params indicating success or failure
//////////////////////////////////////////////////////////////////////////////////////
function Validate($params)
{
    global $mysqli;     //mysqli connection

    // filter data, param is already lowercase
    $username = $mysqli->real_escape_string($params['user']);

    // construct query
    $query = "select *";
    $query .= " from users ";
    $query .= "where username like '{$username}'";

    // init to no
    $params["response"] = "User {$params['user']} Not Authorized<br>with password: {$params['password']}";
    $params['status'] = false;  

    // if successful
    if ($rowData = mysqliQuery($query))
    {
        // if one row inserted
        if ($rowData->num_rows == 1)
        {
            // fetsh row data
            $row = $rowData->fetch_assoc();

            // clean up and assign psw
            $password = $mysqli->real_escape_string($params["password"]);

            // check to see if password matches user
            if (password_verify($password, $row['password']))
            {
                // update response
                $params["response"] = "{$username} logged in";  

                // save userID
                $_SESSION['userID'] = $row['userID'];
        
                // outcome is successful (true)
                $params['status'] = true; 
            }
            // else bad password
            else
                $params["response"] = "Wrong Password";
            
        }

        //If no user
        else if ($rowData->num_rows < 1)
            $params["response"] = "No User Found!";
        
        //If more than one user, should be impossible with insertion checks on create page
        else
            $params["response"] = "Uhm, admin??? Too many users found pAnIC!1!!1!";
        
    }

    return $params;
}
?>