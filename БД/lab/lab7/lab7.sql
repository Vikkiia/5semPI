//1
//BVV_PDBConnect
alter session set container = BVV_PDB;
//ALTER SESSION SET CONTAINER = CDB$ROOT;
--DROP USER BVV CASCADE;

--CREATE USER BVV IDENTIFIED BY 123;


select * from DBA_USERS;

grant create any view to BVV;
grant create any table to BVV;
grant create sequence to BVV;
grant unlimited tablespace to BVV;
grant restricted session to BVV;
grant create cluster to BVV;
grant create synonym to BVV;
grant create public synonym to BVV;
grant create materialized view to BVV;
GRANT CREATE SESSION TO BVV;


//2
//BVV

alter session set container = BVV_PDB;

create sequence S1
    start with 1000
    increment by 10
    nominvalue
    nomaxvalue
    nocycle
    nocache
    noorder;
    
select S1.nextval from DUAL;

select S1.currval from DUAL;

drop sequence S1;


//3-4

create sequence S2
    start with 10
    increment by 10
    maxvalue 100
    nocycle;
    
select S2.nextval from DUAL;



drop sequence S2;


//5

create sequence S3
    start with 10
    increment by -10
    minvalue -100
    maxvalue 10
    nocycle
    order;
    
select S3.nextval from DUAL;

drop sequence S3;


//6


create sequence S4
    start with 10    
    increment by 1
    minvalue 10
    maxvalue 20
    cycle
    cache 5
    noorder;
    
select S4.nextval from DUAL;

drop sequence S4;


//7

select * from user_sequences;

//8


create table T1 (
    N1 number(20),
    N2 number(20),
    N3 number(20),
    N4 number(20)
) cache storage ( buffer_pool keep );
    begin
      for i in 1..7 loop
        insert into T1 values (S1.nextval, S2.nextval, S3.nextval, S4.nextval);
      end loop;
    end;

select * from T1;


drop table T1;

//9

create cluster ABC (
    X number(10),
    V varchar2(12)  
) size 200 hashkeys 200;


drop cluster ABC;


//10


create table A (
    XA number(10),
    VA varchar2(12),
    Y number(10)
) cluster ABC (XA, VA);

insert into A values (3, '3', 3);

select * from A;


drop table A;

//11

create table B (
    XB number(10),
    VB varchar2(12),
    Z number(10)
) cluster ABC (XB, VB);

insert into B values (4, '4', 4);

select * from B;

drop table B;


//12

create table C (
    XC number(10),
    VC varchar2(12),
    W number(10)
) cluster ABC (XC, VC);

insert into C values (1, '1', 1);

select * from C;

drop table C;


//13

select table_name from user_tables
    where table_name = 'A'
    or table_name = 'B'
    or table_name = 'C';
select cluster_name from user_clusters;


//14

//select * from all_objects where object_name = 'C';

create synonym SC for BVV.C;

select * from SC;

drop synonym SC;


//15

create public synonym SB for BVV.B;

insert into BVV.B values (9, 'b', 5);

select * from SB;

//pdb
drop public synonym SB;


//16


DROP TABLE A_A CASCADE CONSTRAINTS PURGE;

create table A_A
(
    X number(20) primary key
);

DROP TABLE BB CASCADE CONSTRAINTS PURGE;

create table BB
(
    Y number(20),
    constraint fk_y foreign key (Y) references A_A (X)
);

insert into A_A(X) values (1);
insert into A_A(X) values (2);
insert into BB(Y) values (1);
insert into BB(Y) values (2);
commit;

drop view V1

create view V1 as select * from A_A inner join BB on A_A.X = BB.Y;

select * from V1;


//17

drop materialized view MV;

create materialized view MV
    build immediate
    refresh complete on demand
    next sysdate + numtodsinterval(10, 'second')
    as select * from A_A inner join BB on A_A.X = BB.Y;

select * from MV;
insert into A_A values (123);
insert into BB values (123);
commit;

begin
DBMS_MVIEW.REFRESH('MV');
end;
