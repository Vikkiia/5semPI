create table BVV_t( x number(3) primary key, s varchar2(50));
//11
INSERT INTO BVV_t(x,s)
VALUES (120,'NEW');

INSERT INTO BVV_t(x,s)
VALUES (80,'APPLE');

INSERT INTO BVV_t(x,s)
VALUES (101,'CAKE');

COMMIT

//12

SELECT * FROM BVV_t

UPDATE BVV_t SET x=25 WHERE s='NEW';

UPDATE BVV_t SET  x=11 WHERE s='CAKE';
COMMIT

//13
SELECT * FROM BVV_t WHERE x > 50;

SELECT COUNT(*) AS count_rows FROM BVV_t;
SELECT MIN(x) AS min_x FROM BVV_t;
SELECT AVG(x) AS avg_x FROM BVV_t;

//14
DELETE FROM BVV_t WHERE  s='APPLE';

COMMIT

//15

CREATE TABLE BVV_t1(
x1 number(3), 
s1 varchar(50), 
foreign key(x1) references BVV_t(x)
);

INSERT INTO BVV_t1(x1,s1) values (25, 'PIE');

select * from BVV_t;
commit;

//16
SELECT * FROM BVV_t LEFT JOIN BVV_t1 ON BVV_t.x=x1;
SELECT * FROM BVV_T RIGHT JOIN BVV_t1 ON BVV_t.x=x1;
SELECT t.x, t.s, t1.s1 FROM BVV_t t INNER JOIN BVV_t1 t1 ON t.x = t1.x1;




//18
DROP TABLE BVV_t1;
DROP TABLE BVV_t;






