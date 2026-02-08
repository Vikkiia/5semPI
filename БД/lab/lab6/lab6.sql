//1


--docker exec -it oracle-free-official /bin/bash

--cat /opt/oracle/product/23ai/dbhomeFree/network/admin/sqlnet.ora
--cat /opt/oracle/product/23ai/dbhomeFree/network/admin/tnsnames.ora

//2

--sqlplus system/StrongPass1!@//localhost:1521/FREE
--SHOW PARAMETER INSTANCE

//3


--select TABLESPACE_NAME from dba_tablespaces;
--select TABLESPACE_NAME, FILE_NAME from dba_data_files;--
--select TABLESPACE_NAME, FILE_NAME from dba_temp_files;
--select ROLE from dba_roles;
--select USERNAME from dba_users;



//4
-- win+R 
--regedit

//5

--docker exec -it oracle-free-official bash

--export DISPLAY=host.docker.internal:0

--/opt/oracle/product/23ai/dbhomeFree/bin/netmgr


--docker exec -it oracle-free-official bash
--cat /opt/oracle/product/23ai/dbhomeFree/network/admin/tnsnames.ora


//6

--sqlplus system/StrongPass1!@//localhost:1521/BVV_PDB



----docker exec -it oracle-free-official bash
--sqlplus sys/BVV_SID as sysdba

--sqlplus BVV/BVV_SID as sysdba
//7

-- create table myTable (a number, b number);
-- insert into myTable values (1, 2);
-- insert into myTable values (3, 4);
-- commit;
--select * from myTable;


//8

-- help timing
-- set timing on;
-- select * from myTable;
-- set timing off;

-- 9

-- help describe;
-- describe myTable;


//10
--select segment_name from user_segments;

//11
--create view view_segments as select count(segment_name) segments_count, sum(extents) extents_count, sum(blocks) blocks_count, sum(bytes) memory_size from user_segments;
--select * from view_segments;


