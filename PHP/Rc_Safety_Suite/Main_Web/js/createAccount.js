/*
 * File:    createAccount.js
 * Authors: Jesse Federkiewicz & Timothy Hachey
 * TLDR:    Allows user to create account
*/

$(document).ready( () => {

    //On user creation click
    $('#createBtn').click( () => {
        let data = {};
        data['action'] = 'AddUser';
        data['user'] = $('#addUsrInput').val();
        data['pass'] = $('#addPswInput').val();
    
        //Empty status container as ajax goes to fetsh new updated status
        $('#status-container').html("");
        
        //Send ajax request
        AjaxRequest('./webservice.php', 'POST', data, 'json', HandleStatus, Fail)
    });

});

//////////////////////////////////////////////////////////
// function HandleStatus(data, response)
// Args: 
//			data            : ajax response data
//          response        : response message
//
// Updates status of operation
//
// Returns: none
///////////////////////////////////////////////////////////
function HandleStatus(data, response)
{
    //Prompt status output
    $('#status-container').html("Page Status:<br>");

    //If user already exists (query returns -1)
    if (data['status'].includes('-1'))
        $('#status-container').append("User Already Exists.");
    
    //If new user
    else
    {
        //Would display 1 row inserted but will redirect before most people notice
        $('#status-container').append(data['status']);
        window.location.replace("./login.php");
    }
}

//////////////////////////////////////////////////////////
// function Fail(errorData, message)
// Args: 
//			errorData       : ajax response data
//          response        : response message
//
// Updates status of ajax request to display to user
//
// Returns: none
///////////////////////////////////////////////////////////
function Fail(errorData, response)
{
    console.log(response);
}