--SQL web_to_car Creation
--Author Jesse Federkiewicz

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

-- CREATE TABLE IF NOT EXISTS `web_to_car` (
  -- `carID` int NOT NULL AUTO_INCREMENT,
  -- `xCoord` int NOT NULL,
  -- `yCoord` int NOT NULL,
  -- `timeStamp` int NOT NULL,
  -- PRIMARY KEY (`carID`)
-- ) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- INSERT INTO `web_to_car` (`carID`, `xCoord`, `yCoord`, `timeStamp`) VALUES
-- (1, 0, 0, 0)



CREATE TABLE IF NOT EXISTS `web_to_car` (
  `carID` int NOT NULL AUTO_INCREMENT,
  `userID` int NOT NULL,
  `intendedAngle` int NOT NULL,
  `intendedSpeed` int NOT NULL,
  `timeStamp` int NOT NULL,
  PRIMARY KEY (`carID`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

INSERT INTO `web_to_car` (`carID`, `userID`,`intendedAngle`, `intendedSpeed`, `timeStamp`) VALUES
(1, 1, 0, 0, 0)

-- CREATE TABLE IF NOT EXISTS `web_to_car` (
  -- `carID` int NOT NULL AUTO_INCREMENT,
  -- `userID` int NOT NULL,
  -- `intendedAngle` int NOT NULL,
  -- `intendedSpeed` int NOT NULL,
  -- `timeStamp` int NOT NULL,
  -- PRIMARY KEY (`carID`),
  -- FOREIGN KEY (`userID`) REFERENCES users (`userID`) ON DELETE CASCADE
	
-- ) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- INSERT INTO `web_to_car` (`carID`, `userID`,`intendedAngle`, `intendedSpeed`, `timeStamp`) VALUES
-- (1, 1, 0, 0, 0)





--in use
CREATE TABLE IF NOT EXISTS `web_to_car` (
  `carID` int NOT NULL AUTO_INCREMENT,
  `userID` int NOT NULL,
  `intendedAngle` int NOT NULL,
  `intendedSpeed` int NOT NULL,
  `timeStamp` int NOT NULL,
  PRIMARY KEY (`carID`)
	
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

INSERT INTO `web_to_car` (`carID`, `userID`,`intendedAngle`, `intendedSpeed`, `timeStamp`) VALUES
(1, 1, 0, 0, 0)