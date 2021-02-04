--SQL web_to_car Creation
--Author Jesse Federkiewicz

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

CREATE TABLE IF NOT EXISTS `web_to_car` (
  `carID` int NOT NULL AUTO_INCREMENT,
  `xCoord` int(4) NOT NULL,
  `yCoord` int(4) NOT NULL,
  `timeStamp` int(6) NOT NULL,
  PRIMARY KEY (`carID`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;
