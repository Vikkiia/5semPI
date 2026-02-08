alter session set container = CDB$ROOT;
//BVV_PDBConnect

//1
declare
    faculty_res FACULTY%rowtype;
begin
   
    select * into faculty_res  from FACULTY  where FACULTY = 'F001';
    dbms_output.put_line('Faculty Code: ' || faculty_res.FACULTY || ', Faculty Name: ' || faculty_res.FACULTY_NAME);
    end;
    
    
    --2
    
    select * from pulpit ;
    
    
    declare
    faculty_res PULPIT%rowtype;
begin
   
    select * into faculty_res from PULPIT where FACULTY = 'F002';

    dbms_output.put_line('Faculty Code: ' || faculty_res.FACULTY || ', Faculty Name: ' || faculty_res.FACULTY);
exception
    when others then
       
        dbms_output.put_line('Error: ' || sqlerrm || ', code: ' || sqlcode);
end;




--3



declare
    pulpit_res PULPIT%rowtype;
begin
    select * into pulpit_res from PULPIT;
    dbms_output.put_line(pulpit_res.PULPIT_NAME);
exception
    when TOO_MANY_ROWS then
        DBMS_OUTPUT.PUT_LINE(
            'результат состоит из нескольких строк (ORA' || SQLCODE || ')'
        );
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE(SQLERRM);
end;
    
    --4
    declare
    faculty_res FACULTY%rowtype;
begin

    select * into faculty_res from FACULTY where FACULTY = 'F999';
    dbms_output.put_line('Faculty Name: ' || faculty_res.FACULTY_NAME);
exception
    when NO_DATA_FOUND then
        dbms_output.put_line('Error: No data found ');
    when others then
        dbms_output.put_line('Error: ' || sqlerrm || ', code: ' || sqlcode);
end;


declare
    faculty_rec faculty%rowtype;
begin
    select * into faculty_rec from faculty where faculty = 'F001';
    dbms_output.put_line(rtrim(faculty_rec.faculty) || ': ' || faculty_rec.faculty_name);

    if sql%found then
        dbms_output.put_line('%found: true');
    else
        dbms_output.put_line('%found:false');
    end if;

    if sql%isopen then
        dbms_output.put_line('$isopen: true');
    else
        dbms_output.put_line('$isopen: false');
    end if;

    if sql%notfound then
        dbms_output.put_line('%notfound:true');
    else
        dbms_output.put_line('%notfound:false');
    end if;

    dbms_output.put_line('%rowcount:  ' || sql%rowcount);
end;


--5


begin
    update auditorium
    set auditorium = '206-1',
        auditorium_name = '206-1',
        auditorium_capacity = 38,
        auditorium_type = 'Computer Class'
    where auditorium = '210-1';
    --commit;
    --rollback;
exception
    when others
        then dbms_output.put_line(sqlcode || ' ' || sqlerrm);
end;

select * from auditorium;



--6
BEGIN
    UPDATE auditorium SET auditorium_capacity = 'ten' WHERE auditorium = '408-1';

EXCEPTION
    WHEN OTHERS THEN
     dbms_output.put_line('Error: ' || sqlerrm || ', code: ' || sqlcode);
  
END;

select * from auditorium;

--7

delete from auditorium where auditorium = '205-2';

BEGIN
   
    INSERT INTO auditorium (auditorium, auditorium_name, auditorium_capacity, auditorium_type)
    VALUES ('205-2', '205-2', 100, 'Seminar');
    COMMIT;  

    INSERT INTO auditorium (auditorium, auditorium_name, auditorium_capacity, auditorium_type)
    VALUES ('205-3', '205-3', 100, 'Seminar');
    --ROLLBACK;  

EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE(SQLCODE || ' ' || SQLERRM);
END;

select * from auditorium;

--8

BEGIN
   
    INSERT INTO auditorium (auditorium, auditorium_name, auditorium_capacity, auditorium_type) VALUES ('205-3', '205-3', 150, 'PZ'); 
   

EXCEPTION
    WHEN OTHERS THEN
       
        DBMS_OUTPUT.PUT_LINE(SQLCODE || ' ' || SQLERRM);  
         
END;

select * from auditorium;

--9

INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY, AUDITORIUM_TYPE)
VALUES
  ('227-1', '227-1', 15, 'Computer Class');
  
BEGIN
   
    DELETE FROM auditorium WHERE auditorium = '227-1';
    COMMIT; 

  
    DELETE FROM auditorium WHERE auditorium = '325-4';
    ROLLBACK; 

EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE(SQLCODE || ' ' || SQLERRM);
END;

select * from auditorium;


--10

BEGIN
    
    DELETE FROM auditorium_type WHERE auditorium_type = 'Lecture'; 

EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE(SQLCODE || ' ' || SQLERRM);
       
END;

select * from AUDITORIUM_TYPE;


--11
DECLARE
    
    CURSOR curs_teachers IS
        SELECT TEACHER, TEACHER_NAME, PULPIT
        FROM TEACHER;

    m_teacher      TEACHER.TEACHER%TYPE;
    m_teacher_name TEACHER.TEACHER_NAME%TYPE;
    m_pulpit       TEACHER.PULPIT%TYPE;
BEGIN

    OPEN curs_teachers;
    LOOP
        FETCH curs_teachers INTO m_teacher, m_teacher_name, m_pulpit;
        EXIT WHEN curs_teachers%NOTFOUND;

        DBMS_OUTPUT.PUT_LINE(' '
            || curs_teachers%ROWCOUNT || ' '  
            || m_teacher || ' '  
            || m_teacher_name || ' ' 
            || m_pulpit);  
    END LOOP;
    CLOSE curs_teachers;

EXCEPTION
    WHEN OTHERS THEN
        dbms_output.put_line(sqlerrm);
END;

select * from teacher;

--12

DECLARE
    CURSOR curs_subject IS
        SELECT subject, subject_name, pulpit
        FROM subject;
    rec_subject subject%ROWTYPE;
BEGIN

    OPEN curs_subject;
    FETCH curs_subject INTO rec_subject;

    WHILE curs_subject%FOUND 
    LOOP
        
        DBMS_OUTPUT.PUT_LINE(' '
            || curs_subject%ROWCOUNT || ' '  
            || rec_subject.subject || ' '  
            || rec_subject.subject_name || ' '
            || rec_subject.pulpit);

        FETCH curs_subject INTO rec_subject;
    END LOOP;
    CLOSE curs_subject;

EXCEPTION
    WHEN OTHERS THEN
        dbms_output.put_line(sqlerrm);
END;


--13
DECLARE
    CURSOR curs_pulpit IS
        SELECT pulpit.pulpit, teacher.teacher_name
        FROM pulpit
        JOIN teacher
            ON pulpit.pulpit = teacher.pulpit;

BEGIN
    FOR rec_pulpit IN curs_pulpit LOOP
        DBMS_OUTPUT.PUT_LINE(' '
            || rec_pulpit.pulpit || ' '  
            || rec_pulpit.teacher_name);  
    END LOOP;

EXCEPTION
    WHEN OTHERS THEN
        dbms_output.put_line(sqlerrm);
END;

select * from pulpit;
select * from teacher;


--14
INSERT INTO AUDITORIUM (AUDITORIUM, AUDITORIUM_NAME, AUDITORIUM_CAPACITY, AUDITORIUM_TYPE)
VALUES
  ('444-1', '444-1', 70, 'Computer Class');
  
 
  
DECLARE
 
    CURSOR curs (from_cap auditorium.auditorium_capacity%type, to_cap auditorium.auditorium_capacity%type)
        IS 
            SELECT auditorium, auditorium_capacity, auditorium_type
            FROM auditorium
            WHERE auditorium_capacity >= from_cap
              AND auditorium_capacity <= to_cap;

    record curs%ROWTYPE;
BEGIN
   --1
    DBMS_OUTPUT.PUT_LINE('capacity < 20 :');
    FOR aum IN curs(0, 20) LOOP
        DBMS_OUTPUT.PUT_LINE(aum.auditorium || ' ' || aum.auditorium_capacity);
    END LOOP;

   --2
    DBMS_OUTPUT.PUT_LINE('21 < capacity < 30 :');
    OPEN curs(21, 30);  
    FETCH curs INTO record;  
    WHILE curs%FOUND LOOP
        DBMS_OUTPUT.PUT_LINE(record.auditorium || ' ' || record.auditorium_capacity);
        FETCH curs INTO record;  
    END LOOP;
    CLOSE curs;  

    DBMS_OUTPUT.PUT_LINE('31 < capacity < 60 :');
    FOR aum IN curs(31, 60) LOOP
        DBMS_OUTPUT.PUT_LINE(aum.auditorium || ' ' || aum.auditorium_capacity);
    END LOOP;

    --3
    DBMS_OUTPUT.PUT_LINE('61 < capacity < 80 :');
OPEN curs(61, 80);

LOOP
    FETCH curs INTO record;
    EXIT WHEN curs%NOTFOUND; 
    DBMS_OUTPUT.PUT_LINE(record.auditorium || ' ' || record.auditorium_capacity);
END LOOP;

CLOSE curs;

EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE(SQLCODE || ' ' || SQLERRM);
END;

select * from auditorium;




--15
DECLARE
    xcurs        SYS_REFCURSOR;              
    rec_aud      AUDITORIUM%ROWTYPE;       

    v_from_cap   AUDITORIUM.AUDITORIUM_CAPACITY%TYPE := 0;   
    v_to_cap     AUDITORIUM.AUDITORIUM_CAPACITY%TYPE := 80;  
BEGIN
    
    OPEN xcurs FOR
        SELECT *
        FROM AUDITORIUM
        WHERE AUDITORIUM_CAPACITY BETWEEN v_from_cap AND v_to_cap;

   
    FETCH xcurs INTO rec_aud;

    WHILE xcurs%FOUND
    LOOP
        DBMS_OUTPUT.PUT_LINE(
              'Аудитория: ' || rec_aud.AUDITORIUM
           || ', вместимость: ' || rec_aud.AUDITORIUM_CAPACITY
           || ', тип: ' || rec_aud.AUDITORIUM_TYPE
        );

        FETCH xcurs INTO rec_aud;  
    END LOOP;

    CLOSE xcurs;

EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE(SQLCODE || ' ' || SQLERRM);
END;


--16

DECLARE
    CURSOR curs_aut IS
        SELECT auditorium_type,
               CURSOR (
                   SELECT auditorium
                   FROM auditorium
                   WHERE auditorium_type = auditorium_type.auditorium_type
               ) AS aum_cursor
        FROM auditorium_type;

    curs_aum SYS_REFCURSOR;

    aut auditorium_type.auditorium_type%TYPE;
    txt VARCHAR2(1000);
    aum auditorium.auditorium%TYPE;
BEGIN
    
    OPEN curs_aut;

    FETCH curs_aut INTO aut, curs_aum;

    WHILE curs_aut%FOUND LOOP
        txt := RTRIM(aut) || ': ';

        LOOP
            FETCH curs_aum INTO aum;
            EXIT WHEN curs_aum%NOTFOUND;
            txt := txt || RTRIM(aum) || '; ';
        END LOOP;

        DBMS_OUTPUT.PUT_LINE(txt);

        FETCH curs_aut INTO aut, curs_aum;
    END LOOP;

    CLOSE curs_aut;
     

EXCEPTION
    WHEN OTHERS THEN
         dbms_output.put_line(sqlerrm);
END;



--17

DECLARE
    CURSOR curs_auditorium(
        from_cap auditorium.auditorium_capacity%TYPE,
        to_cap   auditorium.auditorium_capacity%TYPE
    ) IS
        SELECT auditorium, auditorium_capacity
        FROM auditorium
        WHERE auditorium_capacity BETWEEN from_cap AND to_cap
        FOR UPDATE;  
BEGIN
   
    FOR rec IN curs_auditorium(40, 80) LOOP
        
        DECLARE
            v_new_capacity auditorium.auditorium_capacity%TYPE;
        BEGIN
            v_new_capacity := rec.auditorium_capacity * 0.9;

            UPDATE auditorium
            SET auditorium_capacity = v_new_capacity
            WHERE CURRENT OF curs_auditorium;

            DBMS_OUTPUT.PUT_LINE(
                'Auditorium: ' || rec.auditorium ||
                ' New Capacity: ' || v_new_capacity
            );
        END;
    END LOOP;

EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE(SQLCODE || ' ' || SQLERRM);
END;


 ROLLBACK;  
 
select * from auditorium;

--18

DECLARE
    CURSOR curs_auditorium(from_cap auditorium.auditorium_capacity%TYPE, to_cap auditorium.auditorium_capacity%TYPE)
        IS
        SELECT auditorium, auditorium_capacity
        FROM auditorium
        WHERE auditorium_capacity >= from_cap
          AND auditorium_capacity <= to_cap
        FOR UPDATE;  

    aum auditorium.auditorium%TYPE;
    cty auditorium.auditorium_capacity%TYPE;
BEGIN
    OPEN curs_auditorium(0, 20);

    FETCH curs_auditorium INTO aum, cty;

    WHILE curs_auditorium%FOUND LOOP
        DELETE FROM auditorium
        WHERE CURRENT OF curs_auditorium;

        FETCH curs_auditorium INTO aum, cty;
    END LOOP;

    CLOSE curs_auditorium;

EXCEPTION
    WHEN OTHERS THEN
        dbms_output.put_line(sqlerrm);
END;

 ROLLBACK;
select * from auditorium;


--19
select * from auditorium where auditorium_capacity between 20 and 150 order by auditorium;


DECLARE
    CURSOR cur
        IS
        SELECT auditorium, auditorium_capacity, rowid
        FROM auditorium
        FOR UPDATE; 

    aum auditorium.auditorium%type;
    cap auditorium.auditorium_capacity%type;
BEGIN
 
    FOR rec IN cur LOOP
        IF rec.auditorium_capacity >= 90 THEN
            DELETE FROM auditorium
            WHERE rowid = rec.rowid;
        
        ELSIF rec.auditorium_capacity >= 20 THEN
            UPDATE auditorium
            SET auditorium_capacity = auditorium_capacity + 3
            WHERE rowid = rec.rowid;
        END IF;
    END LOOP;

    FOR rec IN cur LOOP
        DBMS_OUTPUT.PUT_LINE(rec.auditorium || ' ' || rec.auditorium_capacity);
    END LOOP;

EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('Ошибка: ' || SQLERRM);
END;


    ROLLBACK;
select * from auditorium;


--20

DECLARE
   
    CURSOR curs_teachers IS 
        SELECT teacher, teacher_name, pulpit
        FROM teacher;

    m_teacher      teacher.teacher%type;
    m_teacher_name teacher.teacher_name%type;
    m_pulpit       teacher.pulpit%type;
BEGIN
    OPEN curs_teachers;

    LOOP

        FETCH curs_teachers INTO m_teacher, m_teacher_name, m_pulpit;

        EXIT WHEN curs_teachers%NOTFOUND;

        DBMS_OUTPUT.PUT_LINE(' ' || curs_teachers%ROWCOUNT || ' ' || m_teacher || ' ' || m_teacher_name || ' ' || m_pulpit);

        IF (MOD(curs_teachers%ROWCOUNT, 3) = 0) THEN
            DBMS_OUTPUT.PUT_LINE('-----------------------');
        END IF;
    END LOOP;
    CLOSE curs_teachers;

EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE(SQLCODE || ' ' || SQLERRM);
END;



   
   
   
    
    