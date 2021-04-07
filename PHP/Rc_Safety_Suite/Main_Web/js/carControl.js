/*
 * File: 	carControl.js
 * Authors: Jesse Federkiewicz & Timothy Hachey
 * TLDR: 	controls all javascript aspects of car control page
*/

// used as guide for controller: -- https://www.cssscript.com/touch-joystick-controller/ 
		
let intendedAngle = 0;		// angle from -180 to 180 which represents drive directin
let speedAsPercent = 0;		// speed from 0-100, interpreted as a percent
let joystick;				// joystick obj
let restartSong = true;		// burnout music so it doesnt restart every tick
$carID = null;				// selected car to control
let ajaxIntervalID;			// interval to send data to car
let pullDataIntervalID;		// interval to pull data from webserver (statics and widgets)
let testTimer;				// timer ID for hardcoded verification test timer
let carToAddID;				// carID to add

class JoystickController
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// constructor( stickID, maxDistance, deadzone )
	// Args: 
	//			stickID         : ID of HTML element (representing joystick) that will be dragged
	//          maxDistance     : maximum amount joystick can move in any direction
	//          deadzone        : joystick must move at least this amount from origin to register value change
	//
	// Constructs joystick
	//
	// Returns: none
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	constructor( stickID, maxDistance, deadzone )
	{
		// make this the stick
		this.id = stickID;
		let stick = document.getElementById(stickID);

		// location from which drag begins, used to calculate offsets
		this.dragStart = null;

		// track touch identifier in case multiple joysticks present
		this.touchId = null;
		
		// make non active
		this.active = false;

		// init pos to centre centre
		this.value = { x: 0, y: 0 }; 

		// make self variable
		let self = this;

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// function handleDown(event)
		// Args: 
		//			event	        : Assign it with the event
		//
		// Event handler for mousedown
		//
		// Returns: none
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		function handleDown(event)
		{
			// activate self
		    self.active = true;

			// all drag movements are instantaneous
			stick.style.transition = '0s';

			// touch event fired before mouse event; prevent redundant mouse event from firing
			event.preventDefault();

			// update current pos
		    if (event.changedTouches)
		    	self.dragStart = { x: event.changedTouches[0].clientX, y: event.changedTouches[0].clientY };
		    else
		    	self.dragStart = { x: event.clientX, y: event.clientY };

			// if this is a touch event, keep track of which one
		    if (event.changedTouches)
                self.touchId = event.changedTouches[0].identifier;                            
		}
		
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// function handleMove(event) 
		// Args: 
		//			event	        : Assign it with the event
		//
		// Event handler for mouseMove
		//
		// Returns: none
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		function handleMove(event) 
		{
			// validity check
		    if ( !self.active ) return;

		    // if this is a touch event, make sure it is the right one
		    // also handle multiple simultaneous touchmove events
		    let touchmoveId = null;

			// update events
		    if (event.changedTouches)
		    {
		    	for (let i = 0; i < event.changedTouches.length; i++)
		    	{
					// move stick
		    		if (self.touchId == event.changedTouches[i].identifier)
		    		{
		    			touchmoveId = i;
		    			event.clientX = event.changedTouches[i].clientX;
		    			event.clientY = event.changedTouches[i].clientY;
		    		}
		    	}

				// validity check
		    	if (touchmoveId == null) return;
		    }

			// values needed to determine angle and speed
		    const xDiff = event.clientX - self.dragStart.x;
		    const yDiff = event.clientY - self.dragStart.y;
		    const angle = Math.atan2(yDiff, xDiff);
			const distance = Math.min(maxDistance, Math.hypot(xDiff, yDiff));
			const xPosition = distance * Math.cos(angle);
			const yPosition = distance * Math.sin(angle);

			// move stick image to new position
		    stick.style.transform = `translate3d(${xPosition}px, ${yPosition}px, 0px)`;

			// deadzone adjustment
			const distance2 = (distance < deadzone) ? 0 : maxDistance / (maxDistance - deadzone) * (distance - deadzone);
		    const xPosition2 = distance2 * Math.cos(angle);
			const yPosition2 = -1 * distance2 * Math.sin(angle);
		    const xPercent = parseFloat((xPosition2 / maxDistance).toFixed(4));
            const yPercent = parseFloat((yPosition2 / maxDistance).toFixed(4));

			// create angle and angle
			intendedAngle = parseFloat((angle * (180 / Math.PI) + 90).toFixed(0));
			intendedAngle = intendedAngle > 180 ? intendedAngle - 360 : intendedAngle;
			speedAsPercent = (distance / maxDistance * 100).toFixed(0);
		    
			// update the current value based on above values
            self.value = { x: (xPercent * 100).toFixed(0), y: (yPercent * 100).toFixed(0)};
		}


		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// function handleUp(event)  
		// Args: 
		//			event	        : Assign it with the event
		//
		// Event handler for mouseUp
		//
		// Returns: none
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		function handleUp(event) 
		{
			// validity check
		    if ( !self.active ) return;

		    // if this is a touch event, make sure it is the right one
		    if (event.changedTouches && self.touchId != event.changedTouches[0].identifier) return;

		    // transition the joystick position back to center
		    stick.style.transition = '.2s';
		    stick.style.transform = `translate3d(0px, 0px, 0px)`;

		    // reset everything
		    self.value = { x: 0, y: 0 };
		    self.touchId = null;
			self.active = false;
			
			
			//If lift put back to 0
			intendedAngle = 0;
			speedAsPercent = 0;
		}

		// assign handlers for stick at bottom of constructor
		stick.addEventListener('mousedown', handleDown);
		stick.addEventListener('touchstart', handleDown);
		document.addEventListener('mousemove', handleMove, {passive: false});
		document.addEventListener('touchmove', handleMove, {passive: false});
		document.addEventListener('mouseup', handleUp);
		document.addEventListener('touchend', handleUp);
	}
}

$(document).ready ( () => {
	joyStick = new JoystickController("joyStick", 64, 8);	
	//joyStick = new JoystickController("joyStick", document.querySelector("#joyBase").clientWidth / 2, 8);

	// hide buttons
	$('#selCarBtn').hide();
	$('#delCarBtn').hide();
	$('#hardcodeGoBtn').hide();
	$('#hardcodeStopBtn').hide();
	$('#timerHardCode').hide();

	// hide car control page
	$('#carControl').hide();

	// slider event handler
	$("#tcSlider").change ( () => {
		$("#tcSliderLbl").html("TC Level [" + $("#tcSlider").val() + "]");
	});

	// ABS level slider
	$("#absSlider").change ( () => {
		$("#absSliderLbl").html("ABS Level [" + $("#absSlider").val() + "]");
	});

	// ABS level slider
	$("#brakeSlider").change ( () => {
		$("#brakeStrengthSlider").html("Brake Strength [" + $("#brakeSlider").val() + "]");
	});

	// redirect to index on click
	$('#toIndexCarControl').click( () =>{
        window.location.replace("./index.php");
    });
	$("#toIndexBtnCarControl").click( () =>{
		window.location.replace("./index.php");
	});

	// go back to main page and clear timer, show hardcode testing controls (mostly used for verification testing)
	$('#hardCodedTesting').click( () =>{
		$('#chooseCar').show();
		$('#carControl').hide();

		$('#carControlBanner').html("Select a car to control");

		clearInterval(ajaxIntervalID);
		clearInterval(pullDataIntervalID);
    });


	// hardcode verification test click, send data to db and therefore car
	$('#hardcodeGoBtn').click ( () => {
		let angle = $("#hardCodeAngle").val();
		let speed = $("#hardCodeSpeed").val();
		FixedSendData(speed, angle);
	});

	// send stop to server with stop btn
	$('#hardcodeStopBtn').click ( () => {

		document.getElementById("hardCodeAngle").setAttribute("value", 0);
		document.getElementById("hardCodeSpeed").setAttribute("value", 0);

		FixedSendData(0, 0);
	});

	// timer between start and stop of car -> used for verification testing
	$('#timerHardCode').click ( () => {
		BrakeTestHardCode();
	});

	// get number of cars fromDB
	GetCarCount();
	
	// add a resize handler for gauge
	window.addEventListener('resize', SizeGauge);
});


///////////////////////////////////////////////////////////
// function update()
//
// Update XY Coords in loop function
//
// Returns: none
////////////////////////////////////////////////////////////
function update()
{
	document.getElementById("xyCoords").innerText = "Joystick: " + JSON.stringify(joyStick.value);
}

///////////////////////////////////////////////////////////
// function loop()
//
// Animate, update joystick data, update gauge
//
// Returns: none
////////////////////////////////////////////////////////////
function loop()
{
	requestAnimationFrame(loop);
	update();
	DoGaugeStuff();
}

///////////////////////////////////////////////////////////
// function loop()
//
// Update gauge speed
//
// Returns: none
////////////////////////////////////////////////////////////
function DoGaugeStuff()
{
	// update gauge size based on page size
	SizeGauge();

	// grab rpms
	let FL_RPM = (Number)($("#FL_RPM").html());
	let BL_RPM = (Number)($("#BL_RPM").html());
	let FR_RPM = (Number)($("#FR_RPM").html());
	let BR_RPM = (Number)($("#BR_RPM").html());

	// get a speeed approximation
	let speed = (FL_RPM + BL_RPM + FR_RPM + BR_RPM) / 4 * 6.5 * 0.001885;

	// update gauge
	document.getElementById("speedGauge").setAttribute("data-value", `${speed}`);
}

///////////////////////////////////////////////////////////
// function GetCarCount()
//
// Gets car count in db
// Sends data to set up items
//
// Returns: none
////////////////////////////////////////////////////////////
function GetCarCount()
{
	let data = {};
	data['action'] = "GetCarCount";
	AjaxRequest('./webservice.php', 'POST', data, 'json', SetUpItems, Fail)
}

function RefreshCarCount()
{
	let data = {};
	data['action'] = "GetCarCount";
	AjaxRequest('./webservice.php', 'POST', data, 'json', RefreshItems, Fail)
}

function RefreshItems (data, response)
{
	// empty select
	$("#carSelect").html("");

	// hide btns while we check new status
	$('#selCarBtn').hide();
	$('#delCarBtn').hide();
	$('#hardcodeGoBtn').hide();
	$('#hardcodeStopBtn').hide();
	$('#timerHardCode').hide();

	// create select and assign properties
	let select = document.createElement('select');
	$(select).prop({"name" : "carSelect", "id" : "whichCarSel"});

	// get what cars the user owns
	FillUserCars();
}

///////////////////////////////////////////////////////////
// function FillUserCars()
//
// Gets car count in db that the current user owns
// Sends data to fill the cars select
//
// Returns: none
////////////////////////////////////////////////////////////
function FillUserCars()
{
    let data = {};
    data['action'] = 'GetUserCars';

    //Send ajax request
    AjaxRequest('./webservice.php', 'POST', data, 'json', FillCarsSelect, Fail)
}

//////////////////////////////////////////////////////////
// function FillCarsSelect(data, response)
// Args: 
//			data            : ajax response data
//          response        : response message
//
// Fills the cars the user owns into first page select
//
// Returns: none
///////////////////////////////////////////////////////////
function FillCarsSelect(data, response)
{
	// assign select
    let select = $('#yourCarNum');
    (select).html("");

	// set it to first item
    let isFirst = true;

	// iterate through data and fill select
    data['data'].forEach(e => {
		
		// declare option
        let option = document.createElement("option");

		// assign option properties
        $(option).prop({"value" : e['carID'], "innerHTML" : "Car " + e['carID']})

		// if first make it selected
        if (isFirst)
        {
            $(option).prop("selected", true);

			// update to not first item
            isFirst = false;
        }

		// append option to select
        $(select).append(option);
    }); 
}


//////////////////////////////////////////////////////////
// function SetUpItems (data, response)
// Args: 
//			data            : ajax response data
//          response        : response message
//
// Update options and create first page controls
//
// Returns: none
///////////////////////////////////////////////////////////
function SetUpItems (data, response)
{
	// clear html of needed divs
	$('#selectCarNumber').html("");
	$('#selectCarBtn').html("");
	$('#addCarBtnDIV').html("");

	// make user select car
	let selCar =  $('#selectCarNumber');
	$(selCar).html("");

	// create select and assign properties
	let select = document.createElement('select');
	$(select).prop({"name" : "carSelect", "id" : "whichCarSel"});

	// // append the car number to the select
	$(selCar).append(select);

	// get what cars the user owns
	FillUserCars();

	// set up button
	let selBtnDiv = $('#selectCarBtnDIV');
	let selCarBtn = document.createElement("input");
	$(selCarBtn).prop({"type" : "button", "id" : "selCarBtn", "value" : "Control Car #" + $(select).val()});
	$(selBtnDiv).append(selCarBtn);

		
	// add option to add another car
	let addCarBtnDiv = $('#addCarBtnDIV');
	let addCarBtn = document.createElement("input");
	$(addCarBtn).prop({"type" : "button", "id" : "addCarBtn", "value" : "Add Car #X"});
	$(addCarBtnDiv).append(addCarBtn);

	// option to delete a car
	let delCarBtnDiv = $('#addCarBtnDIV');			
	let delCarBtn = document.createElement("input");
	$(delCarBtn).prop({"type" : "button", "id" : "delCarBtn", "value" : "Delete Car #" + $(select).val()});
	$(delCarBtnDiv).append(delCarBtn);

	// add select changed event handler to update button
	$(select).change( () => {
		$(selCarBtn).prop("value", "Control Car #" + $(select).val());
		$(delCarBtn).prop("value", "Delete Car #" + $(select).val());
		$carID = $(select).val()
	});

	// add car Btn event handler, adds next availiable car to DB
	$(addCarBtn).click ( () => {
		let sendData = {};
		sendData['action'] = "addNewCar";
		sendData['carID'] = carToAddID;

		AjaxRequest('./webservice.php', 'POST', sendData, 'json', RefreshCarCount, Fail)

		// see which spot we can add in
		GetOpenCarSlot();
	});

	// delete car Btn event handler, deletes car in select
	$(delCarBtn).click ( () => {
		let sendData = {};
		sendData['action'] = "deleteCar";
		sendData['carID'] = $carID;

		AjaxRequest('./webservice.php', 'POST', sendData, 'json', RefreshCarCount, Fail)

		// see which spot we can add in
		GetOpenCarSlot();
	});

	// select car btn click event handler, opens car control page
	$(selCarBtn).click ( () => {

		let data = {};
		data['action'] = "addNewCar";
		data['carID'] = data['data'];

		AjaxRequest('./webservice.php', 'POST', data, 'json', DisplayControls, Fail)

		// start timers
		let interval = 200;
		ajaxIntervalID = setInterval(AjaxData, interval);
		pullDataIntervalID = setInterval(ReceiveData, interval * 3);
		loop();	
	});

	
	//Update which slot is open for add
	GetOpenCarSlot();
}

//////////////////////////////////////////////////////////
// function DisplayControls(data, response)
// Args: 
//			data            : ajax response data
//          response        : response message
//
// display controls and size gauge to page dimentions
//
// Returns: none
///////////////////////////////////////////////////////////
function DisplayControls(data, response)
{
	$('#carControlBanner').html("Controlling car #" + $('#whichCarSel').val());

	$('#chooseCar').hide();
	$('#carControl').show();

	SizeGauge();
}

//////////////////////////////////////////////////////////
// function SizeGauge()
//
// Resizes gauge according to page dimentions
//
// Returns: none
///////////////////////////////////////////////////////////
function SizeGauge()
{
	// set up the speedo size
	let gauge = document.getElementById("speedGauge");

	// set height
	var speedGaugeHeight = $("#speedGaugeDiv").css('height');
	speedGaugeHeight = speedGaugeHeight.substring(0, speedGaugeHeight.length - 2);

	// set width
	var speedGaugeWidth = $("#speedGaugeDiv").css('width');
	speedGaugeWidth = speedGaugeWidth.substring(0, speedGaugeWidth.length - 2);

	// update attributes
	gauge.setAttribute("data-height", `${speedGaugeHeight}`);
	gauge.setAttribute("data-width", `${speedGaugeWidth}`);
}


//////////////////////////////////////////////////////////
// function DisplayControls(data, response)
// Args: 
//			data            : ajax response data
//          response        : response message
//
// Updates status of ajax request to display to user
//
// Returns: none
///////////////////////////////////////////////////////////
function HandleStatus(data, response)
{
    $('#connectionStatus').html("GOOD AJAX REQUEST!");
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
function Fail(errorData, message)
{
    $('#connectionStatus').html("BAD AJAX REQUEST!");
	console.log(message);
}

//////////////////////////////////////////////////////////
// function CarSimReq(data, response)
// Args: 
//			data            : ajax response data
//          response        : response message
//
// Updates right side of page data with what in DB
// Shows what car sees
//
// Returns: none
///////////////////////////////////////////////////////////
function CarSimReq(data, response)
{
	$("#XYFeedBackTest").html("Angle = ");
	$("#XYFeedBackTest").append(data['intendedAngle']);
	$("#XYFeedBackTest").append("<br>Speed as % = ");
	$("#XYFeedBackTest").append(data['intendedSpeed']);
	$("#XYFeedBackTest").append("<br>TimeStamp = ");
	$("#XYFeedBackTest").append(data['timeStamp']);	
}

//////////////////////////////////////////////////////////
// function AjaxData()
//
// Happens every 250ms send data to webserver
//
// Returns: none
///////////////////////////////////////////////////////////
function AjaxData()
{
	//Send Data
	let data = {};
    data['action'] = 'web_to_car_Data';
    data['carID'] = $('#whichCarSel').val();              
	data['intendedAngle'] = intendedAngle;
	data['intendedSpeed'] = speedAsPercent;
	data['brakeStrength'] = $("#brakeSlider").val();
	data['TC_Level'] = $("#tcSlider").val();
	data['ABS_Level'] = $("#absSlider").val();
	
    // Date.now() returns number of ms from January 1, 1970, parsed to keep track of 1 minute in ms
    // for unsigned 16bit int (0 - 65,535), and as little wrapping as little as possible
    // if mem space is required can easily be reduced further
    data['timeStamp'] = Date.now() % 60000;     // returns number of ms from January 1, 1970, good for timeout comparison
	// parsed to last 4 digits for data storage as its essentially acting as a timer

	AjaxRequest('./webservice.php', 'POST', data, 'json', HandleStatus, Fail)	
}

//////////////////////////////////////////////////////////
// function BrakeTestHardCode()
//
// Sends angle and speed to server
//
// Returns: none
///////////////////////////////////////////////////////////
function BrakeTestHardCode()
{
	let angle = $("#hardCodeAngle").val();
	let speed = $("#hardCodeSpeed").val();
	FixedSendData(speed, angle);
	testTimer = setInterval(Brake, $("#timerVal").val());
}

//////////////////////////////////////////////////////////
// Brake()
//
// Tell car to STOP
//
// Returns: none
///////////////////////////////////////////////////////////
function Brake()
{
	FixedSendData(0, 0);
	clearInterval(testTimer);
}

//////////////////////////////////////////////////////////
// function CarSimReq(data, response)
// Args: 
//			speed        : speed from 0-100 (%)
//          angle        : angle from -180 to 180 degrees
//
// Sends data based on numeric input on front page of
// car control
//
// Returns: none
///////////////////////////////////////////////////////////
function FixedSendData(speed, angle)
{	
	// send Data
	let data = {};
    data['action'] = 'web_to_car_Data';
    data['carID'] = $('#whichCarSel').val();              
	data['intendedAngle'] = angle;
	data['intendedSpeed'] = speed;
	data['brakeStrength'] = $("#hardCodeBrakeStrength").val();
	data['TC_Level'] = $("#hardCodeTC").val();
	data['ABS_Level'] = $("#hardCodeAbs").val();
	
    // date.now() returns number of ms from January 1, 1970, parsed to keep track of 1 minute in ms
    // for unsigned 16bit int (0 - 65,535), and as little wrapping as little as possible
    // if mem space is required can easily be reduced further
    data['timeStamp'] = Date.now() % 60000;     // returns number of ms from January 1, 1970, good for timeout comparison
	// parsed to last 4 digits for data storage as its essentially acting as a timer

	AjaxRequest('./webservice.php', 'POST', data, 'json', Useless, Fail)	
}

//////////////////////////////////////////////////////////////////////
// function Useless()
//
// Console logs that the server received your hardcoded command
//
// Returns: none
///////////////////////////////////////////////////////////////////////
function Useless()
{
	console.log("Hardcoded command sent successfully");
}

//////////////////////////////////////////////////////////////////////
// function ReceiveData()
//
// Sends ajax request to display statistics
//
// Returns: none
///////////////////////////////////////////////////////////////////////
function ReceiveData()
{
	let receiveCarData = {};
	receiveCarData['fromGUI'] = "yeppers";
	receiveCarData['carID'] = $carID;

	AjaxRequest('./webservice.php', 'POST', receiveCarData, 'json', UpdateSelfData, Fail);

	let fetchSentData = {};
	fetchSentData['action'] = "GetCarDrives";
	fetchSentData['fromGUI'] = "yeppers";
	fetchSentData['carID'] = $carID;

	AjaxRequest('./webservice.php', 'POST', fetchSentData, 'json', UpdateCarData, Fail);
}

//////////////////////////////////////////////////////////
// function CarSimReq(data, response)
// Args: 
//			data            : ajax response data
//          response        : response message
//
// Display Statistics on car control page
//
// Returns: none
///////////////////////////////////////////////////////////
function UpdateCarData(data, response)
{
	// account for if car has never been driven
	if (data['data'].length == 0)
		return;

	// grab rpm data of most recent pull
	$("#FL_RPM").html(data['data'][0]['FL_RPM']);
	$("#FR_RPM").html(data['data'][0]['FR_RPM']);
	$("#BL_RPM").html(data['data'][0]['BL_RPM']);
	$("#BR_RPM").html(data['data'][0]['BR_RPM']);

	// update if tc is active
	if (data['data'][0]['TC_ACTIVE'] == 1)
		$("#TC_ACTIVE").html("TC Active");
	else
		$("#TC_ACTIVE").html("TC Not Active");

	// update if abs is active
	if (data['data'][0]['ABS_ACTIVE'] == 1)
		$("#ABS_ACTIVE").html("ABS Active");
	else
		$("#ABS_ACTIVE").html("ABS Not Active");

	// update if there is a burnout
	if (data['data'][0]['Burnout_In_Progress'] == 1)
		$("#Burnout_Active").html("Wee spinny spinny right");
	else if (data['data'][0]['Burnout_In_Progress'] == 2)
		$("#Burnout_Active").html("Wee spinny spinny left");
	else
		$("#Burnout_Active").html("No Spinny :(");

	// start song if a burnout is in progress
	if (data['data'][0]['Burnout_In_Progress'] === "1" || data['data'][0]['Burnout_In_Progress'] === "2" && restartSong)
	{
		restartSong = false;
		document.getElementById("roundAndRound").play();
	}
	else
	{
		restartSong = true;
		document.getElementById("roundAndRound").pause();
	}
}

//////////////////////////////////////////////////////////
// function CarSimReq(data, response)
// Args: 
//			data            : ajax response data
//          response        : response message
//
// Update intended speed and angle
//
// Returns: none
///////////////////////////////////////////////////////////
function UpdateSelfData(data, response)
{
	$("#XYFeedBackTest").html(`Intended Angle: ${data['a']} || Intended Speed: ${data['s']}`);
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
// function FillCarsSelect(data, response)
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
    let select = $('#whichCarSel');
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

		// hide buttons
		$('#selCarBtn').hide();
		$('#delCarBtn').hide();
		$('#hardcodeGoBtn').hide();
		$('#hardcodeStopBtn').hide();
		$('#timerHardCode').hide();
    }

	// show buttons if there are cars registered
	else
	{
		$('#selCarBtn').show();
		$('#delCarBtn').show();
		$('#hardcodeGoBtn').show();
		$('#hardcodeStopBtn').show();
		$('#timerHardCode').show();
	}

	// update btn text
	$("#selCarBtn").val("Control Car #" + $(select).val())
	$("#delCarBtn").val("Delete Car #" + $(select).val())

	// assign global carID to current selected val
	$carID = $(select).val()
}

//////////////////////////////////////////////////////////
// function FillCarsSelect(data, response)
//
// Ask DB for all cars back
//
// Returns: none
///////////////////////////////////////////////////////////
function GetOpenCarSlot()
{
	let data = {};
	data['action'] = "GetCarCount";
	AjaxRequest('./webservice.php', 'POST', data, 'json', ReceivedSlotData, Fail)
}

//////////////////////////////////////////////////////////
// function FillCarsSelect(data, response)
// Args: 
//			data            : ajax response data
//          response        : response message
//
// Updates which slot you can add a car (Lowest num)
//
// Returns: none
///////////////////////////////////////////////////////////
function ReceivedSlotData(data, response)
{
	// check all availiable numbers
	for ($i = 1; $i < Infinity; $i++)
	{
		// init bool
		isOpen = true;

		// iterate through current cars to check if current one is open
		for ($x = 0; $x < data["data"].length; $x++)
		{
			// if not open
			if (data["data"][$x]["carID"] == $i)
				isOpen = false;
		}

		// if open assign and get out
		if (isOpen)
		{
			$("#addCarBtn").val("Add Car #" + $i)
			carToAddID = $i;
			return;
		}

		// only allow 1M cars in the db... sorry china
		else if ($i > 1000000)
		{
			$("#addCarBtn").val("Something Is Really Wrong Or This Website is really popular (only 1M cars allowed!)")
			return;
		}
	}
}