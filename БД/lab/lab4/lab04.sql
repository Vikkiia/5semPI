alter session set container = FREEPDB1;
  
  //1 BVV
  
      select tablespace_name, file_name from dba_data_files;
      select tablespace_name, file_name from dba_temp_files;
  
  //2 BVV
  
CREATE TABLESPACE BVV_QDATA
    DATAFILE 'D:\БД\LAB02'
    SIZE 10M
    OFFLINE;

DROP TABLESPACE BVV_QDATA INCLUDING CONTENTS AND DATAFILES;
    
ALTER TABLESPACE BVV_QDATA ONLINE;

ALTER USER BVVCORE QUOTA 2M ON BVV_QDATA;


--BVVCORE

alter session set container = FREEPDB1;

DROP TABLE BVV_T1


CREATE TABLE BVV_T1(
    a INT PRIMARY KEY,
    b VARCHAR2(20)
) TABLESPACE BVV_QDATA;

INSERT INTO BVV_T1 VALUES(1, 'ONE');
INSERT INTO BVV_T1 VALUES(2, 'TWO');
INSERT INTO BVV_T1 VALUES(3, 'THREE');
SELECT * FROM BVV_T1;


//3 BVVCORE
   select * from user_segments where tablespace_name = 'BVV_QDATA';
   //BVV
   select * from dba_segments;
   
//4 BVVCORE
DROP TABLE BVV_T1

select * from user_segments where tablespace_name = 'BVV_QDATA';

select * from user_recyclebin;


//5 BVVCORE

  flashback table BVV_T1 to before drop;
SELECT * FROM BVV_T1;

//6 BVVCore

begin
    for x in 4..10000
    loop
    insert into BVV_T1 values(x, x);
    end loop;
    commit;
    end;

    select count(*) from BVV_T1;
    SELECT * from BVV_T1;
    
    //7 BVVCORE
    
    select * from user_segments where tablespace_name like 'BVV_QDATA';
    select extents, blocks, bytes from user_segments where tablespace_name like 'BVV_QDATA';
    
    //8
    //BVV
 drop tablespace BVV_QDATA including contents and datafiles;
 
 //9
 //BVV
    select GROUP# from v$log;
    select GROUP# from v$log where STATUS = 'CURRENT';
    
    //10
    //BVV
    
    select * from v$logfile
    
     //LECT * FROM V$LOGFILE ORDER BY GROUP#;
      
      //11 BVV
    
    ALTER SESSION SET CONTAINER = CDB$ROOT;

ALTER SYSTEM SWITCH LOGFILE;
SELECT GROUP#, STATUS, MEMBERS FROM V$LOG;
select current_timestamp from SYS.DUAL;
//14.10.25 21:10:59,223107000 EUROPE/MOSCOW

//12

//BVV_PDBConnect
alter database add logfile group 4 (
    '/opt/oracle/oradata/FREE/REDO041.LOG',
    '/opt/oracle/oradata/FREE/REDO042.LOG',
    '/opt/oracle/oradata/FREE/REDO043.LOG'
) size 100m;


SELECT GROUP#, STATUS, MEMBER FROM V$LOGFILE;

select * from v$logfile;
select current_scn from v$database;

ALTER SYSTEM SWITCH LOGFILE;


//13
//BVV_PDBConnect
ALTER SYSTEM CHECKPOINT;

SELECT GROUP#, STATUS, MEMBER FROM V$LOGFILE;


alter database drop logfile group 4;

--docker exec -it --user root oracle-free-official /bin/bash
--rm /opt/oracle/oradata/FREE/REDO041.LOG
--rm /opt/oracle/oradata/FREE/REDO042.LOG
--rm /opt/oracle/oradata/FREE/REDO043.LOG
select * from v$logfile;

--проверка на наличие
--ls /opt/oracle/oradata/FREE




SELECT group#, member FROM v$logfile;

//14
//BVV_PDBConnect

select instance_name, archiver from v$instance;
SELECT DBID, NAME, LOG_MODE FROM V$DATABASE;

//15


select * from v$archived_log;
select count(*) from v$archived_log;

 //select MAX(SEQUENCE#) from v$archived_log;
 
 
 
 
//17 

ALTER SYSTEM SET LOG_ARCHIVE_DEST_1 ='LOCATION=/opt/oracle/product/23ai/dbhomeFree/dbs/arch1_8_1212474384';
alter system switch logfile;
select * from v$archived_log;
SELECT * FROM V$LOG ORDER BY GROUP#;

//18 проверка
select instance_name, archiver from v$instance;

//19
select * from V$CONTROLFILE;

//20

show parameter control;

SELECT * FROM V$CONTROLFILE_RECORD_SECTION;

//21
select name, value from v$parameter where name = 'spfile';

//22

create pfile = '/opt/oracle/product/23ai/dbhomeFree/dbs/BVV_PFILE.ora' from spfile;


//23

select * from v$pwfile_users;
SHOW PARAMETER REMOTE_LOGIN_PASSWORDFILE;

//24
select * from v$diag_info;

//25
//select distinct segment_type from dba_segments;


select value from v$diag_info where name = 'Diag Alert';

--docker exec -it --user root oracle-free-official /bin/bash
--cd /opt/oracle/diag/rdbms/free/FREE/alert
--cat log.xml



















