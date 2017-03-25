PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE "dat"(
id INT,
first_name TEXT,
last_name TEXT, 
email TEXT, birthday int, department_id int, height REAL, weight INT);
INSERT INTO "dat" VALUES(1,'Vladimir','Tribushevsky','tribushevsky97@gmail.com',NULL,NULL,170.0,65);
INSERT INTO "dat" VALUES(2,'Dmitriy','Emelianov','emelya@gmail.com',NULL,NULL,170.0,65);
INSERT INTO "dat" VALUES(3,'Max','Alekhnovich','mAlekhnovich@gmail.com',NULL,NULL,170.0,65);
INSERT INTO "dat" VALUES(5,'Daniel','Muraveiko','dan.mur@gmail.com',NULL,NULL,170.0,65);
INSERT INTO "dat" VALUES(6,'Anastasya','Klimovich','anastasya.klimovich@gmail.com',NULL,NULL,200.0,80);
INSERT INTO "dat" VALUES(7,'Roman','Gricak','roman.gricak.klimovich@gmail.com',NULL,NULL,170.0,65);
INSERT INTO "dat" VALUES(8,'Alex','Pikaviy','pika@gmail.com',15031197,1,190.0,70);
CREATE TABLE department(
id INT,
name_department TEXT);
INSERT INTO "department" VALUES(1,'marketing');
COMMIT;
