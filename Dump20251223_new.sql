CREATE DATABASE  IF NOT EXISTS `themost_db` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;
USE `themost_db`;
-- MySQL dump 10.13  Distrib 8.0.43, for Win64 (x86_64)
--
-- Host: localhost    Database: themost_db
-- ------------------------------------------------------
-- Server version	8.0.43

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
-- Table structure for table `brands`
--

DROP TABLE IF EXISTS `brands`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `brands` (
  `brand_id` int NOT NULL AUTO_INCREMENT COMMENT '브랜드 PK',
  `brand_name` varchar(50) NOT NULL COMMENT '브랜드 이름',
  `brand_code` varchar(10) DEFAULT NULL COMMENT '브랜드 약어 코드 (예: SP, NK)',
  PRIMARY KEY (`brand_id`),
  UNIQUE KEY `brand_name` (`brand_name`),
  UNIQUE KEY `brand_code` (`brand_code`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='브랜드 정보';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `brands`
--

LOCK TABLES `brands` WRITE;
/*!40000 ALTER TABLE `brands` DISABLE KEYS */;
INSERT INTO `brands` VALUES (1,'자체제작','JJ'),(2,'슈프림','SP'),(3,'나이키','NK'),(4,'아디다스','AD');
/*!40000 ALTER TABLE `brands` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `cart`
--

DROP TABLE IF EXISTS `cart`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `cart` (
  `cart_id` bigint NOT NULL AUTO_INCREMENT COMMENT '장바구니 PK',
  `member_id` bigint NOT NULL COMMENT '회원 FK',
  `option_id` bigint NOT NULL COMMENT '상품 옵션 FK',
  `quantity` int NOT NULL DEFAULT '1' COMMENT '수량',
  PRIMARY KEY (`cart_id`),
  KEY `member_id` (`member_id`),
  KEY `option_id` (`option_id`),
  CONSTRAINT `cart_ibfk_1` FOREIGN KEY (`member_id`) REFERENCES `members` (`member_id`) ON DELETE CASCADE,
  CONSTRAINT `cart_ibfk_2` FOREIGN KEY (`option_id`) REFERENCES `product_options` (`option_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='장바구니';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `cart`
--

LOCK TABLES `cart` WRITE;
/*!40000 ALTER TABLE `cart` DISABLE KEYS */;
/*!40000 ALTER TABLE `cart` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `categories`
--

DROP TABLE IF EXISTS `categories`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `categories` (
  `category_id` int NOT NULL AUTO_INCREMENT COMMENT '카테고리 PK',
  `category_name` varchar(50) NOT NULL COMMENT '카테고리 이름 (상의, 하의 등)',
  `category_code` varchar(10) DEFAULT NULL COMMENT '카테고리 약어 코드 (예: TS, PT)',
  PRIMARY KEY (`category_id`),
  UNIQUE KEY `category_name` (`category_name`),
  UNIQUE KEY `category_code` (`category_code`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='상품 카테고리 정보';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `categories`
--

LOCK TABLES `categories` WRITE;
/*!40000 ALTER TABLE `categories` DISABLE KEYS */;
INSERT INTO `categories` VALUES (1,'Top','TP'),(2,'Bottom','BT'),(3,'Outer','OT'),(4,'Inner','IN'),(5,'Shoes','SH');
/*!40000 ALTER TABLE `categories` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `category_sizes`
--

DROP TABLE IF EXISTS `category_sizes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `category_sizes` (
  `category_id` int NOT NULL COMMENT '카테고리 FK',
  `size_id` int NOT NULL COMMENT '사이즈 FK',
  PRIMARY KEY (`category_id`,`size_id`),
  KEY `size_id` (`size_id`),
  CONSTRAINT `category_sizes_ibfk_1` FOREIGN KEY (`category_id`) REFERENCES `categories` (`category_id`) ON DELETE CASCADE,
  CONSTRAINT `category_sizes_ibfk_2` FOREIGN KEY (`size_id`) REFERENCES `sizes` (`size_id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='카테고리별 사용 가능 사이즈 연결 정보';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `category_sizes`
--

LOCK TABLES `category_sizes` WRITE;
/*!40000 ALTER TABLE `category_sizes` DISABLE KEYS */;
INSERT INTO `category_sizes` VALUES (1,1),(2,1),(3,1),(4,1),(1,2),(2,2),(3,2),(4,2),(1,3),(2,3),(3,3),(4,3),(1,4),(2,4),(3,4),(4,4),(1,5),(5,6),(5,7),(5,8);
/*!40000 ALTER TABLE `category_sizes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `colors`
--

DROP TABLE IF EXISTS `colors`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `colors` (
  `color_id` int NOT NULL AUTO_INCREMENT COMMENT '색상 PK',
  `color_name` varchar(30) NOT NULL COMMENT '색상 이름',
  `color_code` varchar(10) DEFAULT NULL COMMENT '색상 약어 코드 (예: BK, WH)',
  PRIMARY KEY (`color_id`),
  UNIQUE KEY `color_name` (`color_name`),
  UNIQUE KEY `color_code` (`color_code`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='상품 색상 정보';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `colors`
--

LOCK TABLES `colors` WRITE;
/*!40000 ALTER TABLE `colors` DISABLE KEYS */;
INSERT INTO `colors` VALUES (1,'흰색','WH'),(2,'검정','BK'),(3,'네이비','NV'),(4,'베이지','BE'),(5,'블루','BL'),(6,'그레이','GY'),(7,'레드','RD');
/*!40000 ALTER TABLE `colors` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `members`
--

DROP TABLE IF EXISTS `members`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `members` (
  `member_id` bigint NOT NULL AUTO_INCREMENT COMMENT '회원 PK',
  `username` varchar(100) NOT NULL COMMENT '아이디',
  `password` varchar(255) NOT NULL COMMENT '비밀번호 (해시 암호화 저장)',
  `name` varchar(50) NOT NULL COMMENT '이름',
  `phone` varchar(20) NOT NULL COMMENT '전화번호',
  `address` varchar(255) NOT NULL COMMENT '주소',
  `grade` varchar(20) NOT NULL DEFAULT '일반' COMMENT '회원 등급 (일반, VIP, 관리자)',
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '가입일',
  PRIMARY KEY (`member_id`),
  UNIQUE KEY `username` (`username`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='회원 정보';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `members`
--

LOCK TABLES `members` WRITE;
/*!40000 ALTER TABLE `members` DISABLE KEYS */;
INSERT INTO `members` VALUES (1,'testuser','password123','테스트유저','010-1234-5678','대한민국 서울특별시','일반','2025-10-08 17:20:07'),(2,'admin','admin123','관리자','010-9876-5432','본사 사무실','관리자','2025-10-08 17:20:43'),(3,'kim','1234','김태은','010-2059-3119','경기도 안산시','일반','2025-10-09 17:10:16'),(4,'jeong','1234','정소빈','010-7553-6842','충청남도 아산시','일반','2025-10-09 17:10:16'),(5,'lee','1234','이진희','010-7272-3174','경기도 수원시','일반','2025-10-09 17:10:16'),(6,'lim','1234','임준용','010-5611-7751','경기도 수원시','일반','2025-10-09 17:10:16'),(7,'sung','1234','성요셉','010-6384-2564','경상남도 거창군','vip','2025-10-09 17:10:16'),(8,'beak','1234','백용','010-2305-5672','경기도 수원시','일반','2025-10-09 17:10:16');
/*!40000 ALTER TABLE `members` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `order_details`
--

DROP TABLE IF EXISTS `order_details`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `order_details` (
  `order_detail_id` bigint NOT NULL AUTO_INCREMENT COMMENT '주문 상세 PK',
  `order_id` bigint NOT NULL COMMENT '주문 FK',
  `option_id` bigint NOT NULL COMMENT '상품 옵션 FK',
  `quantity` int NOT NULL COMMENT '주문 수량',
  `price_per_item` int NOT NULL COMMENT '주문 당시 개당 가격',
  PRIMARY KEY (`order_detail_id`),
  KEY `order_id` (`order_id`),
  KEY `option_id` (`option_id`),
  CONSTRAINT `order_details_ibfk_1` FOREIGN KEY (`order_id`) REFERENCES `orders` (`order_id`) ON DELETE CASCADE,
  CONSTRAINT `order_details_ibfk_2` FOREIGN KEY (`option_id`) REFERENCES `product_options` (`option_id`)
) ENGINE=InnoDB AUTO_INCREMENT=35 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='주문 상세 내역';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `order_details`
--

LOCK TABLES `order_details` WRITE;
/*!40000 ALTER TABLE `order_details` DISABLE KEYS */;
INSERT INTO `order_details` VALUES (8,8,86,1,5000),(10,10,128,1,74900),(12,12,65,1,128000),(14,14,86,1,5000),(17,17,104,1,119000),(18,18,128,1,74900),(20,20,64,1,450000),(21,21,19,2,89000),(22,22,20,1,89000),(23,23,113,8,99000),(26,26,128,1,74900),(27,27,128,1,74900),(28,28,64,1,450000),(29,29,89,1,49000),(30,30,115,1,129000),(31,31,100,1,139000),(32,32,64,3,450000),(33,33,15,1,38000),(34,34,15,1,38000);
/*!40000 ALTER TABLE `order_details` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `orders`
--

DROP TABLE IF EXISTS `orders`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `orders` (
  `order_id` bigint NOT NULL AUTO_INCREMENT COMMENT '주문 PK',
  `member_id` bigint NOT NULL COMMENT '회원 FK',
  `order_date` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '주문일',
  `recipient_name` varchar(50) NOT NULL COMMENT '수령인 이름',
  `shipping_address` varchar(255) NOT NULL COMMENT '배송 주소',
  `recipient_phone` varchar(20) NOT NULL COMMENT '수령인 연락처',
  `total_price` int NOT NULL COMMENT '총 결제 금액',
  `status` varchar(20) NOT NULL DEFAULT '결제완료' COMMENT '주문 상태',
  `idempotency_key` varchar(100) DEFAULT NULL COMMENT '멱등성 키 (중복 주문 방지)',
  PRIMARY KEY (`order_id`),
  UNIQUE KEY `idempotency_key` (`idempotency_key`),
  KEY `idx_member_date` (`member_id`,`order_date`),
  KEY `idx_idempotency_key` (`idempotency_key`),
  CONSTRAINT `orders_ibfk_1` FOREIGN KEY (`member_id`) REFERENCES `members` (`member_id`)
) ENGINE=InnoDB AUTO_INCREMENT=35 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='주문 정보';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `orders`
--

LOCK TABLES `orders` WRITE;
/*!40000 ALTER TABLE `orders` DISABLE KEYS */;
INSERT INTO `orders` VALUES (8,4,'2025-10-14 12:38:19','정소빈','충청남도 아산시','010-7553-6842',5000,'결제완료','a54c53be-abc5-49e0-b39a-2e8289d447a2'),(9,3,'2025-10-15 09:41:25','김태은','경기도 안산시','010-2059-3119',249000,'결제완료','7c91c3e8-c31a-4433-857c-ddf488d8dc13'),(10,3,'2025-10-15 09:42:56','김태은','경기도 안산시','010-2059-3119',74900,'결제완료','bab08f00-8cea-419d-91f1-fe35e1b29cf8'),(11,3,'2025-10-15 12:07:57','김태은','경기도 안산시','010-2059-3119',450000,'결제완료','c2bf169e-4f88-469a-84d6-3a63973e8b09'),(12,4,'2025-10-15 12:08:24','정소빈','충청남도 아산시','010-7553-6842',128000,'결제완료','14807857-db73-46db-a9a5-e9539fc8b761'),(13,4,'2025-10-15 17:10:36','정소빈','충청남도 아산시','010-7553-6842',249000,'결제완료','938947c5-e64f-41e9-86e2-67c67d04877a'),(14,3,'2025-10-15 17:17:06','김태은','경기도 안산시','010-2059-3119',5000,'결제완료','03680716-2762-4ad6-9bfa-33eb9bb1390d'),(15,4,'2025-10-15 17:17:49','정소빈','충청남도 아산시','010-7553-6842',249000,'결제완료','cc62453b-c544-4a17-85e9-61771ef5d05d'),(16,3,'2025-10-15 17:21:36','김태은','경기도 안산시','010-2059-3119',249000,'결제완료','37b6d666-d1ac-4402-b503-60c57a41f59c'),(17,3,'2025-10-15 17:25:44','김태은','경기도 안산시','010-2059-3119',119000,'결제완료','cebb9b2e-6af4-444a-a055-07ac82815ae0'),(18,3,'2025-10-15 17:26:07','김태은','경기도 안산시','010-2059-3119',74900,'결제완료','e1bb6d41-d5ce-488f-b208-f8ce38336890'),(19,4,'2025-10-15 17:29:45','정소빈','충청남도 아산시','010-7553-6842',249000,'결제완료','407b4910-dc3f-4a5a-96d0-b8392976e2cf'),(20,7,'2025-10-15 19:48:26','성요셉','경상남도 거창군','010-6384-2564',450000,'결제완료','2a42571a-74fc-4f70-ba73-c9362d770b4c'),(21,7,'2025-10-16 18:34:26','성요셉','경상남도 거창군','010-6384-2564',178000,'결제완료','7f10f52a-5ee6-4399-91ca-c953fed86653'),(22,7,'2025-10-16 18:35:11','성요셉','경상남도 거창군','010-6384-2564',89000,'결제완료','bc6d37c6-3d41-49d3-86e2-b9eceffb5e9e'),(23,7,'2025-10-16 18:36:39','성요셉','경상남도 거창군','010-6384-2564',792000,'결제완료','b656c15e-64f1-443f-bae7-f3aeb6020a7f'),(26,5,'2025-10-17 10:37:33','이진희','경기도 수원시','010-7272-3174',74900,'결제완료','431c4695-7a3b-4235-b910-ae466db73e0a'),(27,5,'2025-10-17 10:46:14','이진희','경기도 수원시','010-7272-3174',74900,'결제완료','43259ae2-1ac5-4103-a26d-36593911b89b'),(28,6,'2025-10-17 11:40:34','임준용','경기도 수원시','010-5611-7751',450000,'결제완료','44b3a798-4f96-4888-aa29-93848bd777d7'),(29,5,'2025-10-17 11:41:20','이진희','경기도 수원시','010-7272-3174',49000,'결제완료','a2c890d5-aed1-4d4b-8b10-ebb59e76a1e0'),(30,7,'2025-11-21 10:13:50','성요셉','경상남도 거창군','010-6384-2564',129000,'결제완료','c3e30fa3-b158-4355-88e9-d2f524c8d163'),(31,5,'2025-11-28 10:39:22','이진희','경기도 수원시','010-7272-3174',139000,'결제완료','2aa4dee8-4fd5-4b77-ae25-9e93ea901468'),(32,5,'2025-11-28 11:01:27','이진희','경기도 수원시','010-7272-3174',1350000,'결제완료','4dc8cf80-6bc3-4e59-9fce-af52ab44806b'),(33,5,'2025-11-28 14:06:22','이진희','경기도 수원시','010-7272-3174',38000,'결제완료','47db3fab-6cee-4082-a770-08f25e7483cc'),(34,7,'2025-12-23 11:54:39','성요셉','경상남도 거창군','010-6384-2564',38000,'결제완료','2b695d8c-00ad-4647-b708-b04b9696f0e7');
/*!40000 ALTER TABLE `orders` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `product_images`
--

DROP TABLE IF EXISTS `product_images`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `product_images` (
  `image_id` bigint NOT NULL AUTO_INCREMENT COMMENT '이미지 PK',
  `product_id` bigint NOT NULL COMMENT '상품 FK',
  `image_url` varchar(255) NOT NULL COMMENT '이미지 파일 경로/URL',
  `is_main_image` tinyint(1) NOT NULL DEFAULT '0' COMMENT '대표 이미지 여부 (TRUE/FALSE)',
  PRIMARY KEY (`image_id`),
  KEY `product_id` (`product_id`),
  CONSTRAINT `product_images_ibfk_1` FOREIGN KEY (`product_id`) REFERENCES `products` (`product_id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=113 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='상품 이미지 정보';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `product_images`
--

LOCK TABLES `product_images` WRITE;
/*!40000 ALTER TABLE `product_images` DISABLE KEYS */;
INSERT INTO `product_images` VALUES (1,1,'https://image.msscdn.net/thumbnails/images/goods_img/20200402/1382658/1382658_17048730367124_big.jpg?w=1200',1),(2,2,'https://img.danawa.com/prod_img/500000/610/626/img/92626610_1.jpg?shrink=360:360&_v=20250618173702',1),(3,3,'https://media.bunjang.co.kr/product/339053763_1_1749776635_w%7Bres%7D.jpg',1),(4,4,'https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcRICPoL4QOKetzPx14jIre_Y9TDcRq1dB3mDg&s',1),(5,5,'https://img.ssfshop.com/cmd/LB_750x1000/src/https://img.ssfshop.com/goods/8SBR/24/02/20/GM0024022056135_0_THNAIL_ORGINL_20240321151113974.jpg',1),(6,6,'https://img.danawa.com/prod_img/500000/852/861/img/9861852_1.jpg?shrink=330:*&_v=20240409105159',1),(7,7,'https://img.croketglobal.net/item/thumbnail/6d7f3f48-742a-4c22-9f08-13df680c0767.webp',1),(8,8,'https://img.danawa.com/prod_img/500000/345/363/img/17363345_1.jpg?shrink=330:*&_v=20250710151436',1),(9,9,'https://image.hm.com/assets/hm/9f/ed/9fedee420f6d71cbc480bcf1043f2e98e590bda2.jpg?imwidth=1260',1),(10,10,'https://before-images-item.croketglobal.net/item/thumbnail/c1f14a69bb0462db20a7fad5793a7a2e.webp',1),(11,11,'https://m.byslim.com/web/product/big/202403/dbc64f4701bc8980659b6b87fdd9af46.jpg',1),(12,12,'https://img.danawa.com/prod_img/500000/063/097/img/19097063_1.jpg?shrink=330:*&_v=20230221154823',1),(13,13,'https://1746b291a6740af9.kinxzone.com/upload/images/product/100/100408/Product_1743210960123.jpg',1),(14,14,'https://sitem.ssgcdn.com/38/95/59/item/1000683599538_i1_1200.jpg',1),(15,15,'https://images.kolonmall.com/Prod_Img/10003159/2023/LM1/K1685694619757066NA01_LM1.jpg',1),(16,16,'https://img.danawa.com/prod_img/500000/028/057/img/16057028_1.jpg?shrink=330:*&_v=20211229130310',1),(17,17,'https://img.ssfshop.com/cmd/LB_750x1000/src/https://img.ssfshop.com/goods/LUXR/25/04/15/GRN725041589444_0_ORGINL_20250415180958339.jpg',1),(18,18,'https://encrypted-tbn2.gstatic.com/shopping?q=tbn:ANd9GcRpXBcWcpPn-qf27bQhSLwz_a8N-YDUPDmywj2ISIvQ2FCFb9BACkYCeM1OvkbZ8eBLS057Uq19pR-6bFBjCuMETA9v8st5bw',1),(19,19,'https://simio.kr/web/product/small/202505/31c1bc611774c421588e6e14210b1952.jpg',1),(20,20,'https://img.danawa.com/prod_img/500000/775/045/img/55045775_1.jpg?shrink=330:*&_v=20240531033231',1),(21,21,'https://image.msscdn.net/thumbnails/images/goods_img/20240112/3790870/3790870_17101368224107_big.jpg?w=1200',1),(22,22,'https://image.msscdn.net/thumbnails/images/goods_img/20250523/5143308/5143308_17484009215236_big.jpg?w=1200',1),(23,23,'https://img.danawa.com/prod_img/500000/702/084/img/20084702_1.jpg?shrink=330:*&_v=20230510032459',1),(24,24,'https://before-images-item.croketglobal.net/item/thumbnail/e75ababbdcb5b97fd5dc66c46de14ac6.webp',1),(25,25,'https://m.thetrillion.co.kr/web/product/big/202403/9ccfef7ef0e1bda46692bc67b0cd09b7.jpg',1),(26,26,'https://static.nike.com/a/images/t_PDP_1728_v1/f_auto,q_auto:eco/cd7d8f32-a7b6-40f1-a09c-0018306188e1/AS+M+ACG+SF+PRMLFT+SKL+PK+JKT.png',1),(27,27,'https://assets.adidas.com/images/h_840,f_auto,q_auto,fl_lossy,c_fill,g_auto/7ed2662ec0dc4dcaa05901d42f3f6f69_9366/Blue_IR9893_01_laydown.jpg',1),(29,29,'https://img.danawa.com/prod_img/500000/187/369/img/6369187_1.jpg?shrink=330:*&_v=20220916111649',1),(30,30,'https://img.danawa.com/prod_img/500000/603/569/img/6569603_1.jpg?shrink=330:*&_v=20200106173221',1),(31,31,'https://image.uniqlo.com/UQ/ST3/AsianCommon/imagesgoods/467556/item/goods_09_467556_3x4.jpg?width=400',1),(32,32,'https://search.pstatic.net/common/?src=http%3A%2F%2Fshopping.phinf.naver.net%2Fmain_4394333%2F43943332151.20231110172046.jpg&type=sc960_832',1),(33,33,'https://static.nike.com/a/images/t_PDP_1728_v1/f_auto,q_auto:eco/ae4ce65f-7fdf-450b-9c11-c6e468e539fd/AS+M+NK+DF+MILER+TANK.png',1),(34,34,'https://image.msscdn.net/thumbnails/images/goods_img/20190322/991339/991339_17138316859815_big.jpg?w=1200',1),(35,35,'https://image.msscdn.net/thumbnails/images/prd_img/20241227/4682859/detail_4682859_17352800527977_big.jpg?w=1200',1),(36,36,'https://image.msscdn.net/thumbnails/images/goods_img/20240321/3975808/3975808_17217990375752_big.jpg?w=1200',1),(37,37,'https://image.msscdn.net/thumbnails/images/goods_img/20220907/2778693/2778693_17137649079340_big.jpg?w=1200',1),(38,38,'https://search.pstatic.net/common/?src=http%3A%2F%2Fshopping.phinf.naver.net%2Fmain_5384599%2F53845995953.20250329195713.jpg&type=sc960_832',1),(39,39,'https://image.msscdn.net/thumbnails/images/goods_img/20240321/3975909/3975909_17115943559810_big.jpg?w=1200',1),(40,40,'https://search.pstatic.net/common/?src=http%3A%2F%2Fshopping.phinf.naver.net%2Fmain_3883357%2F38833574007.20230322155130.jpg&type=sc960_832',1),(41,41,'https://image.msscdn.net/thumbnails/images/goods_img/20240321/3976350/3976350_17115804993997_big.jpg?w=1200',1),(42,42,'https://image.msscdn.net/thumbnails/images/goods_img/20250318/4909415/4909415_17423626172301_big.jpg?w=1200',1),(43,43,'https://image.msscdn.net/thumbnails/images/goods_img/20241004/4485747/4485747_17280125019087_big.jpg?w=1200',1),(44,44,'https://img.soldout.co.kr/item_thumb/2023/10/23/ae15aced-5881-4ba2-9d2e-5676ee1a28e1.png/soldout/resize/1000/optimize',1),(45,45,'https://assets.adidas.com/images/h_840,f_auto,q_auto,fl_lossy,c_fill,g_auto/c1ce1ad09cec4c8399308c8822551c77_9366/5_GTX_Black_JI1384_01_00_standard.jpg',1),(46,46,'https://image.msscdn.net/thumbnails/images/goods_img/20200908/1590038/1590038_2_big.jpg?w=1200',1),(47,47,'https://image.msscdn.net/thumbnails/images/goods_img/20250523/5143223/5143223_17548715684772_big.jpg?w=1200',1),(48,48,'https://image.msscdn.net/thumbnails/images/goods_img/20241212/4662663/4662663_17339817414789_big.jpg?w=1200',1),(49,49,'https://image.msscdn.net/thumbnails/images/goods_img/20200511/1439527/1439527_16817806349116_big.jpg?w=1200',1),(50,50,'https://image.msscdn.net/thumbnails/images/goods_img/20240603/4172816/4172816_17183292632657_big.jpg?w=1200',1),(55,55,'https://images.ptry.im/products/PCF1MCP403DN/lk2.3000.webp',1),(56,56,'https://thumbnail10.coupangcdn.com/thumbnails/remote/320x320ex/image/vendor_inventory/156c/323bbdbbe0989b28c9d965cdd1320951795c75ec20216d72bc737944823f.jpg',1),(57,1,'https://image.msscdn.net/thumbnails/images/prd_img/20200402/1382658/detail_1382658_1_big.jpg?w=1200',0),(58,1,'https://image.msscdn.net/thumbnails/images/prd_img/20200402/1382658/detail_1382658_4_big.jpg?w=1200',0),(59,50,'https://image.msscdn.net/thumbnails/images/prd_img/20240603/4172816/detail_4172816_17183292758711_big.jpg?w=1200',0),(60,50,'https://image.msscdn.net/thumbnails/images/prd_img/20240603/4172816/detail_4172816_17183292774059_big.jpg?w=1200',0),(61,41,'https://image.msscdn.net/thumbnails/images/prd_img/20240321/3976350/detail_3976350_17115805103080_big.jpg?w=1200',0),(62,41,'https://image.msscdn.net/thumbnails/images/prd_img/20240321/3976350/detail_3976350_17115805111767_big.jpg?w=1200',0),(63,47,'https://image.msscdn.net/thumbnails/images/goods_img/20250924/5520349/5520349_17591224535799_big.jpg?w=1200',0),(64,47,'https://image.msscdn.net/thumbnails/images/goods_img/20241216/4668551/4668551_17345036534794_big.jpg?w=1200',0),(65,43,'https://image.msscdn.net/thumbnails/images/goods_img/20241004/4485745/4485745_17280124937422_big.jpg?w=1200',0),(66,43,'https://image.msscdn.net/thumbnails/images/goods_img/20241004/4485744/4485744_17280190604773_big.jpg?w=1200',0),(67,44,'https://img.soldout.co.kr/item_detail/2023/10/23/6d40d244-ea01-4968-8a3f-93d9ec1b0465.png/soldout/resize/1000/optimize',0),(68,44,'https://img.soldout.co.kr/item_detail/2023/10/23/2e871dc1-39ba-442f-8a6b-db6484ebbb0e.png/soldout/resize/1000/optimize',0),(69,42,'https://image.msscdn.net/thumbnails/images/prd_img/20250318/4909415/detail_4909415_17423626232348_big.jpg?w=1200',0),(70,42,'https://image.msscdn.net/thumbnails/images/prd_img/20250318/4909415/detail_4909415_17423626302175_big.jpg?w=1200',0),(71,45,'https://assets.adidas.com/images/h_840,f_auto,q_auto,fl_lossy,c_fill,g_auto/3225d003998f423d95421f8709ac2639_9366/5_GTX_Black_JI1384_06_standard.jpg',0),(72,45,'https://assets.adidas.com/images/h_840,f_auto,q_auto,fl_lossy,c_fill,g_auto/72a2429976984b97869ac6438327eccc_9366/5_GTX_Black_JI1384_02_standard_hover.jpg',0),(73,46,'https://image.msscdn.net/thumbnails/images/prd_img/20200908/1590038/detail_1590038_45_big.jpg?w=1200',0),(74,46,'https://image.msscdn.net/thumbnails/images/prd_img/20200908/1590038/detail_1590038_2_big.jpg?w=1200',0),(75,48,'https://image.msscdn.net/thumbnails/images/prd_img/20241212/4662663/detail_4662663_17339815261680_big.jpg?w=1200',0),(76,48,'https://image.msscdn.net/thumbnails/images/prd_img/20241212/4662663/detail_4662663_17339815265930_big.jpg?w=1200',0),(77,49,'https://image.msscdn.net/thumbnails/images/goods_img/20190819/1120491/1120491_16817806645551_big.jpg?w=1200',0),(78,49,'https://image.msscdn.net/thumbnails/images/prd_img/20190819/1120491/detail_1120491_16817810999800_big.jpg?w=1200',0),(79,34,'https://image.msscdn.net/thumbnails/images/prd_img/20190322/991339/detail_991339_38_big.jpg?w=1200',0),(80,34,'https://image.msscdn.net/thumbnails/images/goods_img/20210511/1945869/1945869_17536865788223_big.jpg?w=1200',0),(81,31,'https://image.uniqlo.com/UQ/ST3/AsianCommon/imagesgoods/467556/item/goods_08_467556_3x4.jpg?width=400',0),(82,31,'https://image.uniqlo.com/UQ/ST3/AsianCommon/imagesgoods/467556/sub/goods_467556_sub13_3x4.jpg?width=400',0),(83,36,'https://image.msscdn.net/thumbnails/images/prd_img/20240321/3975808/detail_3975808_17115941615851_big.jpg?w=1200',0),(84,36,'https://image.msscdn.net/thumbnails/images/goods_img/20240614/4198747/4198747_17217988795104_big.jpg?w=1200',0),(85,39,'https://image.msscdn.net/thumbnails/images/prd_img/20240321/3975909/detail_3975909_17115943657788_big.jpg?w=1200',0),(86,39,'https://image.msscdn.net/thumbnails/images/prd_img/20240321/3975909/detail_3975909_17115943685006_big.jpg?w=1200',0),(87,33,'https://static.nike.com/a/images/t_PDP_1728_v1/f_auto,q_auto:eco/22224302-8d0b-41ea-ac69-7cbe6f23b50b/AS+M+NK+DF+MILER+TANK.png',0),(88,33,'https://static.nike.com/a/images/t_PDP_1728_v1/f_auto,q_auto:eco/09a30314-0a3d-411d-b7bf-cf7fae2d4011/AS+M+NK+DF+MILER+TANK.png',0),(89,35,'https://image.msscdn.net/thumbnails/images/goods_img/20241227/4682859/4682859_17352816432364_big.jpg?w=1200',0),(90,35,'https://image.msscdn.net/thumbnails/images/prd_img/20241227/4682859/detail_4682859_17352800523705_big.jpg?w=1200',0),(91,40,'https://search.pstatic.net/common/?src=http%3A%2F%2Fshop1.phinf.naver.net%2F20201217_74%2F1608174248358RoKfW_JPEG%2F103995_1.jpg&type=sc960_832',0),(92,37,'https://image.msscdn.net/thumbnails/images/prd_img/20220907/2778693/detail_2778693_1_big.jpg?w=1200',0),(93,37,'https://image.msscdn.net/thumbnails/images/prd_img/20220907/2778693/detail_2778693_2_big.jpg?w=1200',0),(94,22,'https://image.msscdn.net/thumbnails/images/prd_img/20250523/5143308/detail_5143308_17484009269656_big.jpg?w=1200',0),(95,22,'https://image.msscdn.net/thumbnails/images/goods_img/20250711/5243543/5243543_17525373596011_big.jpg?w=1200',0),(96,21,'https://image.msscdn.net/thumbnails/images/prd_img/20240112/3790870/detail_3790870_17098804561053_big.jpg?w=1200',0),(97,21,'https://image.msscdn.net/thumbnails/images/goods_img/20240516/4134294/4134294_17207397426331_big.jpg?w=1200',0),(98,26,'https://static.nike.com/a/images/t_PDP_1728_v1/f_auto,q_auto:eco/ad23697e-7842-426a-a6f9-937c2d9a4ede/AS+M+ACG+SF+PRMLFT+SKL+PK+JKT.png',0),(99,27,'https://static.nike.com/a/images/t_PDP_1728_v1/f_auto,q_auto:eco/f73f4ae9-3fb2-4d56-a5ca-9b271d5a30f5/AS+M+ACG+SF+PRMLFT+SKL+PK+JKT.png',0);
/*!40000 ALTER TABLE `product_images` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `product_options`
--

DROP TABLE IF EXISTS `product_options`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `product_options` (
  `option_id` bigint NOT NULL AUTO_INCREMENT COMMENT '상품 옵션 PK',
  `option_code` varchar(100) DEFAULT NULL COMMENT '옵션 식별 코드 (예: SP-TS-001-BK-L)',
  `product_id` bigint NOT NULL COMMENT '상품 FK',
  `color_id` int NOT NULL COMMENT '색상 FK',
  `size_id` int NOT NULL COMMENT '사이즈 FK',
  `stock` int NOT NULL DEFAULT '0' COMMENT '옵션별 재고 수량',
  PRIMARY KEY (`option_id`),
  UNIQUE KEY `product_id` (`product_id`,`color_id`,`size_id`),
  UNIQUE KEY `option_code` (`option_code`),
  KEY `color_id` (`color_id`),
  KEY `size_id` (`size_id`),
  KEY `idx_stock_check` (`option_id`,`stock`),
  CONSTRAINT `product_options_ibfk_1` FOREIGN KEY (`product_id`) REFERENCES `products` (`product_id`) ON DELETE CASCADE,
  CONSTRAINT `product_options_ibfk_2` FOREIGN KEY (`color_id`) REFERENCES `colors` (`color_id`),
  CONSTRAINT `product_options_ibfk_3` FOREIGN KEY (`size_id`) REFERENCES `sizes` (`size_id`)
) ENGINE=InnoDB AUTO_INCREMENT=145 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='상품의 색상/사이즈별 재고 정보';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `product_options`
--

LOCK TABLES `product_options` WRITE;
/*!40000 ALTER TABLE `product_options` DISABLE KEYS */;
INSERT INTO `product_options` VALUES (1,'JJ-TP-001-WH-M',1,1,2,488),(2,'JJ-TP-001-WH-L',1,1,3,520),(3,'JJ-TP-001-BK-M',1,2,2,520),(4,'JJ-TP-001-GY-L',1,6,3,550),(5,'NK-TP-001-BK-L',2,2,3,600),(6,'NK-TP-001-BK-XL',2,2,4,580),(7,'NK-TP-001-RD-L',2,7,3,530),(11,'AD-TP-001-BL-S',4,5,1,510),(12,'AD-TP-001-BL-M',4,5,2,540),(13,'AD-TP-001-BK-S',4,2,1,500),(15,'JJ-TP-002-BL-F',5,5,5,618),(16,'NK-TP-002-GY-L',6,6,3,580),(17,'NK-TP-002-GY-XL',6,6,4,560),(18,'NK-TP-002-NV-L',6,3,3,590),(19,'SP-TP-002-WH-M',7,1,2,101),(20,'SP-TP-002-WH-L',7,1,3,120),(21,'AD-TP-002-NV-M',8,3,2,530),(22,'AD-TP-002-NV-L',8,3,3,550),(23,'AD-TP-002-WH-M',8,1,2,520),(24,'JJ-TP-003-BE-M',9,4,2,150),(25,'JJ-TP-003-BE-L',9,4,3,490),(26,'JJ-TP-003-GY-M',9,6,2,500),(27,'NK-TP-003-BK-L',10,2,3,620),(28,'NK-TP-003-GY-L',10,6,3,650),(29,'NK-TP-003-GY-XL',10,6,4,610),(30,'JJ-BT-001-BK-S',11,2,1,500),(31,'JJ-BT-001-BK-M',11,2,2,520),(32,'JJ-BT-001-BE-S',11,4,1,515),(33,'AD-BT-001-BK-M',12,2,2,580),(34,'AD-BT-001-BK-L',12,2,3,590),(35,'NK-BT-001-GY-M',13,6,2,490),(36,'NK-BT-001-GY-L',13,6,3,155),(37,'NK-BT-001-BK-M',13,2,2,510),(40,'JJ-BT-002-BE-S',15,4,1,600),(41,'JJ-BT-002-BE-M',15,4,2,650),(42,'JJ-BT-002-NV-S',15,3,1,620),(43,'AD-BT-002-BK-M',16,2,2,550),(44,'AD-BT-002-BK-L',16,2,3,560),(45,'NK-BT-002-BK-S',17,2,1,580),(46,'NK-BT-002-GY-S',17,6,1,570),(47,'NK-BT-002-GY-M',17,6,2,590),(48,'SP-BT-002-GY-M',18,6,2,140),(50,'JJ-BT-003-BL-M',19,5,2,500),(51,'JJ-BT-003-BL-L',19,5,3,510),(52,'AD-BT-003-WH-S',20,1,1,490),(53,'AD-BT-003-WH-M',20,1,2,520),(54,'AD-BT-003-NV-S',20,3,1,500),(56,'JJ-OT-001-BK-L',21,2,3,505),(57,'JJ-OT-001-BE-M',21,4,2,101),(58,'NK-OT-001-RD-L',22,7,3,550),(59,'NK-OT-001-RD-XL',22,7,4,530),(60,'NK-OT-001-BK-L',22,2,3,580),(61,'AD-OT-001-BK-L',23,2,3,500),(62,'AD-OT-001-BK-XL',23,2,4,490),(64,'SP-OT-001-RD-M',24,7,2,497),(65,'JJ-OT-002-BK-L',25,2,3,509),(66,'JJ-OT-002-BK-XL',25,2,4,500),(67,'NK-OT-002-BK-M',26,2,2,488),(68,'NK-OT-002-GY-M',26,6,2,492),(69,'AD-OT-002-BL-L',27,5,3,540),(70,'AD-OT-002-RD-L',27,7,3,530),(73,'JJ-OT-003-BE-M',29,4,2,520),(74,'JJ-OT-003-BE-L',29,4,3,540),(75,'JJ-OT-003-NV-M',29,3,2,530),(76,'NK-OT-003-BK-XL',30,2,4,560),(77,'NK-OT-003-GY-XL',30,6,4,550),(78,'JJ-IN-001-BK-M',31,2,2,700),(79,'JJ-IN-001-GY-M',31,6,2,750),(80,'JJ-IN-001-BK-L',31,2,3,720),(81,'AD-IN-001-BK-L',32,2,3,600),(82,'AD-IN-001-WH-L',32,1,3,620),(83,'NK-IN-001-WH-L',33,1,3,650),(84,'NK-IN-001-WH-XL',33,1,4,630),(85,'NK-IN-001-BK-L',33,2,3,680),(86,'JJ-IN-002-WH-F',34,1,5,1098),(87,'JJ-IN-002-BK-F',34,2,5,1200),(88,'AD-IN-002-BK-S',35,2,1,550),(89,'AD-IN-002-BK-M',35,2,2,579),(90,'NK-IN-002-WH-F',36,1,5,1100),(91,'NK-IN-002-BK-F',36,2,5,1100),(92,'JJ-IN-003-GY-L',37,6,3,800),(93,'JJ-IN-003-GY-XL',37,6,4,850),(94,'AD-IN-003-BK-M',38,2,2,700),(95,'AD-IN-003-NV-M',38,3,2,680),(96,'NK-IN-003-BK-S',39,2,1,680),(97,'NK-IN-003-BK-M',39,2,2,720),(98,'JJ-IN-004-BL-L',40,5,3,580),(99,'JJ-IN-004-GY-L',40,6,3,600),(100,'NK-SH-001-WH-270',41,1,7,699),(101,'NK-SH-001-WH-280',41,1,8,650),(102,'NK-SH-001-BK-270',41,2,7,680),(103,'AD-SH-001-WH-260',42,1,6,680),(104,'AD-SH-001-WH-270',42,1,7,719),(105,'JJ-SH-001-WH-270',43,1,7,580),(106,'JJ-SH-001-BK-270',43,2,7,590),(107,'JJ-SH-001-BE-270',43,4,7,550),(108,'NK-SH-002-GY-270',44,6,7,550),(109,'NK-SH-002-GY-280',44,6,8,520),(110,'AD-SH-002-BK-260',45,2,6,530),(111,'AD-SH-002-BK-270',45,2,7,560),(112,'JJ-SH-002-BK-270',46,2,7,510),(113,'JJ-SH-002-BK-280',46,2,8,492),(114,'NK-SH-003-BK-270',47,2,7,600),(115,'NK-SH-003-WH-270',47,1,7,649),(116,'NK-SH-003-RD-270',47,7,7,580),(117,'AD-SH-003-NV-260',48,3,6,570),(118,'AD-SH-003-NV-270',48,3,7,590),(119,'JJ-SH-003-BK-280',49,2,8,700),(120,'JJ-SH-003-NV-280',49,3,8,680),(128,'NK-BT-003-NV-XL',55,3,4,0);
/*!40000 ALTER TABLE `product_options` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `products`
--

DROP TABLE IF EXISTS `products`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `products` (
  `product_id` bigint NOT NULL AUTO_INCREMENT COMMENT '상품 PK',
  `product_code` varchar(50) NOT NULL COMMENT '상품 고유 품번 (예: SP-TS-001)',
  `product_name` varchar(255) NOT NULL COMMENT '상품명',
  `brand_id` int NOT NULL COMMENT '브랜드 FK',
  `category_id` int NOT NULL COMMENT '카테고리 FK',
  `price` int NOT NULL COMMENT '가격',
  `description` text COMMENT '상품 상세설명',
  `view_count` int NOT NULL DEFAULT '0' COMMENT '조회수',
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '상품 등록일',
  PRIMARY KEY (`product_id`),
  UNIQUE KEY `product_code` (`product_code`),
  KEY `brand_id` (`brand_id`),
  KEY `category_id` (`category_id`),
  CONSTRAINT `products_ibfk_1` FOREIGN KEY (`brand_id`) REFERENCES `brands` (`brand_id`),
  CONSTRAINT `products_ibfk_2` FOREIGN KEY (`category_id`) REFERENCES `categories` (`category_id`)
) ENGINE=InnoDB AUTO_INCREMENT=70 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='상품(품목)의 기본 정보';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `products`
--

LOCK TABLES `products` WRITE;
/*!40000 ALTER TABLE `products` DISABLE KEYS */;
INSERT INTO `products` VALUES (1,'JJ-TP-001','데일리 베이직 티셔츠',1,1,19000,'어디에나 잘 어울리는 기본 티셔츠입니다.',164,'2025-09-28 17:20:07'),(2,'NK-TP-001','스포츠 드라이핏 상의',3,1,45000,'운동 시 쾌적함을 유지해주는 기능성 상의입니다.',256,'2025-09-13 17:20:07'),(3,'SP-TP-001','로고 프린팅 후드',2,1,159000,'슈프림 로고가 돋보이는 스트릿 패션 후드입니다.',464,'2025-10-03 17:20:07'),(4,'AD-TP-001','트레이닝 긴팔 져지',4,1,79000,'편안한 착용감의 트레이닝 져지입니다.',183,'2025-08-29 17:20:07'),(5,'JJ-TP-002','오버핏 스트라이프 셔츠',1,1,38000,'자연스러운 멋이 있는 오버핏 셔츠.',161,'2025-10-06 17:20:07'),(6,'NK-TP-002','NSW 클럽 맨투맨',3,1,69000,'부드러운 기모 안감의 맨투맨입니다.',310,'2025-09-23 17:20:07'),(7,'SP-TP-002','그래픽 반팔 티셔츠',2,1,89000,'독특한 그래픽이 특징인 반팔 티셔츠.',528,'2025-09-30 17:20:07'),(8,'AD-TP-002','퍼포먼스 폴로 셔츠',4,1,55000,'클래식한 디자인의 폴로 셔츠입니다.',95,'2025-08-09 17:20:07'),(9,'JJ-TP-003','케이블 니트 스웨터',1,1,52000,'따뜻하고 부드러운 케이블 니트입니다.',211,'2025-09-20 17:20:07'),(10,'NK-TP-003','에센셜 후드 집업',3,1,99000,'활용도 높은 기본 후드 집업입니다.',369,'2025-10-05 17:20:07'),(11,'JJ-BT-001','와이드 슬랙스',1,2,42000,'트렌디한 와이드 핏의 슬랙스입니다.',280,'2025-09-26 17:20:07'),(12,'AD-BT-001','티로 23 트레이닝 팬츠',4,2,65000,'활동성이 뛰어난 트레이닝 팬츠입니다.',380,'2025-09-08 17:20:07'),(13,'NK-BT-001','테크 플리스 조거',3,2,129000,'가볍고 따뜻한 테크 플리스 소재의 조거 팬츠.',584,'2025-10-01 17:20:07'),(14,'SP-BT-001','카고 데님 팬츠',2,2,210000,'넉넉한 수납 공간의 카고 데님 팬츠입니다.',418,'2025-09-16 17:20:07'),(15,'JJ-BT-002','밴딩 린넨 쇼츠',1,2,28000,'시원한 린넨 소재의 여름용 쇼츠입니다.',93,'2025-07-10 17:20:07'),(16,'AD-BT-002','알파스킨 타이츠',4,2,59000,'근육을 지지해주는 컴프레션 타이츠.',150,'2025-08-24 17:20:07'),(17,'NK-BT-002','챌린저 7인치 쇼츠',3,2,49000,'러닝에 최적화된 경량 쇼츠입니다.',290,'2025-09-24 17:20:07'),(18,'SP-BT-002','스웻 팬츠',2,2,180000,'편안함과 스타일을 모두 잡은 스웻 팬츠.',354,'2025-10-05 17:20:07'),(19,'JJ-BT-003','데미지 워싱진',1,2,68000,'빈티지한 매력의 데미지 워싱진입니다.',220,'2025-09-19 17:20:07'),(20,'AD-BT-003','골프 플리츠 스커트',4,2,89000,'활동적인 디자인의 골프 스커트입니다.',114,'2025-08-19 17:20:07'),(21,'JJ-OT-001','오버핏 블레이저 자켓',1,3,89000,'격식있는 자리나 캐주얼룩 모두 어울리는 블레이저.',323,'2025-09-18 17:20:07'),(22,'NK-OT-001','윈드러너 후드 재킷',3,3,119000,'가볍고 바람을 잘 막아주는 윈드러너.',494,'2025-09-29 17:20:07'),(23,'AD-OT-001','유틸리티 다운 파카',4,3,299000,'혹한에도 끄떡없는 따뜻한 다운 파카입니다.',553,'2025-06-30 17:20:07'),(24,'SP-OT-001','GORE-TEX 쉘 자켓',2,3,450000,'방수, 방풍 기능이 뛰어난 고어텍스 자켓.',701,'2025-09-25 17:20:07'),(25,'JJ-OT-002','레더 블루종',1,3,128000,'시크한 매력의 인조 가죽 블루종입니다.',269,'2025-09-05 17:20:07'),(26,'NK-OT-002','ACG 마운틴 플라이 자켓',3,3,249000,'아웃도어 활동에 최적화된 기능성 자켓.',392,'2025-09-10 17:20:07'),(27,'AD-OT-002','바람막이 트랙 탑',4,3,99000,'레트로 감성의 3선 트랙 탑입니다.',214,'2025-08-27 17:20:07'),(29,'JJ-OT-003','더블 트렌치 코트',1,3,159000,'가을 시즌 필수 아이템, 더블 트렌치 코트.',335,'2025-08-14 17:20:07'),(30,'NK-OT-003','스포츠웨어 리펠 파카',3,3,189000,'생활 방수 기능이 있는 롱 파카입니다.',285,'2025-07-20 17:20:07'),(31,'JJ-IN-001','에어리즘 심리스 드로즈',1,4,12000,'안 입은 듯 편안한 심리스 드로즈.',453,'2025-10-04 17:20:07'),(32,'AD-IN-001','알파스킨 복서 브리프',4,4,25000,'운동 시에도 편안한 기능성 복서 브리프.',213,'2025-08-31 17:20:07'),(33,'NK-IN-001','프로 드라이핏 탱크탑',3,4,35000,'흡습성이 뛰어난 드라이핏 탱크탑입니다.',314,'2025-09-15 17:20:07'),(34,'JJ-IN-002','수피마 코튼 크루삭스',1,4,5000,'부드러운 감촉의 고급 면 양말.',623,'2025-10-06 17:20:07'),(35,'AD-IN-002','스포츠 브라',4,4,49000,'안정적인 지지력의 스포츠 브라입니다.',191,'2025-08-19 17:20:07'),(36,'NK-IN-002','에브리데이 쿠션 삭스',3,4,15000,'푹신한 쿠션감의 스포츠 양말 3팩.',427,'2025-09-27 17:20:07'),(37,'JJ-IN-003','웜테크 발열 내의',1,4,22000,'추운 날씨를 위한 기능성 발열 내의.',353,'2025-06-10 17:20:07'),(38,'AD-IN-003','코튼 드로즈 3팩',4,4,45000,'편안한 면 소재의 드로즈 3종 세트.',285,'2025-08-09 17:20:07'),(39,'NK-IN-003','Dri-FIT 레깅스',3,4,69000,'다양한 운동에 적합한 드라이핏 레깅스.',396,'2025-09-22 17:20:07'),(40,'JJ-IN-004','실크 트렁크',1,4,18000,'부드럽고 고급스러운 실크 트렁크입니다.',153,'2025-07-25 17:20:07'),(41,'NK-SH-001','에어포스 1 07',3,5,139000,'시대를 초월하는 클래식, 나이키 에어포스 1.',957,'2025-10-02 17:20:07'),(42,'AD-SH-001','슈퍼스타',4,5,119000,'아디다스의 아이코닉한 모델, 슈퍼스타.',859,'2025-09-21 17:20:07'),(43,'JJ-SH-001','독일군 스니커즈',1,5,79000,'미니멀한 디자인의 독일군 스니커즈입니다.',455,'2025-09-17 17:20:07'),(44,'NK-SH-002','에어맥스 97',3,5,219000,'미래적인 디자인의 에어맥스 97.',792,'2025-09-07 17:20:07'),(45,'AD-SH-002','울트라부스트 1.0',4,5,229000,'최고의 쿠셔닝을 자랑하는 러닝화.',683,'2025-08-21 17:20:07'),(46,'JJ-SH-002','첼시 부츠',1,5,99000,'댄디한 스타일을 완성하는 첼시 부츠.',325,'2025-08-04 17:20:07'),(47,'NK-SH-003','덩크 로우 레트로',3,5,129000,'클래식한 농구화에서 스트릿 아이콘으로.',1123,'2025-10-06 17:20:07'),(48,'AD-SH-003','가젤',4,5,109000,'빈티지한 매력의 스웨이드 스니커즈.',556,'2025-07-10 17:20:07'),(49,'JJ-SH-003','플립플랍',1,5,25000,'여름철 편안하게 신는 플립플랍입니다.',188,'2025-05-11 17:20:07'),(50,'NK-SH-004','인피니티 런 4',3,5,189000,'부상 방지에 초점을 맞춘 안정적인 러닝화.',496,'2025-09-12 17:20:07'),(55,'NK-BT-003','나이키 카고 바지',3,2,74900,'진희의 원픽',54,'2025-10-14 14:38:23'),(56,'NK-OT-007','나이키 항공점퍼',3,3,249000,'태은이의 원픽',52,'2025-10-14 14:49:25');
/*!40000 ALTER TABLE `products` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `purchase_order_items`
--

DROP TABLE IF EXISTS `purchase_order_items`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `purchase_order_items` (
  `po_item_id` bigint NOT NULL AUTO_INCREMENT,
  `po_id` bigint NOT NULL,
  `option_id` bigint NOT NULL,
  `quantity` int NOT NULL,
  `unit_price` int NOT NULL,
  PRIMARY KEY (`po_item_id`),
  KEY `po_id` (`po_id`),
  KEY `option_id` (`option_id`),
  CONSTRAINT `purchase_order_items_ibfk_1` FOREIGN KEY (`po_id`) REFERENCES `purchase_orders` (`po_id`),
  CONSTRAINT `purchase_order_items_ibfk_2` FOREIGN KEY (`option_id`) REFERENCES `product_options` (`option_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='발주 상세';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `purchase_order_items`
--

LOCK TABLES `purchase_order_items` WRITE;
/*!40000 ALTER TABLE `purchase_order_items` DISABLE KEYS */;
/*!40000 ALTER TABLE `purchase_order_items` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `purchase_orders`
--

DROP TABLE IF EXISTS `purchase_orders`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `purchase_orders` (
  `po_id` bigint NOT NULL AUTO_INCREMENT,
  `po_date` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `supplier_name` varchar(100) DEFAULT NULL,
  `expected_date` date DEFAULT NULL,
  `status` varchar(20) DEFAULT '대기중',
  `total_amount` int DEFAULT NULL,
  `notes` text,
  `created_by` varchar(50) DEFAULT NULL,
  PRIMARY KEY (`po_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='발주서';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `purchase_orders`
--

LOCK TABLES `purchase_orders` WRITE;
/*!40000 ALTER TABLE `purchase_orders` DISABLE KEYS */;
/*!40000 ALTER TABLE `purchase_orders` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `qna`
--

DROP TABLE IF EXISTS `qna`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `qna` (
  `qna_id` bigint NOT NULL AUTO_INCREMENT COMMENT '문의 PK',
  `product_id` bigint NOT NULL COMMENT '상품 FK',
  `member_id` bigint NOT NULL COMMENT '회원 FK',
  `question_content` text NOT NULL COMMENT '질문 내용',
  `answer_content` text COMMENT '답변 내용',
  `question_date` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '질문 작성일',
  `answer_date` datetime DEFAULT NULL COMMENT '답변 작성일',
  `is_private` tinyint(1) NOT NULL DEFAULT '0' COMMENT '비밀글 여부',
  PRIMARY KEY (`qna_id`),
  KEY `product_id` (`product_id`),
  KEY `member_id` (`member_id`),
  KEY `idx_answer_status` (`qna_id`,`answer_content`(1)),
  CONSTRAINT `qna_ibfk_1` FOREIGN KEY (`product_id`) REFERENCES `products` (`product_id`) ON DELETE CASCADE,
  CONSTRAINT `qna_ibfk_2` FOREIGN KEY (`member_id`) REFERENCES `members` (`member_id`)
) ENGINE=InnoDB AUTO_INCREMENT=10 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='상품 문의(Q&A)';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `qna`
--

LOCK TABLES `qna` WRITE;
/*!40000 ALTER TABLE `qna` DISABLE KEYS */;
INSERT INTO `qna` VALUES (3,56,4,'이진희 바보\n','정소빈 카리나','2025-10-14 15:03:44','2025-10-15 18:49:07',0),(4,55,4,'이진희 바보\n',NULL,'2025-10-14 15:04:04',NULL,0),(5,55,4,'이진희 바보\n',NULL,'2025-10-14 15:04:08',NULL,0),(6,56,4,'김태은 바보~','정소빈 카리나','2025-10-15 17:31:41','2025-10-15 18:48:58',0),(7,56,2,'정소빈 카리나',NULL,'2025-10-15 18:49:15',NULL,0),(8,56,2,'정소빈 카리나',NULL,'2025-10-15 18:49:26',NULL,1),(9,5,7,'보통 95m입는데 이거 괜찮나요? 키 173정도인데...',NULL,'2025-12-23 11:56:31',NULL,0);
/*!40000 ALTER TABLE `qna` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `reviews`
--

DROP TABLE IF EXISTS `reviews`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `reviews` (
  `review_id` bigint NOT NULL AUTO_INCREMENT COMMENT '리뷰 PK',
  `member_id` bigint NOT NULL COMMENT '회원 FK',
  `product_id` bigint NOT NULL COMMENT '상품 FK',
  `rating` int NOT NULL COMMENT '별점 (1~5)',
  `content` text COMMENT '리뷰 내용',
  `created_at` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP COMMENT '작성일',
  PRIMARY KEY (`review_id`),
  UNIQUE KEY `unique_member_product` (`member_id`,`product_id`),
  KEY `product_id` (`product_id`),
  CONSTRAINT `reviews_ibfk_1` FOREIGN KEY (`member_id`) REFERENCES `members` (`member_id`),
  CONSTRAINT `reviews_ibfk_2` FOREIGN KEY (`product_id`) REFERENCES `products` (`product_id`) ON DELETE CASCADE,
  CONSTRAINT `chk_rating` CHECK ((`rating` between 1 and 5))
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='상품 리뷰';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `reviews`
--

LOCK TABLES `reviews` WRITE;
/*!40000 ALTER TABLE `reviews` DISABLE KEYS */;
INSERT INTO `reviews` VALUES (1,3,55,3,'정소빈 카리나\n','2025-10-15 17:31:37'),(2,5,35,4,'너무 작아여...','2025-10-17 11:41:52'),(3,7,5,5,'너무 딱 맞아요!!','2025-12-23 11:56:06');
/*!40000 ALTER TABLE `reviews` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sizes`
--

DROP TABLE IF EXISTS `sizes`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `sizes` (
  `size_id` int NOT NULL AUTO_INCREMENT COMMENT '사이즈 PK',
  `size_name` varchar(20) NOT NULL COMMENT '사이즈 이름 (S, M, L, 270 등)',
  `size_code` varchar(10) DEFAULT NULL COMMENT '사이즈 약어 코드 (예: M, L, 270)',
  PRIMARY KEY (`size_id`),
  UNIQUE KEY `size_name` (`size_name`),
  UNIQUE KEY `size_code` (`size_code`)
) ENGINE=InnoDB AUTO_INCREMENT=9 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='상품 사이즈 정보';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sizes`
--

LOCK TABLES `sizes` WRITE;
/*!40000 ALTER TABLE `sizes` DISABLE KEYS */;
INSERT INTO `sizes` VALUES (1,'S','S'),(2,'M','M'),(3,'L','L'),(4,'XL','XL'),(5,'FREE','F'),(6,'260','260'),(7,'270','270'),(8,'280','280');
/*!40000 ALTER TABLE `sizes` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `stock_history`
--

DROP TABLE IF EXISTS `stock_history`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `stock_history` (
  `history_id` bigint NOT NULL AUTO_INCREMENT,
  `option_id` bigint NOT NULL,
  `change_type` varchar(20) NOT NULL,
  `quantity_change` int NOT NULL,
  `before_stock` int NOT NULL,
  `after_stock` int NOT NULL,
  `reason` text,
  `created_at` datetime DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`history_id`),
  KEY `option_id` (`option_id`),
  CONSTRAINT `stock_history_ibfk_1` FOREIGN KEY (`option_id`) REFERENCES `product_options` (`option_id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='재고 변동 이력';
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `stock_history`
--

LOCK TABLES `stock_history` WRITE;
/*!40000 ALTER TABLE `stock_history` DISABLE KEYS */;
/*!40000 ALTER TABLE `stock_history` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping events for database 'themost_db'
--

--
-- Dumping routines for database 'themost_db'
--
/*!50003 DROP PROCEDURE IF EXISTS `AddOrUpdateProductOption` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`mfcuser`@`%` PROCEDURE `AddOrUpdateProductOption`(
    IN p_brand_name VARCHAR(50), IN p_product_name VARCHAR(255), IN p_category_name VARCHAR(50),
    IN p_color_name VARCHAR(30), IN p_size_name VARCHAR(20), IN p_price INT,
    IN p_stock INT, IN p_description TEXT,
    IN p_image_url VARCHAR(255), IN p_is_main_image BOOLEAN,
    OUT p_new_product_id BIGINT
)
BEGIN
    DECLARE v_brand_id INT; DECLARE v_category_id INT; DECLARE v_color_id INT; DECLARE v_size_id INT;
    DECLARE v_product_id BIGINT; DECLARE v_option_id BIGINT; DECLARE v_product_code VARCHAR(50);
    SELECT brand_id INTO v_brand_id FROM brands WHERE brand_name = p_brand_name;
    SELECT category_id INTO v_category_id FROM categories WHERE category_name = p_category_name;
    SELECT color_id INTO v_color_id FROM colors WHERE color_name = p_color_name;
    SELECT size_id INTO v_size_id FROM sizes WHERE size_name = p_size_name;
    SELECT product_id INTO v_product_id FROM products WHERE product_name = p_product_name AND brand_id = v_brand_id;
    IF v_product_id IS NULL THEN
        BEGIN
            DECLARE v_brand_code VARCHAR(10); DECLARE v_category_code VARCHAR(10);
            DECLARE v_last_seq INT; DECLARE v_new_seq_str VARCHAR(3);
            SELECT brand_code INTO v_brand_code FROM brands WHERE brand_id = v_brand_id;
            SELECT category_code INTO v_category_code FROM categories WHERE category_id = v_category_id;
            SELECT MAX(CAST(SUBSTRING(product_code, LENGTH(v_brand_code) + LENGTH(v_category_code) + 3) AS UNSIGNED)) INTO v_last_seq FROM products WHERE product_code LIKE CONCAT(v_brand_code, '-', v_category_code, '-%');
            IF v_last_seq IS NULL THEN SET v_last_seq = 1; ELSE SET v_last_seq = v_last_seq + 1; END IF;
            SET v_new_seq_str = LPAD(v_last_seq, 3, '0');
            SET v_product_code = CONCAT(v_brand_code, '-', v_category_code, '-', v_new_seq_str);
        END;
        INSERT INTO products (product_code, product_name, brand_id, category_id, price, description) VALUES (v_product_code, p_product_name, v_brand_id, v_category_id, p_price, p_description);
        SET v_product_id = LAST_INSERT_ID();
    ELSE
        SELECT product_code INTO v_product_code FROM products WHERE product_id = v_product_id;
        UPDATE products SET price = p_price, description = p_description WHERE product_id = v_product_id;
    END IF;
    SELECT option_id INTO v_option_id FROM product_options WHERE product_id = v_product_id AND color_id = v_color_id AND size_id = v_size_id;
    IF v_option_id IS NOT NULL THEN
        UPDATE product_options SET stock = stock + p_stock WHERE option_id = v_option_id;
    ELSE
        BEGIN
            DECLARE v_color_code VARCHAR(10); DECLARE v_size_code VARCHAR(10); DECLARE v_option_code VARCHAR(100);
            SELECT color_code INTO v_color_code FROM colors WHERE color_id = v_color_id;
            SELECT size_code INTO v_size_code FROM sizes WHERE size_id = v_size_id;
            SET v_option_code = CONCAT(v_product_code, '-', v_color_code, '-', v_size_code);
            INSERT INTO product_options (option_code, product_id, color_id, size_id, stock) VALUES (v_option_code, v_product_id, v_color_id, v_size_id, p_stock);
        END;
    END IF;
    IF p_image_url IS NOT NULL AND p_image_url != '' THEN
        INSERT INTO product_images (product_id, image_url, is_main_image) VALUES (v_product_id, p_image_url, p_is_main_image);
    END IF;
    SET p_new_product_id = v_product_id;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `add_product` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`mfcuser`@`%` PROCEDURE `add_product`(
    IN p_brand_name VARCHAR(50),
    IN p_category_name VARCHAR(50),
    IN p_product_name VARCHAR(255),
    IN p_price INT,
    IN p_description TEXT,
    IN p_images JSON,   -- JSON_ARRAY('main.jpg','sub1.jpg')
    IN p_options JSON   -- JSON_ARRAY(JSON_OBJECT('color','블랙','size','M','stock',10), ...)
)
BEGIN
    DECLARE v_brand_id INT DEFAULT NULL;
    DECLARE v_brand_code VARCHAR(10) DEFAULT NULL;
    DECLARE v_category_id INT DEFAULT NULL;
    DECLARE v_category_code VARCHAR(10) DEFAULT NULL;
    DECLARE v_product_id BIGINT DEFAULT NULL;
    DECLARE v_seq INT DEFAULT 0;
    DECLARE v_product_code VARCHAR(50);

    DECLARE i INT DEFAULT 0;
    DECLARE img_count INT DEFAULT 0;
    DECLARE opt_count INT DEFAULT 0;

    DECLARE v_color_name VARCHAR(100);
    DECLARE v_color_id INT;
    DECLARE v_color_code VARCHAR(10);

    DECLARE v_size_input_code VARCHAR(50);
    DECLARE v_size_id INT;
    DECLARE v_size_code_db VARCHAR(50);

    DECLARE v_stock_str VARCHAR(50);
    DECLARE v_stock INT DEFAULT 0;
    DECLARE v_option_code VARCHAR(200);

    DECLARE err_msg VARCHAR(255);

    -- 트랜잭션 시작
    START TRANSACTION;

    -- 브랜드 확인
    SET v_brand_id = (SELECT brand_id FROM brands WHERE brand_name = p_brand_name LIMIT 1);
    SET v_brand_code = (SELECT brand_code FROM brands WHERE brand_name = p_brand_name LIMIT 1);
    IF v_brand_id IS NULL THEN
        SET err_msg = CONCAT('브랜드 없음: ', p_brand_name);
        ROLLBACK;
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = err_msg;
    END IF;

    -- 카테고리 확인
    SET v_category_id = (SELECT category_id FROM categories WHERE category_name = p_category_name LIMIT 1);
    SET v_category_code = (SELECT category_code FROM categories WHERE category_name = p_category_name LIMIT 1);
    IF v_category_id IS NULL THEN
        SET err_msg = CONCAT('카테고리 없음: ', p_category_name);
        ROLLBACK;
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = err_msg;
    END IF;

    -- 브랜드+카테고리별 다음 고유번호
    SELECT IFNULL(MAX(CAST(SUBSTRING_INDEX(product_code,'-',-1) AS UNSIGNED)),0)+1
      INTO v_seq
    FROM products
    WHERE brand_id = v_brand_id AND category_id = v_category_id;

    SET v_product_code = CONCAT(v_brand_code,'-',v_category_code,'-',LPAD(v_seq,3,'0'));

    -- 상품 추가
    INSERT INTO products(product_code, product_name, brand_id, category_id, price, description)
    VALUES(v_product_code, p_product_name, v_brand_id, v_category_id, p_price, p_description);
    SET v_product_id = LAST_INSERT_ID();

    -- 이미지 등록
    SET img_count = JSON_LENGTH(p_images);
    SET i = 0;
    WHILE i < img_count DO
        INSERT INTO product_images(product_id, image_url, is_main_image)
        VALUES(v_product_id, JSON_UNQUOTE(JSON_EXTRACT(p_images, CONCAT('$[',i,']'))), IF(i=0, TRUE, FALSE));
        SET i = i + 1;
    END WHILE;

    -- 옵션 등록
    SET opt_count = JSON_LENGTH(p_options);
    SET i = 0;
    WHILE i < opt_count DO
        SET v_color_name = JSON_UNQUOTE(JSON_EXTRACT(p_options, CONCAT('$[',i,'].color')));
        SET v_size_input_code = JSON_UNQUOTE(JSON_EXTRACT(p_options, CONCAT('$[',i,'].size')));
        SET v_stock_str = JSON_UNQUOTE(JSON_EXTRACT(p_options, CONCAT('$[',i,'].stock')));

        -- stock 기본값 0
        IF v_stock_str IS NULL OR v_stock_str = '' THEN
            SET v_stock = 0;
        ELSE
            SET v_stock = CAST(v_stock_str AS UNSIGNED);
        END IF;

        -- 색상 확인
        SET v_color_id = (SELECT color_id FROM colors WHERE color_name = v_color_name LIMIT 1);
        SET v_color_code = (SELECT color_code FROM colors WHERE color_name = v_color_name LIMIT 1);
        IF v_color_id IS NULL THEN
            SET err_msg = CONCAT('존재하지 않는 색상: ', v_color_name);
            ROLLBACK;
            SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = err_msg;
        END IF;

        -- 사이즈 확인
        SET v_size_id = (SELECT size_id FROM sizes WHERE size_code = v_size_input_code LIMIT 1);
        SET v_size_code_db = (SELECT size_code FROM sizes WHERE size_code = v_size_input_code LIMIT 1);
        IF v_size_id IS NULL THEN
            SET err_msg = CONCAT('존재하지 않는 사이즈 코드: ', v_size_input_code);
            ROLLBACK;
            SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = err_msg;
        END IF;

        -- 옵션코드 생성
        SET v_option_code = CONCAT(v_product_code,'-',v_color_code,'-',v_size_code_db);

        -- 옵션 저장
        INSERT INTO product_options(option_code, product_id, color_id, size_id, stock)
        VALUES(v_option_code, v_product_id, v_color_id, v_size_id, v_stock);

        SET i = i + 1;
    END WHILE;

    -- 모든 작업 성공 시 커밋
    COMMIT;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `delete_product` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`mfcuser`@`%` PROCEDURE `delete_product`(
    IN p_brand_name VARCHAR(50),
    IN p_category_name VARCHAR(50),
    IN p_product_seq VARCHAR(10)  -- 예: '001' (상품코드 마지막 3자리)
)
BEGIN
    DECLARE v_brand_id INT;
    DECLARE v_category_id INT;
    DECLARE v_product_code VARCHAR(50);
    DECLARE v_product_id BIGINT;
    DECLARE err_msg VARCHAR(255);

    -- 트랜잭션 시작
    START TRANSACTION;

    -- 브랜드 확인
    SET v_brand_id = (SELECT brand_id FROM brands WHERE brand_name = p_brand_name LIMIT 1);
    IF v_brand_id IS NULL THEN
        SET err_msg = CONCAT('브랜드 없음: ', p_brand_name);
        ROLLBACK;
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = err_msg;
    END IF;

    -- 카테고리 확인
    SET v_category_id = (SELECT category_id FROM categories WHERE category_name = p_category_name LIMIT 1);
    IF v_category_id IS NULL THEN
        SET err_msg = CONCAT('카테고리 없음: ', p_category_name);
        ROLLBACK;
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = err_msg;
    END IF;

    -- 상품코드 생성
    SET v_product_code = (
        SELECT CONCAT(b.brand_code,'-',c.category_code,'-',LPAD(CAST(p_product_seq AS UNSIGNED),3,'0'))
        FROM brands b, categories c
        WHERE b.brand_id = v_brand_id AND c.category_id = v_category_id
        LIMIT 1
    );

    -- 상품 존재 확인
    SET v_product_id = (SELECT product_id FROM products WHERE product_code = v_product_code LIMIT 1);
    IF v_product_id IS NULL THEN
        SET err_msg = CONCAT('삭제 대상 상품 없음: ', v_product_code);
        ROLLBACK;
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = err_msg;
    END IF;

    -- 상품 삭제 (옵션, 이미지 CASCADE로 자동 삭제)
    DELETE FROM products WHERE product_id = v_product_id;

    -- 트랜잭션 커밋
    COMMIT;

END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP PROCEDURE IF EXISTS `UpdateStock` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`mfcuser`@`%` PROCEDURE `UpdateStock`(
    IN p_brand_name VARCHAR(50), IN p_product_name VARCHAR(255),
    IN p_color_name VARCHAR(30), IN p_size_name VARCHAR(20), IN p_stock_to_add INT
)
BEGIN
    DECLARE v_option_id BIGINT;
    SELECT po.option_id INTO v_option_id FROM product_options po
    JOIN products p ON po.product_id = p.product_id JOIN brands b ON p.brand_id = b.brand_id
    JOIN colors c ON po.color_id = c.color_id JOIN sizes s ON po.size_id = s.size_id
    WHERE b.brand_name = p_brand_name AND p.product_name = p_product_name
    AND c.color_name = p_color_name AND s.size_name = p_size_name;
    IF v_option_id IS NOT NULL THEN
        UPDATE product_options SET stock = stock + p_stock_to_add WHERE option_id = v_option_id;
    END IF;
END ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-12-23 12:02:04
