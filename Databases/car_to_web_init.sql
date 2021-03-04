
SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

CREATE TABLE IF NOT EXISTS `car_to_web` (
  `carID` int NOT NULL AUTO_INCREMENT,
  `userID` int NOT NULL,
  `FL_RPM` int NOT NULL,
  `BL_RPM` int NOT NULL,
  `FR_RPM` int NOT NULL,
  `BR_RPM` int NOT NULL,
  `Ground_Speed_Count` int NOT NULL,
  `TC_ACTIVE` int NOT NULL,
  `ABS_ACTIVE` int NOT NULL,
  `Burnout_In_Progress` int NOT NULL,
  PRIMARY KEY (`carID`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

INSERT INTO `car_to_web` (`carID`, `userID`, `FL_RPM`, `BL_RPM`, `FR_RPM`, `BR_RPM`, `Ground_Speed_Count`, `TC_ACTIVE`, `ABS_ACTIVE`, `Burnout_In_Progress`)
VALUES
	(1, 1, 0, 0, 0, 0, 0, 0, 0, 0)

-- CREATE TABLE IF NOT EXISTS `car_to_web` (
  -- `carID` int NOT NULL AUTO_INCREMENT,
  -- `userID` int NOT NULL,
  -- `FL_RPM` int NOT NULL,
  -- `BL_RPM` int NOT NULL,
  -- `FR_RPM` int NOT NULL,
  -- `BR_RPM` int NOT NULL,
  -- `Ground_Speed_Count` int NOT NULL,
  -- `TC_ACTIVE` int NOT NULL,
  -- `ABS_ACTIVE` int NOT NULL,
  -- `Burnout_In_Progress` int NOT NULL,
  -- PRIMARY KEY (`carID`),
  -- FOREIGN KEY (`userID`) REFERENCES users (`userID`) ON DELETE CASCADE
-- ) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- INSERT INTO `car_to_web` (`carID`, `userID`, `FL_RPM`, `BL_RPM`, `FR_RPM`, `BR_RPM`, `Ground_Speed_Count`, `TC_ACTIVE`, `ABS_ACTIVE`, `Burnout_In_Progress`)
-- VALUES
-- (1, 1, 0, 0, 0, 0, 0, 0, 0, 0)

-- ALTER TABLE `car_to_web`
-- ADD FOREIGN KEY (`carID`) REFERENCES `web_to_car`(`carID`) ON DELETE CASCADE


-- CREATE TRIGGER insert_car_across_tables
-- AFTER INSERT
-- ON `web_to_car` FOR EACH ROW
	-- INSERT INTO `car_to_web` (`carID`, `FL_RPM`, `BL_RPM`, `FR_RPM`, `BR_RPM`, `Ground_Speed_Count`, `TC_ACTIVE`, `ABS_ACTIVE`, `BURNOUT_IN_PROGRESS`)
	-- VALUES 
    	-- (new.carID, 0, 0, 0, 0, 0, 0, 0, 0);

