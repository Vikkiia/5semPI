
CREATE TABLE FACULTY (
    faculty INT IDENTITY(1,1) PRIMARY KEY,
    faculty_name NVARCHAR(100) NOT NULL
);

CREATE TABLE PULPIT (
    pulpit INT IDENTITY(1,1) PRIMARY KEY,
    pulpit_name NVARCHAR(100) NOT NULL,
    faculty INT FOREIGN KEY REFERENCES FACULTY(faculty)
);

CREATE TABLE SUBJECT (
    subject INT IDENTITY(1,1) PRIMARY KEY,
    subject_name NVARCHAR(100) NOT NULL,
    pulpit INT FOREIGN KEY REFERENCES PULPIT(pulpit)
);

CREATE TABLE TEACHER (
    teacher INT IDENTITY(1,1) PRIMARY KEY,
    teacher_name NVARCHAR(100) NOT NULL,
    pulpit INT FOREIGN KEY REFERENCES PULPIT(pulpit)
);

INSERT INTO FACULTY (faculty_name) VALUES
('Факультет математики'),
('Факультет информатики');

INSERT INTO PULPIT (pulpit_name, faculty) VALUES 
('Кафедра информатики', 2), 
('Кафедра высшей математики', 1),
('Кафедра компьютерных наук', 2);

INSERT INTO TEACHER (teacher_name, pulpit) VALUES
('Иванов Иван Иванович', 1),
('Петрова Анна Сергеевна', 2);

INSERT INTO SUBJECT (subject_name, pulpit) VALUES
('Математический анализ', 1),
('Программирование', 2);

SELECT * FROM Pulpit;
SELECT * FROM Faculty;
SELECT * FROM Subject;
SELECT * FROM Teacher;
