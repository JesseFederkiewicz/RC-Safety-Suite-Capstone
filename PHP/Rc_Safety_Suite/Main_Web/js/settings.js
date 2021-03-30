/*
 * File: 	settings.js
 * Authors: Jesse Federkiewicz & Timothy Hachey
 * TLDR: 	all settings page js elements
*/

$(document).ready( () => {

    // To add a user
    $('#addUsrBtn').click( () => {
        let data = {};
        data['action'] = 'AddUser';
        data['user'] = $('#addUsrInput').val();
        data['pass'] = $('#addPswInput').val();

        $('#status-container').html("");

        AjaxRequest('./webservice.php', 'POST', data, 'json', HandleStatus, Fail)
    });

    // Get all users
    GetUsers();
});

//////////////////////////////////////////////////////////
// function HandleStatus(data, response)
// Args: 
//			data            : ajax response data
//          response        : response message
//
// Get Users from table
//
// Returns: none
///////////////////////////////////////////////////////////
function HandleStatus(data, response)
{
    // AjaxRequest('./webservice.php', 'POST', data, 'json', AddUser, Fail)
    $('#status-container').html("Page Status:<br>");
    $('#status-container').append(data['status']);
    $('#status-container').append("<br>");
    GetUsers();
}

//////////////////////////////////////////////////////////
// function GetUsers()
//
// Get all users from db
//
// Returns: none
///////////////////////////////////////////////////////////
function GetUsers()
{
    let data = {};
    data['action'] = 'GetUsers';

    AjaxRequest('./webservice.php', 'GET', data, 'json', ShowUsers, Fail);
}

//////////////////////////////////////////////////////////
// function HandleStatus(data, response)
// Args: 
//			data            : ajax response data
//          response        : response message
//
// Show all users in table
//
// Returns: none
///////////////////////////////////////////////////////////
function ShowUsers(data, response)
{
    // clear table
    $('#tableDiv').html("");

    // iterate through response
    for (let i = 0; i < data['data'].length; i++)
    {
        // create row
        let tr = document.createElement('tr');

        // add button
        let buttonTD = document.createElement('td');
        let button = document.createElement('button');
        $(button).prop({'type': 'button', 'value' : data['data'][i]['userID']})
        $(button).html('Delete');
        $(button).click( () =>{
            let data = {};
            data['action'] = 'DeleteUser';
            data['userID'] = $(button).val();
    
            $('#status-container').html("");
    
            AjaxRequest('./webservice.php', 'POST', data, 'json', HandleStatus, Fail)
        });

        // add button to cell
        $(buttonTD).append(button);
        $(tr).append(buttonTD);

        // add user to cell
        let usrTD = document.createElement('td');
        $(usrTD).append(data['data'][i]['userID']);
        $(tr).append(usrTD);

        // add username to cell
        let userNameTD = document.createElement('td');
        $(userNameTD).append(data['data'][i]['username']);
        $(tr).append(userNameTD);

        // add hashed password to cell
        let encryptTD = document.createElement('td');
        $(encryptTD).append(data['data'][i]['password']);
        $(tr).append(encryptTD);
        
        // update row to tablediv
        $('#tableDiv').append(tr);
    }

    // update status
    $('#status-container').append(`Retrieved: ${data['status']} records<br>`);
}

//////////////////////////////////////////////////////////
// function Fail(errorData, message)
// Args: 
//			errorData       : ajax response data
//          response        : response message
//
// Logs bad response in console
//
// Returns: none
///////////////////////////////////////////////////////////
function Fail(errorData, response)
{
    console.log(response);
}
