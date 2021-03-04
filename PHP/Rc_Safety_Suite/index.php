<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Tree Directories</title>
    <script src="//ajax.googleapis.com/ajax/libs/jquery/3.5.1/jquery.min.js"></script>
    <style>
      @import url(//fonts.googleapis.com/css?family=Comfortaa|Raleway|Special+Elite|McLaren&effect=3d);
      body { font-family: McLaren, sans-serif; font-size: large; }
      input { font-family: "Special Elite", sans-serif; font-size: large; }
      h1 { font-family: Comfortaa, sans-serif; font-size: 3.5em; }
      .hdr { color: #fff; background-color: #00f; margin: 0px; padding: 20px; }
      .site { display: grid; grid-template-columns : 2fr 3fr; margin: 20px; grid-gap: 20px; }
      .site div { padding : 10px; background-color : #ddd; }
      .text-center { text-align : center; }
    </style>
</head>
<body>
    <?php   
        listFolderFiles('.');
    ?>

</body>
</html>
</body>
</html>

<?php
function listFolderFiles($dir){
    $ffs = scandir($dir);

    unset($ffs[array_search('.', $ffs, true)]);
    unset($ffs[array_search('..', $ffs, true)]);

    // prevent empty ordered elements
    if (count($ffs) < 1)
        return;

    echo '<ol>';
    foreach($ffs as $ff){
    {
        echo "<li><a href=\"./{$dir}/{$ff}\">{$ff}</a>";
        if(is_dir($dir.'/'.$ff)) listFolderFiles($dir.'/'.$ff);
            echo '</li>';
    }
        
}
    echo '</ol>';
}
?>