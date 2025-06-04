-- MySQL dump 10.13  Distrib 8.0.40, for Win64 (x86_64)
--
-- Host: localhost    Database: car_parking_reservation_system
-- ------------------------------------------------------
-- Server version	8.0.40

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `car`
--

DROP TABLE IF EXISTS `car`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `car` (
  `CarID` varchar(8) NOT NULL,
  `UserID` varchar(8) NOT NULL,
  `CarModel` varchar(20) NOT NULL,
  `CarLicensePlate` varchar(8) NOT NULL,
  `CarDate` date NOT NULL,
  PRIMARY KEY (`CarID`),
  KEY `UserID_idx` (`UserID`),
  CONSTRAINT `Car_User` FOREIGN KEY (`UserID`) REFERENCES `user` (`UserID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `car`
--

LOCK TABLES `car` WRITE;
/*!40000 ALTER TABLE `car` DISABLE KEYS */;
INSERT INTO `car` VALUES ('C0000001','U0000002','Proton Saga','MDI927','2024-11-20'),('C0000002','U0000005','Toyota Hilux','JIK2983','2025-01-08'),('C0000003','U0000005','Toyota Hilux','JIK2983','2025-01-08'),('C0000004','U0000005','Toyota Hilux','JIK2983','2025-01-08'),('C0000005','U0000005','Toyota Hilux','JIK2983','2025-01-08'),('C0000006','U0000003','KIA','AJK1234','2024-01-09'),('C0000007','U0000003','KIA','AJK1234','2024-01-09'),('C0000008','U0000003','KIA','AKL1234','2024-01-09'),('C0000009','U0000003','KIA','AKL1234','2024-01-09'),('C0000010','U0000003','KIA','AKL1234','2024-01-09'),('C0000011','U0000003','KIA','AKL1234','2024-01-09'),('C0000012','U0000003','KIA','AKL1234','2024-01-09'),('C0000013','U0000003','KIA','AKL1234','2024-01-09'),('C0000014','U0000003','KIA','AKL1234','2024-01-09'),('C0000015','U0000003','KIA','AJK1234','2024-01-09'),('C0000016','U0000003','KIA','AJK1234','2024-01-09'),('C0000017','U0000003','KIA','AJK1234','2024-01-09'),('C0000018','U0000003','KIA','AJK1234','2024-12-21'),('C0000019','U0000003','KIA','AJK1234','2025-01-22'),('C0000020','U0000005','Toyota Hilux','JIK2983','2025-01-23'),('C0000021','U0000004','Myvi','BIF2083','2025-01-24'),('C0000022','U0000005','Toyota Hilux','JIK2983','2025-01-24'),('C0000023','U0000009','Honda City','MNB123','2025-01-24'),('C0000024','U0000012','KIA','MLD536','2025-01-24');
/*!40000 ALTER TABLE `car` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `fines`
--

DROP TABLE IF EXISTS `fines`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `fines` (
  `FineID` varchar(8) NOT NULL,
  `UserID` varchar(8) NOT NULL,
  `FineAmount` decimal(10,2) NOT NULL,
  `FineReason` text NOT NULL,
  `FineStatus` varchar(10) NOT NULL DEFAULT 'No/Yes',
  `FineDate` varchar(10) NOT NULL,
  PRIMARY KEY (`FineID`),
  KEY `Fines_Users_idx` (`UserID`),
  CONSTRAINT `Fines_Users` FOREIGN KEY (`UserID`) REFERENCES `user` (`UserID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `fines`
--

LOCK TABLES `fines` WRITE;
/*!40000 ALTER TABLE `fines` DISABLE KEYS */;
INSERT INTO `fines` VALUES ('F0000001','U0000002',5.00,'Parking in a parking place not designated for the use of that class of vehicle.','Yes','2024-11-20'),('F0000002','U0000005',10.00,'Obstructing parking space','Yes','2025-01-08'),('F0000003','U0000003',5.00,'Using a parking place for unauthorised purpose.','Yes','2025-01-09'),('F0000004','U0000003',5.00,'Parking in a parking place not designated for the use of that class of vehicle.','Yes','2024-12-09'),('F0000005','U0000003',150.00,'Using a parking place for unauthorised purpose.','Yes','2024-01-09'),('F0000006','U0000003',100.00,'Parking in a parking place not designated for the use of that class of vehicle.','Yes','2024-02-09'),('F0000007','U0000003',250.00,'Obstructing parking space','Yes','2024-03-09'),('F0000008','U0000003',70.00,'OverTime','Yes','2024-04-09'),('F0000009','U0000003',50.00,'Using a parking place for unauthorised purpose.','Yes','2024-05-09'),('F0000010','U0000003',130.00,'Obstructing parking space','Yes','2024-06-09'),('F0000011','U0000003',120.00,'Using a parking place for unauthorised purpose.','Yes','2024-07-09'),('F0000012','U0000003',100.00,'OverTime','Yes','2024-08-09'),('F0000013','U0000003',200.00,'OverTime','Yes','2024-09-09'),('F0000014','U0000003',170.00,'Using a parking place for unauthorised purpose.','Yes','2024-10-09'),('F0000015','U0000003',50.00,'Obstructing parking space','Yes','2024-11-09'),('F0000016','U0000003',70.00,'Using a parking place for unauthorised purpose.','Yes','2024-12-09'),('F0000017','U0000009',50.00,'Parking in a parking place not designated for the use of that class of vehicle.','No','2025-01-24');
/*!40000 ALTER TABLE `fines` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `parkingslot`
--

DROP TABLE IF EXISTS `parkingslot`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `parkingslot` (
  `SlotID` varchar(8) NOT NULL,
  `SlotName` varchar(10) NOT NULL,
  `SlotTime` varchar(10) NOT NULL,
  `SlotStatus` enum('Available','Booked') NOT NULL,
  PRIMARY KEY (`SlotID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `parkingslot`
--

LOCK TABLES `parkingslot` WRITE;
/*!40000 ALTER TABLE `parkingslot` DISABLE KEYS */;
INSERT INTO `parkingslot` VALUES ('S001','A001','10am-11am','Available'),('S002','A001','11am-12pm','Booked'),('S003','A001','12pm-1pm','Available'),('S004','A001','1pm-2pm','Available'),('S005','A001','2pm-3pm','Available'),('S006','A001','3pm-4pm','Available'),('S007','A001','4pm-5pm','Available'),('S008','A001','5pm-6pm','Available'),('S009','A001','6pm-7pm','Available'),('S010','A001','7pm-8pm','Available'),('S011','A001','8pm-9pm','Available'),('S012','A001','9pm-10pm','Available'),('S013','A002','10am-11am','Available'),('S014','A002','11am-12pm','Booked'),('S015','A002','12pm-1pm','Booked'),('S016','A002','1pm-2pm','Available'),('S017','A002','2pm-3pm','Available'),('S018','A002','3pm-4pm','Available'),('S019','A002','4pm-5pm','Available'),('S020','A002','5pm-6pm','Available'),('S021','A002','6pm-7pm','Available'),('S022','A002','7pm-8pm','Available'),('S023','A002','8pm-9pm','Available'),('S024','A002','9pm-10pm','Available'),('S025','A003','10am-11am','Booked'),('S026','A003','11am-12pm','Booked'),('S027','A003','12pm-1pm','Available'),('S028','A003','1pm-2pm','Available'),('S029','A003','2pm-3pm','Available'),('S030','A003','3pm-4pm','Available'),('S031','A003','4pm-5pm','Available'),('S032','A003','5pm-6pm','Available'),('S033','A003','6pm-7pm','Available'),('S034','A003','7pm-8pm','Available'),('S035','A003','8pm-9pm','Available'),('S036','A003','9pm-10pm','Available'),('S037','A004','10am-11am','Available'),('S038','A004','11am-12pm','Available'),('S039','A004','12pm-1pm','Available'),('S040','A004','1pm-2pm','Available'),('S041','A004','2pm-3pm','Available'),('S042','A004','3pm-4pm','Available'),('S043','A004','4pm-5pm','Available'),('S044','A004','5pm-6pm','Available'),('S045','A004','6pm-7pm','Available'),('S046','A004','7pm-8pm','Available'),('S047','A004','8pm-9pm','Available'),('S048','A004','9pm-10pm','Available'),('S049','A005','10am-11am','Available'),('S050','A005','11am-12pm','Available'),('S051','A005','12pm-1pm','Available'),('S052','A005','1pm-2pm','Available'),('S053','A005','2pm-3pm','Booked'),('S054','A005','3pm-4pm','Booked'),('S055','A005','4pm-5pm','Available'),('S056','A005','5pm-6pm','Available'),('S057','A005','6pm-7pm','Available'),('S058','A005','7pm-8pm','Available'),('S059','A005','8pm-9pm','Available'),('S060','A005','9pm-10pm','Available'),('S061','A006','10am-11am','Available'),('S062','A006','11am-12pm','Available'),('S063','A006','12pm-1pm','Available'),('S064','A006','1pm-2pm','Available'),('S065','A006','2pm-3pm','Available'),('S066','A006','3pm-4pm','Available'),('S067','A006','4pm-5pm','Available'),('S068','A006','5pm-6pm','Available'),('S069','A006','6pm-7pm','Available'),('S070','A006','7pm-8pm','Available'),('S071','A006','8pm-9pm','Available'),('S072','A006','9pm-10pm','Available'),('S073','A007','10am-11am','Available'),('S074','A007','11am-12pm','Available'),('S075','A007','12pm-1pm','Available'),('S076','A007','1pm-2pm','Available'),('S077','A007','2pm-3pm','Available'),('S078','A007','3pm-4pm','Available'),('S079','A007','4pm-5pm','Available'),('S080','A007','5pm-6pm','Available'),('S081','A007','6pm-7pm','Available'),('S082','A007','7pm-8pm','Available'),('S083','A007','8pm-9pm','Available'),('S084','A007','9pm-10pm','Available'),('S085','A008','10am-11am','Available'),('S086','A008','11am-12pm','Available'),('S087','A008','12pm-1pm','Available'),('S088','A008','1pm-2pm','Available'),('S089','A008','2pm-3pm','Available'),('S090','A008','3pm-4pm','Available'),('S091','A008','4pm-5pm','Available'),('S092','A008','5pm-6pm','Available'),('S093','A008','6pm-7pm','Available'),('S094','A008','7pm-8pm','Available'),('S095','A008','8pm-9pm','Available'),('S096','A008','9pm-10pm','Available'),('S097','A009','10am-11am','Available'),('S098','A009','11am-12pm','Available'),('S099','A009','12pm-1pm','Available'),('S100','A009','1pm-2pm','Available'),('S101','A009','2pm-3pm','Available'),('S102','A009','3pm-4pm','Available'),('S103','A009','4pm-5pm','Available'),('S104','A009','5pm-6pm','Available'),('S105','A009','6pm-7pm','Available'),('S106','A009','7pm-8pm','Available'),('S107','A009','8pm-9pm','Available'),('S108','A009','9pm-10pm','Available');
/*!40000 ALTER TABLE `parkingslot` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `payment`
--

DROP TABLE IF EXISTS `payment`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `payment` (
  `PaymentID` varchar(8) NOT NULL,
  `PaymentAmount` decimal(10,2) NOT NULL,
  `PaymentType` varchar(20) NOT NULL,
  `ReservationID` varchar(8) NOT NULL,
  `PaymentStatus` varchar(45) NOT NULL DEFAULT 'Pending',
  `PaymentDate` date NOT NULL,
  PRIMARY KEY (`PaymentID`),
  KEY `ReservationID_idx` (`ReservationID`),
  CONSTRAINT `Payment_Reservation` FOREIGN KEY (`ReservationID`) REFERENCES `reservations` (`ReservationID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `payment`
--

LOCK TABLES `payment` WRITE;
/*!40000 ALTER TABLE `payment` DISABLE KEYS */;
INSERT INTO `payment` VALUES ('P0000001',1.00,'Credit Card','R0000001','Completed','2024-11-20'),('P0000002',2.00,'E-Wallet','R0000002','Completed','2025-01-08'),('P0000003',1.00,'E-Wallet','R0000003','Completed','2025-01-08'),('P0000004',300.00,'E-Wallet','R0000004','Completed','2025-01-08'),('P0000005',200.00,'E-Wallet','R0000005','Completed','2025-01-08'),('P0000006',1000.00,'Credit Card','R0000006','Completed','2024-01-09'),('P0000007',500.00,'Credit Card','R0000007','Completed','2024-02-09'),('P0000008',700.00,'E-Wallet','R0000008','Completed','2024-03-09'),('P0000009',1200.00,'Bank Transfer','R0000009','Completed','2024-04-09'),('P0000010',680.00,'Credit Card','R0000010','Completed','2024-05-09'),('P0000011',900.00,'E-Wallet','R0000011','Completed','2024-06-09'),('P0000012',600.00,'E-Wallet','R0000012','Completed','2024-07-09'),('P0000013',500.00,'Bank Transfer','R0000013','Completed','2024-08-09'),('P0000014',900.00,'Bank Transfer','R0000014','Completed','2024-09-09'),('P0000015',800.00,'Credit Card','R0000015','Completed','2024-10-09'),('P0000016',870.00,'E-Wallet','R0000016','Completed','2024-11-09'),('P0000017',750.00,'E-Wallet','R0000017','Completed','2024-12-09'),('P0000018',2.00,'Credit Card','R0000018','Completed','2025-01-21'),('P0000019',2.00,'Bank Transfer','R0000019','Completed','2025-01-22'),('P0000020',2.00,'E-Wallet','R0000020','Completed','2025-01-23'),('P0000021',2.00,'Bank Transfer','R0000021','Completed','2025-01-24'),('P0000022',2.00,'Bank Transfer','R0000022','Completed','2025-01-24'),('P0000023',2.00,'E-Wallet','R0000023','Completed','2025-01-24');
/*!40000 ALTER TABLE `payment` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `reservations`
--

DROP TABLE IF EXISTS `reservations`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `reservations` (
  `ReservationID` varchar(8) NOT NULL,
  `UserID` varchar(8) NOT NULL,
  `TotalTimeReserved` varchar(4) NOT NULL,
  `ReservationDate` date NOT NULL,
  `ReservedSlotName` varchar(50) NOT NULL,
  `ReservedTimeSlot` varchar(50) NOT NULL,
  PRIMARY KEY (`ReservationID`),
  KEY `Reservation_User_idx` (`UserID`),
  CONSTRAINT `Reservation_User` FOREIGN KEY (`UserID`) REFERENCES `user` (`UserID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `reservations`
--

LOCK TABLES `reservations` WRITE;
/*!40000 ALTER TABLE `reservations` DISABLE KEYS */;
INSERT INTO `reservations` VALUES ('R0000001','U0000002','1','2024-11-20','A001','10am-11am'),('R0000002','U0000005','2','2025-01-08','A001','11am-12pm, 12pm-1pm'),('R0000003','U0000005','1','2025-01-08','A001','1pm-2pm'),('R0000004','U0000005','1','2025-01-08','A001','1pm-2pm'),('R0000005','U0000005','1','2025-01-08','A001','1pm-2pm'),('R0000006','U0000003','1','2024-01-09','A002','10am-11am'),('R0000007','U0000003','1','2024-01-09','A002','11am-12pm'),('R0000008','U0000003','1','2024-02-09','A002','12pm-1pm'),('R0000009','U0000003','1','2024-03-09','A002','1pm-2pm'),('R0000010','U0000003','1','2024-04-09','A002','2pm-3pm'),('R0000011','U0000003','1','2024-05-09','A002','3pm-4pm'),('R0000012','U0000003','1','2024-06-09','A002','4pm-5pm'),('R0000013','U0000003','1','2024-07-09','A002','5pm-6pm'),('R0000014','U0000003','1','2024-08-09','A002','6pm-7pm'),('R0000015','U0000003','1','2024-09-09','A002','7pm-8pm'),('R0000016','U0000003','1','2024-10-09','A002','8pm-9pm'),('R0000017','U0000003','1','2024-11-09','A002','9pm-10pm'),('R0000018','U0000003','2','2024-12-21','A001','11am-12pm, 12pm-1pm'),('R0000019','U0000003','2','2025-01-22','A001','1pm-2pm, 2pm-3pm'),('R0000020','U0000005','2','2025-01-23','A005','10am-11am, 11am-12pm'),('R0000021','U0000004','2','2025-01-24','A003','10am-11am, 11am-12pm'),('R0000022','U0000005','2','2025-01-24','A002','11am-12pm, 12pm-1pm'),('R0000023','U0000009','2','2025-01-24','A005','2pm-3pm, 3pm-4pm');
/*!40000 ALTER TABLE `reservations` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user`
--

DROP TABLE IF EXISTS `user`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `user` (
  `UserID` varchar(8) NOT NULL,
  `UserName` varchar(50) NOT NULL,
  `UserPassword` varchar(20) NOT NULL,
  `UserPhoneNo` varchar(20) NOT NULL,
  `UserGender` varchar(2) NOT NULL,
  `UserStatus` enum('User','Admin') NOT NULL,
  PRIMARY KEY (`UserID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user`
--

LOCK TABLES `user` WRITE;
/*!40000 ALTER TABLE `user` DISABLE KEYS */;
INSERT INTO `user` VALUES ('U0000001','le jie le','jiale123!','0123046738','M','Admin'),('U0000002','lee jun kiat','abc12346','0127364733','M','User'),('U0000003','wong shan le','shanle123','01246667856','M','User'),('U0000004','thim wong','abc123','0123456788','F','User'),('U0000005','alex chooi','alex3456','0111345848','M','User'),('U0000006','william chong','abc123!','0123456789','F','User'),('U0000007','chen jin han','abcd1234','01485757596','F','User'),('U0000008','eldhon hong ','eldhong','01156682732','F','User'),('U0000009','ding jia jun','jiajun3103','01137777080','M','User'),('U0000010','tan yi xin','yixin123','0123987478','M','Admin'),('U0000011','lim jia xuan','jiaxuan123','0137564865','F','Admin'),('U0000012','ding jun jun','jiajun3103','0123456789','M','User');
/*!40000 ALTER TABLE `user` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-01-24  8:54:08
