--SQL User Creation
--Author Jesse Federkiewicz

CREATE TABLE IF NOT EXISTS `users` (
  `userID` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(24) NOT NULL UNIQUE,
  `password` varchar(64) NOT NULL,
  PRIMARY KEY (`userID`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;


INSERT INTO `users` (`userID`, `username`, `password`) VALUES
(1, 'admin', '$2y$10$/IITqiCPEgZ1X6Bf9kJS..w7Lb6qPaY/9BBS6kyqokeJbir/yUhTS')

