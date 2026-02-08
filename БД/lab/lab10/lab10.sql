alter session set container = CDB$ROOT;
//BVV_PDBConnect


--1


alter table TEACHER add BIRTHDAY DATE;
alter table TEACHER add SALARY NUMBER;

select * from teacher;

ALTER SESSION SET NLS_DATE_FORMAT = 'DD.MM.YYYY';
BEGIN
  FOR rec IN (SELECT TEACHER FROM TEACHER) LOOP
    UPDATE TEACHER
    SET
      SALARY = ROUND(DBMS_RANDOM.VALUE(700, 5000)),
      BIRTHDAY = DATE '1960-01-01' + TRUNC(DBMS_RANDOM.VALUE(0,DATE '1990-12-31' - DATE '1960-01-01'))
    WHERE TEACHER = rec.TEACHER;
  END LOOP;
END;

commit;
select * from teacher;



rollback


--2

SELECT
    regexp_substr(teacher_name, '\S+', 1, 1) || ' ' ||
    substr(regexp_substr(teacher_name, '\S+', 1, 2), 1, 1) || '.' ||
    substr(regexp_substr(teacher_name, '\S+', 1, 3), 1, 1) || '.' AS FIO
FROM teacher;

--3

UPDATE TEACHER SET BIRTHDAY = '05-12-1988' WHERE TEACHER = 'T003';

SELECT TO_CHAR(BIRTHDAY, 'Day') FROM teacher WHERE TEACHER = 'T003';


SELECT teacher_name, BIRTHDAY FROM teacher WHERE TRIM(TO_CHAR(BIRTHDAY, 'Day')) = 'Понедельник'; 

select * from teacher;


--4

create view BirthdayNextMonth as select TEACHER_NAME, BIRTHDAY from TEACHER
    where extract(MONTH from BIRTHDAY) = extract(MONTH from add_months(SYSDATE, 1));
    
select * from BirthdayNextMonth;

drop view BirthdayNextMonth;


--5

CREATE VIEW TeachersByMonth AS
SELECT EXTRACT(MONTH FROM BIRTHDAY) AS MONTH, COUNT(*) AS NUM_OF_TEACHERS FROM  TEACHER
GROUP BY EXTRACT(MONTH FROM BIRTHDAY)           
ORDER BY MONTH; 


    create view TeachersByMonth as
select to_char(birthday, 'Month') as month_name, count(*) as num_teachers
from teacher
group by to_char(birthday, 'Month')

select * from TeachersByMonth;

drop view TeachersByMonth;
select * from teacher;

--6
UPDATE TEACHER SET BIRTHDAY = '13-04-1966' WHERE TEACHER = 'T004';

DECLARE
    CURSOR teachers_anniversary IS
        SELECT TEACHER, BIRTHDAY, TEACHER_NAME
        FROM TEACHER
        WHERE MOD(EXTRACT(YEAR FROM SYSDATE) + 1 - EXTRACT(YEAR FROM BIRTHDAY), 5) = 0
          OR MOD(EXTRACT(YEAR FROM SYSDATE) + 1 - EXTRACT(YEAR FROM BIRTHDAY), 10) = 0;

    v_teacher TEACHER.TEACHER%TYPE;
    v_birthday DATE;
    v_teacher_name TEACHER.TEACHER_NAME%TYPE;
BEGIN
    FOR rec IN teachers_anniversary LOOP
        v_teacher := rec.TEACHER;
        v_birthday := rec.BIRTHDAY;
        v_teacher_name := rec.TEACHER_NAME;
        DBMS_OUTPUT.PUT_LINE(v_teacher || ' - ' || TO_CHAR(v_birthday, 'DD.MM.YYYY')|| ' - ' || v_teacher_name);
    END LOOP;
END;

select * from teacher;


--7
select * from TEACHER;
select * from PULPIT;


INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT, BIRTHDAY, SALARY)
VALUES ('T005', 'Иванов Иван Иванович', 'P001', '15.05.1980', 3500);

INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT, BIRTHDAY, SALARY)
VALUES ('T006', 'Петров Петр Петрович', 'P002', '25.07.1985', 4000);

INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT, BIRTHDAY, SALARY)
VALUES ('T007', 'Сидоров Сидор Сидорович', 'P003', '11.10.1990', 4200);

INSERT INTO TEACHER (TEACHER, TEACHER_NAME, PULPIT, BIRTHDAY, SALARY)
VALUES ('T008', 'Лебедев Владимир Николаевич', 'P004', '22.08.1975', 4600);

commit;


DECLARE
  CURSOR c_avg_salary_pulpit IS
    SELECT P.PULPIT, P.FACULTY, F.FACULTY_NAME, P.PULPIT_NAME, AVG(T.SALARY) AS avg_salary 
    FROM TEACHER T
    JOIN PULPIT P ON T.PULPIT = P.PULPIT
    JOIN FACULTY F ON P.FACULTY = F.FACULTY
    GROUP BY P.PULPIT, P.FACULTY, F.FACULTY_NAME, P.PULPIT_NAME
    ORDER BY P.FACULTY;

  v_current_faculty VARCHAR2(50);
  v_current_faculty_name VARCHAR2(100);
  v_total_avg_salary_faculty FLOAT := 0;
  v_faculty_count NUMBER := 0;
  v_total_avg_salary_all_faculties FLOAT := 0;
  v_total_pulpit_count NUMBER := 0;
BEGIN
  DBMS_OUTPUT.PUT_LINE('Средняя зарплата по кафедрам:');
  
  FOR rec IN р LOOP
    IF v_current_faculty IS NOT NULL AND v_current_faculty != rec.FACULTY THEN
    
      DBMS_OUTPUT.PUT_LINE('Средняя зарплата по факультету ' || v_current_faculty || ' (' || v_current_faculty_name || '): ' || v_total_avg_salary_faculty / v_faculty_count); 
      
      v_total_avg_salary_faculty := 0;
      v_faculty_count := 0;
    END IF;
    
  
    DBMS_OUTPUT.PUT_LINE('Кафедра: ' || rec.PULPIT_NAME || ', Факультет: ' || rec.FACULTY_NAME || ', Средняя зарплата: ' || FLOOR(rec.avg_salary));

   
    v_total_avg_salary_faculty := v_total_avg_salary_faculty + rec.avg_salary;
    v_faculty_count := v_faculty_count + 1; 
    
    v_total_avg_salary_all_faculties := v_total_avg_salary_all_faculties + rec.avg_salary;
    v_total_pulpit_count := v_total_pulpit_count + 1;
    
    v_current_faculty := rec.FACULTY;
    v_current_faculty_name := rec.FACULTY_NAME;
  END LOOP;
  
  IF v_faculty_count > 0 THEN
    DBMS_OUTPUT.PUT_LINE('Средняя зарплата по факультету ' || v_current_faculty || ' (' || v_current_faculty_name || '): ' || v_total_avg_salary_faculty / v_faculty_count);
  END IF;

  DBMS_OUTPUT.PUT_LINE('Средняя зарплата по всем факультетам: ' || v_total_avg_salary_all_faculties / v_total_pulpit_count);
END;

--8


DECLARE
    TYPE teacher_type IS RECORD (
        teacher_id   VARCHAR2(10),
        teacher_name VARCHAR2(100),
        birthday     DATE
    );

    TYPE address_type IS RECORD (
        street  VARCHAR2(100),
        city    VARCHAR2(50),
        zip_code VARCHAR2(10)
    );

    TYPE teacher_with_address_type IS RECORD (
        teacher_info teacher_type,
        address_info address_type
    );

    v_teacher teacher_type;
    v_address address_type;
    v_teacher_with_address teacher_with_address_type;
BEGIN
    v_teacher.teacher_id := 'T005';
    v_teacher.teacher_name := 'Иванов Иван Иванович';
    v_teacher.birthday := TO_DATE('1980-05-15', 'YYYY-MM-DD');


    v_address.street := 'ул. Ленина, 10';
    v_address.city := 'Москва';
    v_address.zip_code := '123456';

    v_teacher_with_address.teacher_info := v_teacher;
    v_teacher_with_address.address_info := v_address;

    DBMS_OUTPUT.PUT_LINE('Teacher: ' || v_teacher_with_address.teacher_info.teacher_name || 
                         ' - ' || TO_CHAR(v_teacher_with_address.teacher_info.birthday, 'DD.MM.YYYY'));
    DBMS_OUTPUT.PUT_LINE('Address: ' || v_teacher_with_address.address_info.street || 
                         ', ' || v_teacher_with_address.address_info.city || 
                         ', ' || v_teacher_with_address.address_info.zip_code);
    
    DBMS_OUTPUT.PUT_LINE('Updated Teacher: ' || v_teacher_with_address.teacher_info.teacher_name);
END;

select * from teacher;

