<?php
    require_once "functions.php"; // add start_session and include fnc Validate()

    // Redirect if not authenticated
    if( !isset($_SESSION["user"])) // not authenticated, does exist
    {
        // not logged in, turf him to login.php
        header("location:login.php");// send user to login.php
        die(); // in a hole
    }
?>

<!DOCTYPE html>
<html lang="en">
<head>
<link rel="stylesheet" href="style.css">
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Car Control</title>
    <script src="//ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
    <script src="./js/carControl.js" type="text/javascript"></script> 
    <script src = "./js/ajax.js"></script>
    <script src="//cdn.rawgit.com/Mikhus/canvas-gauges/gh-pages/download/2.1.7/radial/gauge.min.js"></script>
</head>
<body>
    <!-- Audio for doing burnouts -->
    <audio controls preload="auto" style="display:none" id = "roundAndRound">  
        <source id = "songUrl" src="https://coolstuffliveshere.com/Rc_Safety_Suite/Main_Web/images/bestSong.mp3?nocache=1" type="audio/mp3" loop=true http=true >  
    </audio>  
    <div class = hdr>
        <h1 class = 'font-effect-3d text-center' id = carControlBanner>Select a car to control</h1>
    </div>

    <!-- Front car page -->
    <div id = chooseCar>
        <div id = selectCarNumber>
        </div>

        <div id = selectCarBtnDIV>
        </div>

        <div id = addCarBtnDIV>
        </div>

        <div id = "toIndexDIV">
            <input type="button" id="toIndexBtnCarControl" value="To Index">
        </div>
        
        <br>
        <div id = hardCodedTestingDiv>
            <label id = "hardCodeAngleLbl">Intended Angle : </label>
            <input type="number" id="hardCodeAngle" min="-180" max="180"> <br>
            <label id = "hardCodeSpeedLbl">Intended Speed : </label>
            <input type="number" id="hardCodeSpeed"  min="0" max="100"> <br>
            <label id = "hardCodeBrakeStrengthLbl">Brake Strength : </label>
            <input type="number" id="hardCodeBrakeStrength" min="0" max="10"> <br>
            <label id = "hardCodeAbsLbl">Abs Level : </label>
            <input type="number" id="hardCodeAbs" min="0" max="10"> <br>
            <label id = "hardCodeTcLbl">Tc Level : </label>
            <input type="number" id="hardCodeTC" min="0" max="10"> <br>
            <label id = "timeLbl">Timer Till Brake (ms) : </label>
            <input type="number" id="timerVal" min="0" max="10000"> <br>
            <input id = "hardcodeGoBtn" type = "button" value="Go"> 
            <input id = "hardcodeStopBtn" type = "button" value="Stop"> 
            <input id = "timerHardCode" type = "button" value="TEST"> 
        </div>
    </div>

    <!-- Car control page -->
    <div id = "carControl">
        <div id = "carStick" style="border: 0px solid red; width: 128px;">
            <img src="images/joystick-base.png" id = 'joyBase'/>
            <div id="joyStick" style="position: relative; left:32px; top:-104px;">
                <img src="images/joystick-red.png"/>		
            </div>
        </div>
        <hr>
        <div id = "carStatsBackFromCar">
            <div id = "carControlBtnDiv">
                <input id = "toIndexCarControl" type = "button" value="To Index"> 
                <input id = "hardCodedTesting" type = "button" value="To Fixed Testing">
            </div>
            <!-- <label id = "Approx_Ground_Speed">Approx Ground Speed: 0</label> -->
            <label id = "RPMS_lbl">RPMs:</label>
            <label id = "FL_RPM">0</label>
            <label id = "BL_RPM">0</label>
            <label id = "FR_RPM">0</label>
            <label id = "BR_RPM">0</label>
            <label id = "TC_ACTIVE">TC Not Active</label>
            <label id = "ABS_ACTIVE">ABS Not Active</label>
            <label id = "Burnout_Active">Burnout Not Active</label>
        </div>

        <div id = "sentToCarStats">
            <div id="xyCoords" style="color: red;">Joystick</div>
            <div id = XYFeedBackTest>
                Inteded Angle: 0 || Intended Speed: 0
            </div> 

            <div id = tcSliderDiv>
                <label id = "tcSliderLbl">TC Level [5] </label>
                <input type="range" min="0" max="10" value="5" id="tcSlider" class = "carControlSlider">
            </div>

            <div id = absSliderDiv>
            <label id = "absSliderLbl">ABS Level [5]</label>
                <input type="range" min="0" max="10" value="5" id="absSlider" class = "carControlSlider">
            </div>

            <div id = brakeStrengthDiv>
                <label id = "brakeStrengthSlider">Brake Strength [10]</label>
                <input type="range" min="0" max="10" value="10" id="brakeSlider" class = "carControlSlider">
            </div>

            <div id = connectionStatus></div>
        </div>

        <!-- Credit to https://canvas-gauges.com/documentation/user-guide/ -->
        <!-- Add Gauge -->
        <div id = "speedGaugeDiv">
            <canvas id = "speedGauge" data-type="radial-gauge"
                data-width="400"
                data-height="400"
                data-units="Km/h"
                data-title="false"
                data-value="0"
                data-min-value="0"
                data-max-value="18"
                data-major-ticks="0,2,4,6,8,10,12,14,16,18"
                data-minor-ticks="2"
                data-value-int="2"
                data-stroke-ticks="false"
                data-highlights='[
                    { "from": 0, "to": 6, "color": "rgba(0,255,0,.15)" },
                    { "from": 6, "to": 12, "color": "rgba(255,255,0,.15)" },
                    { "from": 12, "to": 18, "color": "rgba(255,30,0,.25)" }
                ]'
                data-color-plate="#222"
                data-color-major-ticks="#f5f5f5"
                data-color-minor-ticks="#ddd"
                data-color-title="#fff"
                data-color-units="#ccc"
                data-color-numbers="#eee"
                data-color-needle-start="rgba(240, 128, 128, 1)"
                data-color-needle-end="rgba(255, 160, 122, .9)"
                data-value-box="true"
                data-animation-rule="bounce"
                data-animation-duration="500"
                data-font-value="Led"
                data-animated-value="true">
            </canvas>
        </div>
    </div>

    <div class="text-center">
    <br>&copy Copyright 2020 by Jesse Federkiewicz & Timothy Hachey<br/>
      <script>document.write('Last Modified:' + document.lastModified);</script>
    </div>

</div>
</body>
</html>


