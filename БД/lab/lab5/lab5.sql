
//1 BVV_PDBConnect

select sum(value) from v$sga;

//2

SELECT NAME POOL_NAME, VALUE SIZE_IN_BYTES FROM V$SGA;

//3+

SELECT COMPONENT, GRANULE_SIZE FROM V$SGA_DYNAMIC_COMPONENTS;


//4

SELECT CURRENT_SIZE FROM V$SGA_DYNAMIC_FREE_MEMORY;

//5

   SELECT value FROM v$parameter WHERE name = 'sga_target';
    SELECT value FROM v$parameter WHERE name = 'sga_max_size';
    
//6


 // select component, current_size, min_size, MAX_SIZE from v$sga_dynamic_components
   // where component='KEEP buffer cache' or component='DEFAULT buffer cache' or component='RECYCLE buffer cache';


select component, current_size, max_size, min_size from v$sga_dynamic_components where component like '%KEEP%' or component like '%DEFAULT%' or component like '%RECYCLE%';


//7

CREATE TABLE KEEP_T (a NUMBER) STORAGE (BUFFER_POOL KEEP);
INSERT INTO KEEP_T VALUES(1);
INSERT INTO KEEP_T VALUES(2);
INSERT INTO KEEP_T VALUES(3);

COMMIT;

select * from KEEP_T;
SELECT SEGMENT_NAME, SEGMENT_TYPE, TABLESPACE_NAME, BUFFER_POOL FROM USER_SEGMENTS WHERE SEGMENT_NAME LIKE 'KEEP%';


drop table KEEP_T;

//8

CREATE TABLE DEFAULT_T (b NUMBER) STORAGE (BUFFER_POOL DEFAULT);
INSERT INTO DEFAULT_T VALUES(4);
INSERT INTO DEFAULT_T VALUES(5);
SELECT * FROM DEFAULT_T;
SELECT SEGMENT_NAME, SEGMENT_TYPE, TABLESPACE_NAME, BUFFER_POOL from user_segments where segment_name like 'DEFAULT_T%';


drop table DEFAULT_T;

//9

SHOW PARAMETER LOG_BUFFER;

//10

SELECT POOL, NAME, BYTES FROM V$SGASTAT WHERE POOL = 'large pool' AND NAME = 'free memory';

//11
select username, sid, server, status from v$session where username is not null;


//12


SELECT SID, PROCESS, NAME, DESCRIPTION, PROGRAM
FROM V$SESSION s JOIN V$BGPROCESS b on s.paddr = b.paddr
WHERE s.STATUS = 'ACTIVE';


//13

select * from v$process;

SELECT PID, PNAME, USERNAME, PROGRAM FROM V$PROCESS;


//14

SHOW PARAMETER DB_WRITER_PROCESSES;

SELECT * FROM V$PROCESS WHERE PNAME LIKE 'DBW%';

//15

  select * from v$active_services;
  
  //16
  
  
   SHOW PARAMETER DISPATCHERS;
SELECT * FROM V$DISPATCHER;

//17
--docker exec -it oracle-free-official /bin/bash

--cat /opt/oracle/product/23ai/dbhomeFree/network/admin/listener.ora

//18 
//cat /opt/oracle/product/23ai/dbhomeFree/network/admin/listener.ora

//19










