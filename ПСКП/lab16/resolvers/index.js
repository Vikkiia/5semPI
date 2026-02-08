async function getRecordsByField(object, field, context) {
    const fields = {};
    fields[object] = field;
    let records = [];
    if (field) {
        records = await context.getOne(object, fields);
    } else {
        records = await context.getAll(object);
    }
    return records.map(record => {
        if (object === 'FACULTY') {
            return {
                FACULTY: record.faculty.toString(),
                FACULTY_NAME: record.faculty_name
            };
        } else if (object === 'PULPIT') {
            return {
                PULPIT: record.pulpit.toString(),
                PULPIT_NAME: record.pulpit_name,
                FACULTY: record.faculty.toString()
            };
        } else if (object === 'SUBJECT') {
            return {
                SUBJECT: record.subject.toString(),
                SUBJECT_NAME: record.subject_name,
                PULPIT: record.pulpit.toString()
            };
        } else if (object === 'TEACHER') {
            return {
                TEACHER: record.teacher.toString(),
                TEACHER_NAME: record.teacher_name,
                PULPIT: record.pulpit.toString()
            };
        }
        return record; 
    });
}

async function mutateRecord(object, idField, fields, context) {
    const { [idField]: id, ...updateFields } = fields;
    const checkQuery = `SELECT ${idField} FROM ${object} WHERE ${idField} = '${id}'`;
    const existingRecord = await context.query(checkQuery);
    let result;
    if (existingRecord.length > 0) {
        const updateQuery = `
            UPDATE ${object}
            SET ${Object.keys(updateFields).map(key => `${key} = '${updateFields[key]}'`).join(', ')}
            OUTPUT INSERTED.${idField}, INSERTED.${Object.keys(updateFields).join(', INSERTED.')}
            WHERE ${idField} = '${id}'
        `;
        result = await context.query(updateQuery);
    } else {
        const { [idField]: _, ...insertFields } = fields; 
        const insertQuery = `
            INSERT INTO ${object} (${Object.keys(insertFields).join(', ')})
            OUTPUT INSERTED.${idField}, INSERTED.${Object.keys(insertFields).join(', INSERTED.')}
            VALUES (${Object.values(insertFields).map(value => `'${value}'`).join(', ')})
        `;
        result = await context.query(insertQuery);
    }
    if (result.length > 0) {
        const record = result[0];
        return {
            [idField.toUpperCase()]: record[idField],  
            ...Object.keys(updateFields).reduce((acc, key) => {
                acc[key.toUpperCase()] = record[key];
                return acc;
            }, {})
        };
    } else {
        return null;
    }
}

async function deleteRecord(object, id, context) {
    let recordIdObject = {};
    recordIdObject[object] = id;
    const targetRecord = await context.getOne(object, recordIdObject);
    if (!targetRecord || targetRecord.length === 0) {
        return false;
    }
    await context.deleteOne(object, id);
    return true;
}

module.exports =
    {
        getFaculties: (args, context) => getRecordsByField('FACULTY', args.FACULTY, context),
        getPulpits: (args, context) => getRecordsByField('PULPIT', args.PULPIT, context),
        getSubjects:  (args, context) => getRecordsByField('SUBJECT', args.SUBJECT, context),
        getTeachers: (args, context) => getRecordsByField('TEACHER', args.TEACHER, context),
        getSubjectsByFaculties:async (args, context) =>{
            const {SUBJECT, FACULTY} = args;
            console.log(FACULTY);
            return FACULTY ?
                await context.query(
                    `SELECT SUBJECT.SUBJECT,SUBJECT.SUBJECT_NAME,SUBJECT.PULPIT FROM SUBJECT join PULPIT on SUBJECT.PULPIT = PULPIT.PULPIT join FACULTY on PULPIT.FACULTY = FACULTY.FACULTY where FACULTY.FACULTY ='${FACULTY}';`
                ) : await getRecordsByField('SUBJECT', SUBJECT, context);
        },
        getTeachersByFaculty:async (args, context) => {
            const {TEACHER, FACULTY} = args;
            console.log(FACULTY);
            return FACULTY ?
                await context.query(
                    `SELECT TEACHER.TEACHER, TEACHER.TEACHER_NAME, TEACHER.PULPIT FROM TEACHER join PULPIT on TEACHER.PULPIT = PULPIT.PULPIT join FACULTY on PULPIT.FACULTY = FACULTY.FACULTY where FACULTY.FACULTY ='${FACULTY}';`
                ) : await getRecordsByField('TEACHER', TEACHER, context);
        },
        setFaculty: async (args, context) => {
            const fields = {
                FACULTY: args.FACULTY,
                FACULTY_NAME: args.FACULTY_NAME
            };
            return await mutateRecord('FACULTY', 'FACULTY', fields, context);
        },
        setPulpit: async (args, context) => {
            const fields = {
                PULPIT: args.PULPIT,
                PULPIT_NAME: args.PULPIT_NAME,
                FACULTY: args.FACULTY
            };
            return await mutateRecord('PULPIT', 'PULPIT', fields, context);
        },
        setSubject: async (args, context) => {
            const fields = {
                SUBJECT: args.SUBJECT,
                SUBJECT_NAME: args.SUBJECT_NAME,
                PULPIT: args.PULPIT
            };
            return await mutateRecord('SUBJECT', 'SUBJECT', fields, context);
        },
        setTeacher: async (args, context) => {
            const fields = {
                TEACHER: args.TEACHER,
                TEACHER_NAME: args.TEACHER_NAME,
                PULPIT: args.PULPIT
            };
            return await mutateRecord('TEACHER', 'TEACHER', fields, context);
        },
        delFaculty: (args, context) => deleteRecord('FACULTY', args.FACULTY, context),
        delPulpit: (args, context) => deleteRecord('PULPIT', args.PULPIT, context),
        delSubject: (args, context) => deleteRecord('SUBJECT', args.SUBJECT, context),
        delTeacher: (args, context) => deleteRecord('TEACHER', args.TEACHER, context)
    };