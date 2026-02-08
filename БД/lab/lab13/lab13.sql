
alter session set container = BVV_PDB;
//BVV

--1


create table T_RANGE 
(
    id number,
    TIME_ID date
)
partition by range (id)
(
    partition part0 values less than (15),
    partition part1 values less than (25),
    partition part2 values less than (35),
    partition PartMAX values less than (maxvalue)
);

begin
    for i in 1..55
    loop
        insert into T_RANGE(id, time_id) values (i, sysdate);
    end loop;
end;
commit;

select * from T_RANGE partition(part0);
select * from T_RANGE partition(part1);
select * from T_RANGE partition(part2);
select * from T_RANGE partition(PartMAX);

select TABLE_NAME, PARTITION_NAME, HIGH_VALUE, TABLESPACE_NAME from USER_TAB_PARTITIONS where table_name = 'T_RANGE';
select * from T_RANGE;

drop table T_RANGE;
--2


create table T_INTERVAL 
(
    id number,
    time_id date
)
    partition by range (time_id)
    interval (numtoyminterval(1, 'month'))
(
    partition part0 values less than (to_date('1-10-2025', 'dd-mm-yyyy')),
    partition part1 values less than (to_date('1-11-2025', 'dd-mm-yyyy')),
    partition part2 values less than (to_date('1-12-2025', 'dd-mm-yyyy'))
);

insert into T_INTERVAL (id, time_id) values (1, '01-09-2025');
insert into T_INTERVAL (id, time_id) values (2, '10-09-2025');
insert into T_INTERVAL (id, time_id) values (3, '25-09-2025');

insert into T_INTERVAL (id, time_id) values (4, '02-10-2025');
insert into T_INTERVAL (id, time_id) values (5, '15-10-2025');

insert into T_INTERVAL (id, time_id) values (6, '03-11-2025');
insert into T_INTERVAL (id, time_id) values (7, '12-11-2025');


insert into T_INTERVAL (id, time_id) values (8, '27-12-2025');
insert into T_INTERVAL (id, time_id) values (9, '10-01-2026');
insert into T_INTERVAL (id, time_id) values (10, '21-06-2026');

commit;

select * from T_INTERVAL  partition (part0);
select * from T_INTERVAL  partition (part1);
select * from T_INTERVAL  partition (part2);

select * from T_INTERVAL  partition (SYS_P1000);
select * from T_INTERVAL  partition (SYS_P1001);
select * from T_INTERVAL  partition (SYS_P999);

select TABLE_NAME, PARTITION_NAME, HIGH_VALUE, TABLESPACE_NAME from USER_TAB_PARTITIONS where table_name = 'T_INTERVAL';

drop table T_INTERVAL;


--3

create table T_HASH
(
    str varchar2(50),
    id  number
)
partition by hash (str)
(
    partition part0,
    partition part1,
    partition part2,
    partition part3
);

insert into T_HASH (str, id) values ('vilka', 1);
insert into T_HASH (str, id) values ('aaaa', 2);
insert into T_HASH (str, id) values ('bbbbb', 3);
insert into T_HASH (str, id) values ('ccccccc', 4);
insert into T_HASH (str, id) values ('eeeeeeee', 5);
insert into T_HASH (str, id) values ('ffffffff', 6);
insert into T_HASH (str, id) values ('hihihih', 7);
INSERT INTO T_HASH (str, id) VALUES ('aaaaaa', 8);
INSERT INTO T_HASH (str, id) VALUES ('zzzzzz', 9);

commit;

select * from T_HASH partition (part0);
select * from T_HASH partition (part1);
select * from T_HASH partition (part2);
select * from T_HASH partition (part3);

drop table T_HASH;

--4

create table T_LIST
(
    obj char(3)
)
partition by list(obj)
(
    partition part0 values ('A', 'D'),
    partition part1 values ('B', 'E'),
    partition part2 values ('C', 'F')
);

insert into T_LIST(obj) values('A');
insert into T_LIST(obj) values('B');
insert into T_LIST(obj) values('C');
insert into T_LIST(obj) values('D');
insert into T_LIST(obj) values('E');
insert into T_LIST(obj) values('F');
commit;

select * from T_LIST partition (part0);
select * from T_LIST partition (part1);
select * from T_LIST partition (part2);

drop table T_LIST;


--6

alter table T_RANGE  enable row movement;
update T_RANGE set id=60 where id=2;
select * from T_RANGE  partition(PartMAX);
select * from T_RANGE  partition(part0);   --<15

alter table T_INTERVAL  enable row movement;
update T_INTERVAL  set time_id=to_date('15-09-2025') where id=6;
select * from T_INTERVAL  partition(part0);
select * from T_INTERVAL  partition(part2);

alter table T_HASH enable row movement;
update T_HASH set str='ggggggggg' where id=2;
select * from T_HASH partition (part3);
select * from T_HASH partition(part1);


alter table T_LIST enable row movement;
update T_LIST set obj='B' where obj='A';
select * from T_LIST partition (part0);
select * from T_LIST partition (part1);

--7

alter table T_RANGE  merge partitions part1, part2 into partition part5;
select * from T_RANGE  partition(part5);
select * from T_RANGE  partition(part1);--15 24
select * from T_RANGE  partition(part2);--25 34

--8

alter table T_RANGE split partition part5 at (25) into (partition part1, partition part2);
select * from T_RANGE partition(part5);
select * from T_RANGE partition(part1);
select * from T_RANGE partition(part2);

--9

create table T_RANGE1 (
    id number,
    TIME_ID date
);
INSERT INTO T_RANGE1 (id, time_id) VALUES (0,  SYSDATE);
INSERT INTO T_RANGE1 (id, time_id) VALUES (-1, SYSDATE);
COMMIT;


SELECT * FROM T_RANGE1;

alter table T_RANGE exchange partition part0 with table T_RANGE1 without validation;

select * from T_RANGE partition (part0);
select * from T_RANGE1;

drop table T_RANGE1;

