alter session set container = CDB$ROOT;
//BVV_PDBConnect

--1
DECLARE
   PROCEDURE GET_TEACHERS (PCODE TEACHER.PULPIT%TYPE) IS
   BEGIN
      FOR rec IN (SELECT TEACHER_NAME, PULPIT
                  FROM TEACHER
                  WHERE PULPIT = PCODE) LOOP
         DBMS_OUTPUT.PUT_LINE('Teacher: ' || rec.TEACHER_NAME || ' Pulpit: ' || rec.PULPIT);
      END LOOP;
   END GET_TEACHERS;
begin
   GET_TEACHERS('P003');
END;

select * from PULPIT;
--select * from TEACHER;


--2-3
declare
function get_num_teachers(pcode teacher.pulpit%type) 
return number
is 
num number;
begin
select count(*) into num from teacher where pulpit = pcode;
return num;
end get_num_teachers;
begin
dbms_output.put_line('Количество преподавателей: '|| get_num_teachers('P003'));
end;


--4
create or replace procedure GET_TEACHERS(FCODE FACULTY.FACULTY%TYPE) is
begin
  for i in (select * from TEACHER where PULPIT in (select PULPIT from PULPIT where FACULTY = FCODE))
    loop
      dbms_output.put_line(i.TEACHER_NAME);
    end loop;
end;
/
begin
  GET_TEACHERS('F002');
end;

select * from PULPIT;
select * from TEACHER;

create or replace procedure get_subjects(pcode subject.pulpit%type) is
begin 
for i in (select * from subject where pulpit=pcode)
loop
dbms_output.put_line(i.subject_name || ' ' || i.pulpit);
end loop;
end;
/
begin
get_subjects('P002');
end;

select * from SUBJECT;


--5

declare
function get_num_teachers(fcode faculty.faculty%type) return number
is
num number;
begin
select count(*) into num from teacher
inner join PULPIT on PULPIT.PULPIT = TEACHER.PULPIT
inner join FACULTY on FACULTY.FACULTY = PULPIT.FACULTY
where FACULTY.FACULTY = fcode;
return num;
end get_num_teachers;
begin
dbms_output.put_line('Количество преподавателей: ' || get_num_teachers('F002'));
end;


declare 
    function GET_NUM_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE) return number
    is
        result_num number;
    begin
        select count(SUBJECT) into result_num from SUBJECT where PULPIT=PCODE;
        return result_num;
    end GET_NUM_SUBJECTS;
begin
    dbms_output.put_line('Количество дисциплин: '|| GET_NUM_SUBJECTS('P002'));
end;



--6

CREATE OR REPLACE PACKAGE TEACHERS AS
    PROCEDURE GET_TEACHERS (FCODE FACULTY.FACULTY%TYPE);
    PROCEDURE GET_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE);
    FUNCTION GET_NUM_TEACHERS (FCODE FACULTY.FACULTY%TYPE) RETURN NUMBER;
    FUNCTION GET_NUM_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE) RETURN NUMBER;
END TEACHERS;


CREATE OR REPLACE PACKAGE BODY TEACHERS AS

    PROCEDURE GET_TEACHERS (FCODE FACULTY.FACULTY%TYPE) IS
    BEGIN
        FOR rec IN (
            SELECT TEACHER.TEACHER_NAME, TEACHER.PULPIT
            FROM TEACHER
            JOIN PULPIT ON TEACHER.PULPIT = PULPIT.PULPIT
            WHERE PULPIT.FACULTY = FCODE
        ) LOOP
            DBMS_OUTPUT.PUT_LINE('Teacher: ' || rec.TEACHER_NAME || ' Pulpit: ' || rec.PULPIT);
        END LOOP;
    END GET_TEACHERS;

  
    PROCEDURE GET_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE) IS
    BEGIN
        FOR rec IN (
            SELECT SUBJECT.SUBJECT_NAME, SUBJECT.PULPIT
            FROM SUBJECT
            WHERE SUBJECT.PULPIT = PCODE
        ) LOOP
            DBMS_OUTPUT.PUT_LINE('Subject: ' || rec.SUBJECT_NAME || ' Pulpit: ' || rec.PULPIT);
        END LOOP;
    END GET_SUBJECTS;

  
    FUNCTION GET_NUM_TEACHERS (FCODE FACULTY.FACULTY%TYPE) RETURN NUMBER IS
        num NUMBER;
    BEGIN
        SELECT COUNT(*) INTO num
        FROM TEACHER
        JOIN PULPIT ON TEACHER.PULPIT = PULPIT.PULPIT
        WHERE PULPIT.FACULTY = FCODE;
        RETURN num;
    END GET_NUM_TEACHERS;

    
    FUNCTION GET_NUM_SUBJECTS (PCODE SUBJECT.PULPIT%TYPE) RETURN NUMBER IS
        num NUMBER;
    BEGIN
        SELECT COUNT(*) INTO num
        FROM SUBJECT
        WHERE SUBJECT.PULPIT = PCODE;
        RETURN num;
    END GET_NUM_SUBJECTS;

END TEACHERS;


--7
DECLARE
    faculty_code FACULTY.FACULTY%TYPE := 'F002';  
    pulpit_code SUBJECT.PULPIT%TYPE := 'P002';    

BEGIN
    DBMS_OUTPUT.PUT_LINE('Teachers in Faculty ' || faculty_code || ':');
    TEACHERS.GET_TEACHERS(faculty_code);

    DBMS_OUTPUT.PUT_LINE('Subjects in Pulpit ' || pulpit_code || ':');
    TEACHERS.GET_SUBJECTS(pulpit_code);


    DECLARE
        num_teachers NUMBER;
    BEGIN
        num_teachers := TEACHERS.GET_NUM_TEACHERS(faculty_code);
        DBMS_OUTPUT.PUT_LINE('Number of teachers in Faculty ' || faculty_code || ': ' || num_teachers);
    END;

    DECLARE
        num_subjects NUMBER;
    BEGIN
        num_subjects := TEACHERS.GET_NUM_SUBJECTS(pulpit_code);
        DBMS_OUTPUT.PUT_LINE('Number of subjects in Pulpit ' || pulpit_code || ': ' || num_subjects);
    END;

END;
