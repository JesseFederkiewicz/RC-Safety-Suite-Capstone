$(document).ready( () => {

    $('#addUsrBtn').click( () => {
        let data = {};
        data['action'] = 'AddUser';
        data['user'] = $('#addUsrInput').val();
        data['pass'] = $('#addPswInput').val();

        $('#status-container').html("");

        AjaxRequest('./webservice.php', 'POST', data, 'json', HandleStatus, Fail)
    });

    GetUsers();
});

function HandleStatus(data, response)
{
    // AjaxRequest('./webservice.php', 'POST', data, 'json', AddUser, Fail)
    $('#status-container').html("Page Status:<br>");
    $('#status-container').append(data['status']);
    $('#status-container').append("<br>");
    GetUsers();
}

function GetUsers()
{
    let data = {};
    data['action'] = 'GetUsers';

    AjaxRequest('./webservice.php', 'GET', data, 'json', ShowUsers, Fail);
}

function ShowUsers(data, response)
{
    $('#tableDiv').html("");
    for (let i = 0; i < data['data'].length; i++)
    {
        let tr = document.createElement('tr');

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

        $(buttonTD).append(button);
        $(tr).append(buttonTD);

        let usrTD = document.createElement('td');
        $(usrTD).append(data['data'][i]['userID']);
        $(tr).append(usrTD);

        let userNameTD = document.createElement('td');
        $(userNameTD).append(data['data'][i]['username']);
        $(tr).append(userNameTD);

        let encryptTD = document.createElement('td');
        $(encryptTD).append(data['data'][i]['password']);
        $(tr).append(encryptTD);
        
        $('#tableDiv').append(tr);
    }

    $('#status-container').append(`Retrieved: ${data['status']} records<br>`);
}

function Fail(errorMessage)
{
    alert(errorMessage);
    console.log(errorMessage);
}
