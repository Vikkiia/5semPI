
alter session set container = BVV_PDB;
//BVV

GRANT CREATE SESSION TO BVV ;
GRANT CREATE TABLE TO BVV ;
GRANT CREATE VIEW TO BVV ;
GRANT CREATE SEQUENCE TO BVV ;
GRANT CREATE PROCEDURE TO BVV ;
GRANT CREATE TRIGGER TO BVV ;
GRANT CREATE TYPE TO BVV ;
GRANT CREATE SYNONYM TO BVV ;
GRANT CREATE MATERIALIZED VIEW TO BVV;
--1
create table STUDENT
(
  STUDENT      varchar(20) primary key,
  STUDENT_NAME varchar(100) unique,
  PULPIT       varchar(20)
);

--2
insert into STUDENT
values ('student1', 'Карпенко Дарья', 'P001');
insert into STUDENT
values ('student2', 'Кондратчк Анна', 'P001');
insert into STUDENT
values ('student3', 'Жилевич Ульяна', 'P002');
insert into STUDENT
values ('student4', 'Борисов Никита', 'P002');
insert into STUDENT
values ('student5', 'Пукович Татьяна', 'P003');
insert into STUDENT
values ('student6', 'Янкович Денис', 'P003');
insert into STUDENT
values ('student7', 'Казаков Илья', 'P004');
insert into STUDENT
values ('student8', 'Казакова Александра', 'P004');
insert into STUDENT
values ('student9', 'Шкода Кристина', 'P005');
insert into STUDENT
values ('student10', 'Борисов Антон', 'P005');

select * from STUDENT;
select * from PULPIT;

--3

create or replace trigger before_insert
before insert on STUDENT
begin
dbms_output.put_line('before_insert');
end;

create or replace trigger before_delete
before delete on STUDENT
begin
dbms_output.put_line('before_delete');
end;

create or replace trigger before_update
before update on STUDENT
begin
dbms_output.put_line('before_update');
end;

drop trigger before_insert;
drop trigger before_update;
drop trigger before_delete;

--4

insert into STUDENT values ('student11', 'Савич Андрей', 'P007');
select * from STUDENT;
update STUDENT set STUDENT = 'updated data' where PULPIT = 'P007';
delete from STUDENT where PULPIT = 'P007';

--5

create or replace trigger before_insert_row
before insert on STUDENT
for each row
begin
dbms_output.put_line('before_insert_row');
end;

create or replace trigger before_update_row
before update on STUDENT
for each row
begin
dbms_output.put_line('before_update_row');
end;

create or replace trigger before_delete_row
before delete on STUDENT
for each row
begin
dbms_output.put_line('before_delete_row');
end;

insert into STUDENT values ('student12', 'Петрович', 'P006');
select * from STUDENT;
update STUDENT set STUDENT = 'обнова' where PULPIT = 'P006';
delete from STUDENT where PULPIT = 'P006';

select * from STUDENT;

drop trigger before_insert_row;
drop trigger before_update_row;
drop trigger before_delete_row;



--6

create or replace trigger TRIGGER_DML
    before insert or update or delete on STUDENT
begin
    if INSERTING then
        DBMS_OUTPUT.PUT_LINE('TRIGGER_BEFORE_INSERTING');
    ELSIF UPDATING then
        DBMS_OUTPUT.PUT_LINE('TRIGGER_BEFORE_UPDATING');
    ELSIF DELETING then
        DBMS_OUTPUT.PUT_LINE('TRIGGER_BEFORE_DELETING');
    end if;
end;


insert into STUDENT values ('student12', 'Петрович', 'P006');
select * from STUDENT;
update STUDENT set STUDENT = 'обнова' where PULPIT = 'P006';
delete from STUDENT where PULPIT = 'P006';

drop trigger TRIGGER_DML;



--7
create or replace trigger after_insert
after insert on STUDENT
begin
dbms_output.put_line('after_insert');
end;

create or replace trigger after_update
after update on STUDENT
begin
dbms_output.put_line('after_update');
end;

create or replace trigger after_delete
after delete on STUDENT
begin
dbms_output.put_line('after_delete');
end;

insert into STUDENT values ('student12', 'ALTERRR', 'P006');
select * from STUDENT;
update STUDENT set STUDENT = 'AFTER UPDATE' where PULPIT = 'P006';
delete from STUDENT where PULPIT = 'P006';

drop trigger after_insert;
drop trigger after_update;
drop trigger after_delete;


--8

create or replace trigger after_insert_row
after insert on STUDENT
for each row
begin
dbms_output.put_line('after_insert_row');
end;

create or replace trigger after_update_row
after update on STUDENT
for each row
begin
dbms_output.put_line('after_update_row');
end;

create or replace trigger after_delete_row
after delete on STUDENT
for each row
begin
dbms_output.put_line('after_delete_row');
end;

insert into STUDENT values ('student12', 'ALTERRR', 'P006');
select * from STUDENT;
update STUDENT set STUDENT = 'AFTER!!!!' where PULPIT = 'P006';
delete from STUDENT where PULPIT = 'P006';


drop trigger after_insert_row;
drop trigger after_update_row;
drop trigger after_delete_row;

--9

create table AUDITS (
    OperationDate date,
    OperationType varchar2(50),
    TriggerName varchar2(50),
    Data varchar2(40)
);

drop table AUDITS;

--10

create or replace trigger Trigger_before_row
before insert or update or delete
on STUDENT for each row
begin
if inserting then
dbms_output.put_line('dml_before_row insert');
insert into AUDITS (operationdate, operationtype, triggername, data)
values (sysdate, 'insert', 'dml_before_row', :new.STUDENT || ' ' || :new.STUDENT_NAME);
elsif updating then
dbms_output.put_line('dml_before_row update');
insert into AUDITS (operationdate, operationtype, triggername, data)
values (sysdate, 'update', 'dml_before_row', :old.STUDENT || ' ' || :old.STUDENT_NAME || '->' || :new.STUDENT || ' ' || :new.STUDENT_NAME);
elsif deleting then
dbms_output.put_line('dml_before_trigger_row delete');
insert into AUDITS (operationdate, operationtype, triggername, data)
values (sysdate, 'delete', 'dml_before_row', :old.STUDENT || ' ' || :old.STUDENT_NAME);
end if;
end;

create or replace trigger Trigger_before
before insert or update or delete
on STUDENT
begin
  if inserting then
    dbms_output.put_line('dml_before insert');
    insert into AUDITS (operationdate, operationtype, triggername, data)
    values (sysdate, 'insert', 'dml_before',
            'before insert on STUDENT');
  elsif updating then
    dbms_output.put_line('dml_before update');
    insert into AUDITS (operationdate, operationtype, triggername, data)
    values (sysdate, 'update', 'dml_before',
            'before update on STUDENT');
  elsif deleting then
    dbms_output.put_line('dml_before delete');
    insert into AUDITS (operationdate, operationtype, triggername, data)
    values (sysdate, 'delete', 'dml_before',
            'before delete on STUDENT');
  end if;
end;

create or replace trigger Trigger_after_row
after insert or update or delete
on STUDENT for each row
begin
if inserting then
dbms_output.put_line('dml_after_row insert');
insert into AUDITS (operationdate, operationtype, triggername, data)
values (sysdate, 'insert', 'dml_after_row', :new.STUDENT || ' ' || :new.STUDENT_NAME);
elsif updating then
dbms_output.put_line('dml_after_row update');
insert into AUDITS (operationdate, operationtype, triggername, data)
values (sysdate, 'update', 'dml_after_row', :old.STUDENT || ' ' || :old.STUDENT_NAME || '->' || :new.STUDENT || ' ' || :new.STUDENT_NAME);
elsif deleting then
dbms_output.put_line('dml_after_row delete');
insert into AUDITS (operationdate, operationtype, triggername, data)
values (sysdate, 'delete', 'dml_after_row', :old.STUDENT || ' ' || :old.STUDENT_NAME);
end if;
end;


create or replace trigger Trigger_after
after insert or update or delete
on STUDENT
begin
  if inserting then
    dbms_output.put_line('dml_after insert');
    insert into AUDITS (operationdate, operationtype, triggername, data)
    values (sysdate, 'insert', 'dml_after',
            'after insert on STUDENT');

  elsif updating then
    dbms_output.put_line('dml_after update');
    insert into AUDITS (operationdate, operationtype, triggername, data)
    values (sysdate, 'update', 'dml_after',
            'after update on STUDENT');

  elsif deleting then
    dbms_output.put_line('dml_after delete');
    insert into AUDITS (operationdate, operationtype, triggername, data)
    values (sysdate, 'delete', 'dml_after',
            'after delete on STUDENT');
  end if;
end;
/


insert into STUDENT values ('student12', 'ИМЯ', 'P006');
select * from STUDENT;
update STUDENT set STUDENT = 'ОКАK' where PULPIT = 'P006';
delete from STUDENT where PULPIT = 'P006';
select * from AUDITS;

drop trigger Trigger_before_row;
drop trigger Trigger_before;
drop trigger Trigger_after_row;
drop trigger Trigger_after;


--11

INSERT INTO STUDENT VALUES ('student1', 'Новый студент', 'P001');

select * from AUDITS;

--12

drop table STUDENT;


CREATE OR REPLACE TRIGGER prevent_drop_table
BEFORE DROP ON SCHEMA
BEGIN
IF dictionary_obj_name = 'STUDENT' THEN
RAISE_APPLICATION_ERROR(-20000, 'You cannot drop the table STUDENT');
END IF;
END;

select * from STUDENT;

drop trigger prevent_drop_table;



--13
drop table AUDITS;

--select TRIGGER_NAME, STATUS from USER_TRIGGERS;

create table STUDENT
(
  STUDENT      varchar(20) primary key,
  STUDENT_NAME varchar(100) unique,
  PULPIT       varchar(20)
);

insert into STUDENT
values ('student1', 'Карпенко Дарья', 'P001');
insert into STUDENT
values ('student2', 'Кондратчк Анна', 'P001');
insert into STUDENT
values ('student3', 'Жилевич Ульяна', 'P002');
insert into STUDENT
values ('student4', 'Борисов Никита', 'P002');
insert into STUDENT
values ('student5', 'Пукович Татьяна', 'P003');
insert into STUDENT
values ('student6', 'Янкович Денис', 'P003');
insert into STUDENT
values ('student7', 'Казаков Илья', 'P004');
insert into STUDENT
values ('student8', 'Казакова Александра', 'P004');
insert into STUDENT
values ('student9', 'Шкода Кристина', 'P005');
insert into STUDENT
values ('student10', 'Борисов Антон', 'P005');

--14


CREATE OR REPLACE VIEW STUDENT_view AS SELECT * FROM STUDENT;

CREATE OR REPLACE TRIGGER instead_of_insert_trigger
INSTEAD OF INSERT ON STUDENT_view
FOR EACH ROW
BEGIN
  INSERT INTO STUDENT (STUDENT, STUDENT_NAME, PULPIT)
  VALUES (:NEW.STUDENT, :NEW.STUDENT_NAME, :NEW.PULPIT);
END instead_of_insert_trigger;

insert into STUDENT_view values ('student13', 'Сашка', 'P007');
delete from STUDENT_view where PULPIT = 'P007';

select * from STUDENT;


select * from AUDITS;
drop view STUDENT_view;
drop trigger instead_of_insert_trigger;