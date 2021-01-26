--SQL User Creation
--Author Jesse Federkiewicz

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

CREATE TABLE IF NOT EXISTS `users` (
  `userID` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(24) NOT NULL,
  `password` varchar(64) NOT NULL,
  PRIMARY KEY (`userID`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=8 ;

--admin god for default, will likely change once add user functionality is added straight from webpage
INSERT INTO `users` (`userID`, `username`, `password`) VALUES
(1, 'admin', '$2y$10$ZbET3KxW5.G/j8VEm7O1TOMPlTu7KNRv2Iq3VtdopEEm8hXbZkd5C'),

