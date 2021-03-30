/*
 * File:    pastDrives.js
 * Authors: Jesse Federkiewicz & Timothy Hachey
 * TLDR:    pulls past drive data
*/

let _carDriveBins = [];     // holds all drives

$(document).ready( () => {

    // hide table because nothing to display yet
    $("#driveStatsRaw").hide();
    
    // load drive
    $('#loadDriveBtn').click( () =>{
        _carDriveBins.forEach( e => {
            if (e[0]["timeEntry"] != null && ($("#yourDriveDate").val()).includes(e[0]["timeEntry"]))
            {
                FillRawTableDataForDrive(e);
                return;
            }
        });
    });

    // go to index
    $('#toIndexBtn').click( () =>{
        window.location.replace("./index.php");
    });

    // delete all car data
    $('#deleteCarData').click( () =>{
        let data = {};
        data['action'] = 'deleteCarDrives';
        data['carID'] = $('#yourCarNum').val();
            
        //Send ajax request
        AjaxRequest('./webservice.php', 'POST', data, 'json', HandleStatus, Fail)
    });

    // delete a specific car drive
    $('#deleteCarDrive').click( () =>{

        _carDriveBins.forEach( e => {
            if (e[0]["timeEntry"] != null && ($("#yourDriveDate").val()).includes(e[0]["timeEntry"]))
            {
                let data = {};
                data['action'] = 'deleteDrive';
                data['driveData'] = e;
                            
                //Send ajax request
                AjaxRequest('./webservice.php', 'POST', data, 'json', HandleStatus, Fail)

                return;
            }
        });
    });

    // fill all cars that the user owns
    FillUserCars();

    // update drives for a specific car
    $('#yourCarNum').change( () => {
        FillCarDrives($('#yourCarNum').val());
    });
});


//////////////////////////////////////////////////////////////////////////////////////
// function FillRawTableDataForDrive(driveData)
// Args: 
//			driveData       : all rows for selected drive
//
// Fills table with raw drive statistics
//
// Returns: none
//////////////////////////////////////////////////////////////////////////////////////
function FillRawTableDataForDrive(driveData)
{
    // create and show table div
    let table = $("#driveStatsRawBody");
    $("#driveStatsRaw").show();
    
    // blank table
    $(table).html("");

    // add a row to table
    driveData.forEach( e => {

        // validity check
        if (e["timeEntry"] == null)
            return;
            
        // create row
        let tr = document.createElement("tr");
        
        // fl RPM
        let flRpmTd = document.createElement("td");
        $(flRpmTd).html(e["FL_RPM"]);
        $(tr).append(flRpmTd);

        // bl RPM
        let blRpmTd = document.createElement("td");
        $(blRpmTd).html(e["BL_RPM"]);
        $(tr).append(blRpmTd);

        // fr RPM
        let frRpmTd = document.createElement("td");
        $(frRpmTd).html(e["FR_RPM"]);
        $(tr).append(frRpmTd);

        // br RPM
        let brRpmTd = document.createElement("td");
        $(brRpmTd).html(e["BR_RPM"]);
        $(tr).append(brRpmTd);

        // is TC Active
        let tcActiveTd = document.createElement("td");
        $(tcActiveTd).html(e["TC_ACTIVE"]);
        $(tr).append(tcActiveTd);

        // is ABS Active
        let absActiveTd = document.createElement("td");
        $(absActiveTd).html(e["ABS_ACTIVE"]);
        $(tr).append(absActiveTd);

        //if burnout active
        let burnoutTd = document.createElement("td");
        $(burnoutTd).html(e["Burnout_In_Progress"]);
        $(tr).append(burnoutTd);

        // motion counter
        let motionCountTd = document.createElement("td");
        $(motionCountTd).html(e["Ground_Speed_Count"]);
        $(tr).append(motionCountTd);

        // time entered to the ms
        let timeEntryTd = document.createElement("td");
        $(timeEntryTd).html(e["timeEntry"]);
        $(tr).append(timeEntryTd);

        // add row to table
        $(table).append(tr);
    });
}

//////////////////////////////////////////////////////////////////////////////////////
// function FillUserCars()
//
// Fill all cars the user owns
//
// Returns: none
//////////////////////////////////////////////////////////////////////////////////////
function FillUserCars()
{
    let data = {};
    data['action'] = 'GetUserCars';

    //Send ajax request
    AjaxRequest('./webservice.php', 'POST', data, 'json', FillCarsSelect, Fail)
}

//////////////////////////////////////////////////////////
// function HandleStatus(data, response)
// Args: 
//			data            : ajax response data
//          response        : response message
//
// Updates car select
//
// Returns: none
///////////////////////////////////////////////////////////
function FillCarsSelect(data, response)
{
    // get select
    let select = $('#yourCarNum');
    (select).html("");

    // if first
    let isFirst = true;

    // iterate through cars
    data['data'].forEach(e => {

        // create option
        let option = document.createElement("option");

        // update properties of option
        $(option).prop({"value" : e['carID'], "innerHTML" : "Car " + e['carID']})

        // select it if first
        if (isFirst)
        {
            $(option).prop("selected", true);
            isFirst = false;
        }

        // add to table
        $(select).append(option);
    });

    // if no cars found
    if (data['status'] == "GetUserCars: 0 cars retreived")
    {
        // create option
        let option = document.createElement("option");

        // assign option props
        $(option).prop({"value" : "", "innerHTML" : "No Cars Found"})

        // select it if its first
        if (isFirst)
        {
            $(option).prop("selected", true);
            isFirst = false;
        }

        // appent option to select
        $(select).append(option);
    }


    // fill the drives for selected car
    FillCarDrives($(select).val());       
}

//////////////////////////////////////////////////////////
// function HandleStatus(data, response)
// Args: 
//			carID           : what car is selected
//
// Gets drives for car
//
// Returns: none
///////////////////////////////////////////////////////////
function FillCarDrives(carID)
{
    let data = {};
    data['action'] = 'GetCarDrives';
    data['carID'] = carID;

    //Send ajax request
    AjaxRequest('./webservice.php', 'POST', data, 'json', FillCarDrivesSelect, Fail)
}


//////////////////////////////////////////////////////////
// function FillCarDrivesSelect(data, response)
// Args: 
//			data            : ajax response data
//          response        : response message
//
// Fills the past drives for selected car
//
// Returns: none
///////////////////////////////////////////////////////////
function FillCarDrivesSelect(data, response)
{
    var lastDate;       // previous date
    var curDate;        // current date

    // grab select
    let select = $('#yourDriveDate');
    (select).html("");

    // set to first
    let isFirst = true;

    // make temp bin for this drive
    let tempBin = [];

    // if no cars
    if (data['status'] == "GetUserCars: 0 entries retreived")
    {
        // create option
        let option = document.createElement("option");

        // add option properties
        $(option).prop({"value" : "", "innerHTML" : "No Entries for the vehicle"})

        // if first select it
        if (isFirst)
        {
            $(option).prop("selected", true);
            isFirst = false;
        }

        // append option to select
        $(select).append(option);

        return;
    }

    // update select
    data['data'].forEach(e => {

        // make last date = current date
        if (!isFirst)
            lastDate = curDate;

        // update current date
        curDate = new Date(e["timeEntry"]);
         
        // if not first
        if (!isFirst)
        {           
            // see if entries are more than a minute apart, if they are bin it.
            if (curDate - lastDate > 60000)
            {
                _carDriveBins.push(tempBin);
                tempBin = [];
                isFirst = true;
            }
            else
                tempBin.push(e);
        }

        else
        {
            isFirst = false;
            tempBin.push(e);
        }

    
    });

    // update the master bin with the temp bin
    _carDriveBins.push(tempBin);

    // iterate through all drives
    _carDriveBins.forEach(e => {       
        
        // create option
        let option = document.createElement("option");

        // assign properties
        $(option).prop({"value" : e['timeEntry'], "innerHTML" : "StartTime: " + e[0]['timeEntry']})

        // if first select it
        if (isFirst)
        {
            $(option).prop("selected", true);
            isFirst = false;
        }

        // appent option to select
        $(select).append(option);
    });
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

    //Update selects
    FillUserCars();
}

//////////////////////////////////////////////////////////
// function HandleStatus(data, response)
// Args: 
//			data            : ajax response data
//          response        : response message
//
// Fills the cars for user
//
// Returns: none
///////////////////////////////////////////////////////////
function HandleStatus(data, response)
{
    console.log(response);

    //Update selects
    FillUserCars();
}
