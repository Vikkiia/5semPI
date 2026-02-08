alter session set container = CDB$ROOT;
//BVV_PDBConnect


create table AUDITORIUM_TYPE (
  AUDITORIUM_TYPE varchar(20) primary key,
  AUDIOTRIUM_TYPENAME varchar(100) unique
);
select * from AUDITORIUM;

create table AUDITORIUM (
  AUDITORIUM varchar(20) primary key,
  AUDITORIUM_NAME varchar(100) unique,
  AUDITORIUM_CAPACITY int,
  AUDITORIUM_TYPE varchar(20),
  foreign key (AUDITORIUM_TYPE) references AUDITORIUM_TYPE(AUDITORIUM_TYPE)
);

INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY, AUDITORIUM_TYPE)
VALUES
  ('206-1', '206-1', 15, 'Computer Class'),
  ('200-3a', '200-3a', 300, 'Lecture'),
  ('408-1', '408-1', 15, 'Computer Class'),
  ('325-4', '325-4', 30, 'Seminar'),
  

create table FACULTY (
  FACULTY varchar(20) primary key,
  FACULTY_NAME varchar(100) unique
);

create table PULPIT (
  PULPIT varchar(20) primary key,
  PULPIT_NAME varchar(100) unique,
  FACULTY varchar(20),
  foreign key (FACULTY) references FACULTY(FACULTY)
);

create table TEACHER (
  TEACHER varchar(20) primary key,
  TEACHER_NAME varchar(100) unique,
  PULPIT varchar(20),
  foreign key (PULPIT) references PULPIT(PULPIT)
);

create table SUBJECT (
  SUBJECT varchar(20) primary key,
  SUBJECT_NAME varchar(100) unique,
  PULPIT varchar(20),
  foreign key (PULPIT) references PULPIT(PULPIT)
);


INSERT INTO AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDIOTRIUM_TYPENAME)
VALUES ('Lecture', 'Лекционная аудитория');
INSERT INTO AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDIOTRIUM_TYPENAME)
VALUES ('Computer Class', 'Компьютерный');

INSERT INTO AUDITORIUM_TYPE (AUDITORIUM_TYPE, AUDIOTRIUM_TYPENAME)
VALUES ('Seminar', 'Семинарская комната');



INSERT INTO FACULTY (FACULTY, FACULTY_NAME)
VALUES ('F001', 'Факультет информационных технологий');
INSERT INTO FACULTY (FACULTY, FACULTY_NAME)
VALUES ('F002', 'Факультет лесной инженерии');
INSERT INTO FACULTY (FACULTY, FACULTY_NAME)
VALUES ('F003', 'Факультет принттехнологий и медиакоммуникаций');
INSERT INTO FACULTY (FACULTY, FACULTY_NAME)
VALUES ('F004', 'Факультет экономики');





INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY)
VALUES ('P001', 'Кафедра программной инженерии', 'F001');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY)
VALUES ('P002', 'Кафедра физики', 'F001');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY)
VALUES ('P003', 'Кафедра инженерной графики', 'F001');

INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY)
VALUES ('P004', 'Кафедра энергосбережения, гидравлики и теплотехники', 'F002');
INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY)
VALUES ('P005', 'Кафедра материаловедения и проектирования технических систем', 'F002');


INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY)
VALUES ('P006', 'Кафедра белорусской филологии', 'F003');

INSERT INTO PULPIT (PULPIT, PULPIT_NAME, FACULTY)
VALUES ('P007', 'Кафедра экономической теории и маркетинга', 'F004');

-- Заполняем таблицу TEACHER
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT)
VALUES ('T001', 'Смелов Владимир Владиславович', 'P001');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT)
VALUES ('T002', 'Осоко Сергей Анатольевич', 'P001');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT)
VALUES ('T003', 'Гурина Кристина Сергеевна', 'P001');

INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT)
VALUES ('T004', 'Маршалова Галина Сергеевна', 'P002');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT)
VALUES ('T005', 'Грудо Сергей Казимирович', 'P003');
INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT)
VALUES ('T006', 'Криштаносов Виталий Брониславович', 'P004');




INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
VALUES ('S001', 'Опрационные системы', 'P001');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
VALUES ('S002', 'База данных', 'P001');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
VALUES ('S003', 'Технологии программирования в Интернете', 'P001');

INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
VALUES ('S004', 'Природные ресурсы и их использование', 'P002');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
VALUES ('S005', 'Электротехника и электроника', 'P002');


INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
VALUES ('S006', 'Проектирование и дизайн упаковки', 'P003');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
VALUES ('S007', 'Защита печатной продукции', 'P003');

INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
VALUES ('S008', 'Бухгалтерский учет', 'P004');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
VALUES ('S009', 'Бизнес-анализ', 'P004');
INSERT INTO SUBJECT (SUBJECT, SUBJECT_NAME, PULPIT)
VALUES ('S010', 'Финансы организации', 'P004');


--1
begin
    NULL;
end;


--2

begin 
dbms_output.put_line('hello world'); 
end;


--sqlplus system/StrongPass1!@//localhost:1521/FREE
--set serveroutput on 
--begin dbms_output.put_line('Hello World!'); end;
--/


--3

declare
    temp number;
begin
    temp := 1/0;
exception
    when others then
        dbms_output.put_line('Error: ' || sqlerrm);
        dbms_output.put_line('Error code: ' || sqlcode);
end;


--4


declare
    temp number;
begin
    declare
    begin
        temp := 1/0;
    exception
        when others then
            dbms_output.put_line('Error: ' || sqlerrm);
            dbms_output.put_line('Error code: ' || sqlcode);
    end;
    dbms_output.put_line('Hello World!');
end;


--5


--alter system set plsql_warnings = 'ENABLE:INFORMATIONAL';


show parameter plsql_warnings;
select type, name, value from v$parameter where name = 'plsql_warnings';

--6

select keyword from v$reserved_words where length = 1 and  keyword!='A' ;

--7
select keyword from v$reserved_words where length > 1 and  keyword!='A' order by keyword;

--8

select name, value from v$parameter where name like '%plsql%';

--show parameter plsql;


--10-11

declare
    n1 number(10) := 11;
    n2 number(10) := 2;
    n3 number(10);
begin
    dbms_output.put_line('n1 = ' || n1);
    dbms_output.put_line('n2 = ' || n2);
    n3 := n1 - n2;
    dbms_output.put_line('n1 - n2 = ' || n3);
    n3 := n1 / n2;
    dbms_output.put_line('n1 / n2 = ' || n3);
    n3 := MOD(n1, n2);
    dbms_output.put_line('Остаток от деления n1 MOD n2 = ' || n3);
end;


--12-13

--точность масштаб
declare
    n4 number(10, 4) := 123.12456754;
    n5 number(10, -2) := 154.193;
    n6 number(10, -6) := 123.5277;
    n7 number(6, 2) := 1234.567 ;
begin
    dbms_output.put_line('n4 = ' || n4);
    dbms_output.put_line('n5 = ' || n5);
    dbms_output.put_line('n6 = ' || n6);
    dbms_output.put_line('n7 = ' || n7);
end;


--14-15

declare
    binary_float_number binary_float := 123456789.21414;
    binary_double_number binary_double := 123.12421541;
begin
    dbms_output.put_line('Binary float: ' || binary_float_number);
    dbms_output.put_line('Binary double: ' || binary_double_number);
end;



--16-17
declare
    number_E number(10, 3) := 2E+2;
    b1 boolean := true;
    b2 boolean := false;
begin
    dbms_output.put_line('E: ' || number_E);
    if b1 then
        dbms_output.put_line('b1: ' || 'true');
    end if;
    if not b1 then
        dbms_output.put_line('b1: ' || 'false');
    end if;
    if b2 then
        dbms_output.put_line('b2: ' || 'true');
    end if;
    if not b2 then
        dbms_output.put_line('b2: ' || 'false');
    end if;
end;


--18

declare
    const_number constant number(5) := 10;
    const_varchar constant varchar2(10) := 'lojka';
    const_char constant char(10) := 'vilka';
begin
    dbms_output.put_line('const_number: ' || const_number);
    dbms_output.put_line('const_number * 2: ' || const_number * 2);
    dbms_output.put_line('const_varchar: ' || const_varchar);
    dbms_output.put_line('const_char: ' || const_char);
     dbms_output.put_line('Объединение const_varchar и const_char: ' || const_varchar || ' ' || const_char);  
end;


--19

select * from pulpit;

declare
    pulp pulpit.pulpit%type;
begin
    pulp := 'ИТ';
    dbms_output.put_line(pulp);
end;

--20
DECLARE
  AUDITORIUM_TYPE_ROW AUDITORIUM_TYPE%ROWTYPE;
BEGIN 
  AUDITORIUM_TYPE_ROW.AUDIOTRIUM_TYPENAME := '206-1';
  AUDITORIUM_TYPE_ROW.AUDITORIUM_TYPE := 'Computer Class';

  DBMS_OUTPUT.PUT_LINE(AUDITORIUM_TYPE_ROW.AUDITORIUM_TYPE);
  DBMS_OUTPUT.PUT_LINE(AUDITORIUM_TYPE_ROW.AUDIOTRIUM_TYPENAME);
end;
    
    
    --21-22
    
    declare
    x pls_integer := 7;
begin
    if x < 10 then
        dbms_output.put_line('x < 10');
    elsif x > 10 then
        dbms_output.put_line('x > 10');
    else
        dbms_output.put_line('x = 10');
    end if;
end;

--23


DECLARE
  temp NUMBER := 1;
BEGIN
  CASE temp
    WHEN 1 THEN
      DBMS_OUTPUT.PUT_LINE('temp = 1');
    WHEN 2 THEN
      DBMS_OUTPUT.PUT_LINE('temp = 2');
    WHEN 3 THEN
      DBMS_OUTPUT.PUT_LINE('temp = 3');
    ELSE
      DBMS_OUTPUT.PUT_LINE('temp is null');
  END CASE;
END;


--24

declare
    v_num number := 1;
begin
    loop
        dbms_output.put_line(v_num);
        v_num := v_num + 1;
        exit when v_num > 10;
    end loop;
end;

-- 25

declare
    v_num number := 1;
begin
    while v_num <= 10
        loop
            dbms_output.put_line(v_num);
            v_num := v_num + 1;
        end loop;
end;

-- 26

declare
    v_num number := 1;
begin
    for i in 1..10 loop
        dbms_output.put_line(i);
    end loop;
end;
