//1
ALTER SESSION SET CONTAINER = FREEPDB1;


CREATE TABLESPACE TS_BVV
DATAFILE 'D:\БД\LAB02\TS_BVV.dbf'
size 7M
autoextend on next 5M
maxsize 20M;

select TABLESPACE_NAME, STATUS, contents, logging from USER_TABLESPACES;

drop tablespace TS_BVV  INCLUDING CONTENTS AND DATAFILES;

commit

//2


create temporary tablespace TS_BVV_TEMP
tempfile 'D:\БД\LAB02\TS_BVV_TEMP.dbf'
size 5M
autoextend on next 3M
maxsize 30M

select TABLESPACE_NAME, STATUS, contents, logging from USER_TABLESPACES;

drop tablespace TS_BVV_TEMP INCLUDING CONTENTS AND DATAFILES;

//3

select * from dba_tablespaces;
select * from dba_data_files;
select * from dba_temp_files;

//SELECT * FROM dictionary; 


//4

create role RL_BVVCORE


grant create session to RL_BVVCORE;

grant create table to RL_BVVCORE;
grant drop any table to RL_BVVCORE;

grant create any view to RL_BVVCORE;
grant drop any view to RL_BVVCORE;

grant create  procedure to RL_BVVCORE;
grant drop any procedure to RL_BVVCORE;

drop role RL_BVVCORE;


//5

select * from dba_roles where role = 'RL_BVVCORE';
select * from dba_sys_privs where grantee = 'RL_BVVCORE';


//6

create profile PF_BVVCORE LIMIT
    PASSWORD_LIFE_TIME 180
    SESSIONS_PER_USER 3
    FAILED_LOGIN_ATTEMPTS 7
    PASSWORD_LOCK_TIME 1
    PASSWORD_REUSE_TIME 10
    PASSWORD_GRACE_TIME DEFAULT
    CONNECT_TIME 180
    IDLE_TIME 30;
    
    drop profile PF_BVVCORE
    
//7

select * from dba_profiles;
select * from dba_profiles where profile = 'PF_BVVCORE';
select * from dba_profiles where profile = 'DEFAULT';


//8

    create user BVVCORE identified by 12345
    default tablespace TS_BVV QUOTA UNLIMITED ON TS_BVV
    temporary tablespace TS_BVV_TEMP
    profile PF_BVVCORE
    account UNLOCK;
    PASSWORD EXPIRE;
    
    Alter User BVVCORE PASSWORD EXPIRE; 
    
    GRANT RL_BVVCORE to BVVCORE;
  


DROP USER BVVCORE CASCADE;

  
  
  
  
  //11

CREATE TABLESPACE BVV_QDATA
DATAFILE 'D:\БД\LAB02\BVV_QDATA.dbf' SIZE 10M
OFFLINE;

ALTER TABLESPACE BVV_QDATA ONLINE;
ALTER USER BVVCORE QUOTA 2M ON BVV_QDATA;

CREATE TABLE BVV_Tab1(
    id NUMBER PRIMARY KEY,
    name VARCHAR2(50)
) TABLESPACE BVV_QDATA;

INSERT INTO BVV_Tab1 VALUES (1, 'lol');
INSERT INTO BVV_Tab1 VALUES (2, 'kek');
INSERT INTO BVV_Tab1 VALUES (3, 'chebyrek');

select * from BVV_Tab1;

drop TABLESPACE BVV_QDATA INCLUDING CONTENTS AND DATAFILES; 
  

