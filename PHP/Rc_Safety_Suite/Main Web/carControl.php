<!DOCTYPE html>
<html lang="en">
<head>
<link rel="stylesheet" href="style.css">
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <script src="//ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
    <script src="./js/carControl.js" type="text/javascript"></script>
    <script src = "./js/ajax.js"></script>
</head>
<body>
    <div class = hdr>
        <h1 class = 'font-effect-3d text-center' id = carControlBanner>Select a car to control</h1>
    </div>
    <div id = chooseCar>
        <div id = selectCarNumber>
        </div>

        <div id = selectCarBtn>
            <!-- <input type = "button" name = "selCarBtn" value = "Lets Control This thing!" id = selCarBtnID> -->
        </div>

        <div id = addCarBtnDIV>
        </div>
    </div>

    <div id = "carControl">
        <div style="border: 1px solid red; width: 128px; position: absolute; left:10px; top:150px;">
            <img src="images/joystick-base.png" id = 'joyBase'/>
        <div id="joyStick" style="position: absolute; left:32px; top:32px;">
        <img src="images/joystick-red.png"/>		
        </div>
        </div>
        <hr>
        <div id="status1" style="color: red;">Joystick</div>

        <div id = connectionStatus>
        </div>

        <div id = XYMAX>
        
        </div>
        <div id = XYFeedBackTest>
        HERE
        </div> 
    </div>
    

</div>
</body>
</html>


