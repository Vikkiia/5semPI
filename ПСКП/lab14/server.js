const express = require('express');
const { Pool } = require('pg');
const bodyParser = require('body-parser');
const path = require('path');
const app = express();


const port = 3000;

const pool = new Pool({
    user: 'univer_admin',
    host: 'localhost',
    database: 'BVV',
    password: '1111',
    port: 5432,
});
async function connectDB() {
    try {
        const client = await pool.connect();
        console.log('Connected to PostgreSQL');
        client.release();
    } catch (err) {
        console.error('Database connection error:', err);
    }
}
connectDB();

app.use(bodyParser.json());

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'index.html'));
});




app.get('/api/faculties', async (req, res) => {
    try {
        const result = await pool.query('SELECT * FROM faculty ORDER BY faculty');
        res.json(result.rows);
    } catch (error) {
        console.error(error);
        res.status(500).json({ error: 'Ошибка получения факультетов' });
    }
});

app.get('/api/pulpits', async (req, res) => {
    try {
        const result = await pool.query('SELECT * FROM pulpit ORDER BY pulpit');
        if (result.rows.length === 0) {
            return res.status(404).json({ error: 'Кафедры не найдены' });
        }
        res.json(result.rows);
    } catch (error) {
        console.error(error);
        res.status(500).json({ error: 'Ошибка получения кафедр' });
    }
});

app.get('/api/subjects', async (req, res) => {
    try {
        const result = await pool.query('SELECT * FROM subject ORDER BY subject');
        res.json(result.rows);
    } catch (error) {
        console.error(error);
        res.status(500).json({ error: 'Ошибка получения учебных дисциплин' });
    }
});

app.get('/api/auditoriumstypes', async (req, res) => {
    try {
        const result = await pool.query('SELECT * FROM auditorium_type ORDER BY auditorium_type');
        res.json(result.rows);
    } catch (error) {
        console.error(error);
        res.status(500).json({ error: 'Ошибка получения типов аудиторий' });
    }
});

app.get('/api/auditoriums', async (req, res) => {
    try {
        const result = await pool.query('SELECT * FROM auditorium ORDER BY auditorium');
        res.json(result.rows);
    } catch (error) {
        console.error(error);
        res.status(500).json({ error: 'Ошибка получения аудиторий' });
    }
});



app.post('/api/faculties', async (req, res) => {
    const { faculty, faculty_name } = req.body;
    console.log('Received data:', { faculty, faculty_name });

    try {
        const result = await pool.query(
            'INSERT INTO faculty (faculty, faculty_name) VALUES ($1, $2) RETURNING *',
            [faculty, faculty_name]
        );
        res.json(result.rows[0]);
    } catch (error) {
        console.error('Ошибка при добавлении факультета:', error);
        res.status(500).json({ error: 'Ошибка добавления факультета' });
    }
});

app.post('/api/pulpits', async (req, res) => {
    const { pulpit, pulpit_name, faculty } = req.body;
    console.log('Received data:', { pulpit, pulpit_name, faculty });

    try {
        const result = await pool.query(
            'INSERT INTO pulpit (pulpit, pulpit_name, faculty) VALUES ($1, $2, $3) RETURNING *',
            [pulpit, pulpit_name, faculty]
        );
        res.json(result.rows[0]);
    } catch (error) {
        console.error('Error during INSERT pulpit:', error);
        res.status(500).json({ error: 'Ошибка добавления кафедры' });
    }
});

app.post('/api/subjects', async (req, res) => {
    const { subject, subject_name, pulpit } = req.body;
    console.log('Received data:', { subject, subject_name, pulpit });

    try {
        const result = await pool.query(
            'INSERT INTO subject (subject, subject_name, pulpit) VALUES ($1, $2, $3) RETURNING *',
            [subject, subject_name, pulpit]
        );
        res.json(result.rows[0]);
    } catch (error) {
        console.error('Ошибка при добавлении дисциплины:', error);
        res.status(500).json({ error: 'Ошибка добавления дисциплины' });
    }
});

app.post('/api/auditoriumstypes', async (req, res) => {
    const { auditorium_type, auditorium_typename } = req.body;
    console.log('Received data:', { auditorium_type, auditorium_typename });

    try {
        const result = await pool.query(
            'INSERT INTO auditorium_type (auditorium_type, auditorium_typename) VALUES ($1, $2) RETURNING *',
            [auditorium_type, auditorium_typename]
        );
        res.json(result.rows[0]);
    } catch (error) {
        console.error('Ошибка при добавлении типа аудитории:', error);
        res.status(500).json({ error: 'Ошибка добавления типа аудитории' });
    }
});

app.post('/api/auditoriums', async (req, res) => {
    const { auditorium, auditorium_name, auditorium_capacity, auditorium_type } = req.body;
    console.log('Received data:', { auditorium, auditorium_name, auditorium_capacity, auditorium_type });

    try {
        const result = await pool.query(
            `INSERT INTO auditorium (auditorium, auditorium_name, auditorium_capacity, auditorium_type)
             VALUES ($1, $2, $3, $4)
             RETURNING *`,
            [auditorium, auditorium_name, auditorium_capacity, auditorium_type]
        );
        res.json(result.rows[0]);
    } catch (error) {
        console.error('Ошибка при добавлении аудитории:', error);
        res.status(500).json({ error: 'Ошибка добавления аудитории' });
    }
});



app.put('/api/faculties', async (req, res) => {
    const { faculty, faculty_name } = req.body;

    try {
        const result = await pool.query(
            'UPDATE faculty SET faculty_name = $2 WHERE faculty = $1 RETURNING *',
            [faculty, faculty_name]
        );

        if (result.rowCount === 0) {
            return res.status(404).json({ error: 'Факультет не найден' });
        }

        res.json(result.rows[0]);
    } catch (error) {
        console.error('Ошибка при обновлении факультета:', error);
        res.status(500).json({ error: 'Ошибка обновления факультета' });
    }
});

app.put('/api/pulpits', async (req, res) => {
    const { pulpit, pulpit_name, faculty } = req.body;

    try {
        const result = await pool.query(
            'UPDATE pulpit SET pulpit_name = $2, faculty = $3 WHERE pulpit = $1 RETURNING *',
            [pulpit, pulpit_name, faculty]
        );

        if (result.rowCount === 0) {
            return res.status(404).json({ error: 'Кафедра не найдена' });
        }

        res.json(result.rows[0]);
    } catch (error) {
        console.error('Ошибка обновления кафедры:', error);
        res.status(500).json({ error: 'Ошибка обновления кафедры' });
    }
});

app.put('/api/subjects', async (req, res) => {
    const { subject, subject_name, pulpit } = req.body;

    try {
        const result = await pool.query(
            'UPDATE subject SET subject_name = $2, pulpit = $3 WHERE subject = $1 RETURNING *',
            [subject, subject_name, pulpit]
        );

        if (result.rowCount === 0) {
            return res.status(404).json({ error: 'Дисциплина не найдена' });
        }

        res.json(result.rows[0]);
    } catch (error) {
        console.error('Ошибка при обновлении дисциплины:', error);
        res.status(500).json({ error: 'Ошибка обновления дисциплины' });
    }
});

app.put('/api/auditoriumstypes', async (req, res) => {
    const { auditorium_type, auditorium_typename } = req.body;

    try {
        const result = await pool.query(
            'UPDATE auditorium_type SET auditorium_typename = $2 WHERE auditorium_type = $1 RETURNING *',
            [auditorium_type, auditorium_typename]
        );

        if (result.rowCount === 0) {
            return res.status(404).json({ error: 'Тип аудитории не найден' });
        }

        res.json(result.rows[0]);
    } catch (error) {
        console.error('Ошибка при обновлении типа аудитории:', error);
        res.status(500).json({ error: 'Ошибка обновления типа аудитории' });
    }
});


app.put('/auditoriums', async (req, res) => {
    const { auditorium, auditorium_name, auditorium_capacity, auditorium_type } = req.body;

    try {
        const checkResult = await pool.query(
            'SELECT * FROM auditorium WHERE auditorium = $1',
            [auditorium]
        );

        if (checkResult.rowCount === 0) {
            return res.status(404).json({ error: 'Аудитория не найдена' });
        }

        const updateResult = await pool.query(
            `UPDATE auditorium
             SET auditorium_name = $2, auditorium_capacity = $3, auditorium_type = $4
             WHERE auditorium = $1
             RETURNING *`,
            [auditorium, auditorium_name, auditorium_capacity, auditorium_type]
        );

        res.json(updateResult.rows[0]);
    } catch (error) {
        console.error('Ошибка при обновлении аудитории:', error);
        res.status(500).json({ error: 'Ошибка обновления аудитории' });
    }
});



app.delete('/api/faculties/:id', async (req, res) => {
    const id = req.params.id;

    try {
        const result = await pool.query(
            'DELETE FROM faculty WHERE faculty = $1',
            [id]
        );

        if (result.rowCount === 0) {
            return res.status(404).json({ error: 'Факультет не найден' });
        }

        res.json({ message: 'Факультет удалён' });
    } catch (error) {
        console.error('Ошибка удаления факультета:', error);
        res.status(500).json({ error: 'Ошибка удаления факультета' });
    }
});

app.delete('/api/pulpits/:id', async (req, res) => {
    const pulpit = req.params.id;

    try {
        const result = await pool.query(
            'DELETE FROM pulpit WHERE pulpit = $1',
            [pulpit]
        );

        if (result.rowCount === 0) {
            return res.status(404).json({ error: 'Кафедра не найдена' });
        }

        res.json({ message: 'Кафедра успешно удалена' });
    } catch (error) {
        console.error('Ошибка при удалении кафедры:', error);
        res.status(500).json({ error: 'Ошибка удаления кафедры' });
    }
});

app.delete('/api/subjects/:id', async (req, res) => {
    const subject = req.params.id;

    try {
        const result = await pool.query(
            'DELETE FROM subject WHERE subject = $1',
            [subject]
        );

        if (result.rowCount === 0) {
            return res.status(404).json({ error: 'Дисциплина не найдена' });
        }

        res.json({ message: 'Дисциплина успешно удалена' });
    } catch (error) {
        console.error('Ошибка при удалении дисциплины:', error);
        res.status(500).json({ error: 'Ошибка удаления дисциплины' });
    }
});


app.delete('/api/auditoriumstypes/:id', async (req, res) => {
    const auditoriumTypeId = req.params.id;

    try {
        const selectResult = await pool.query(
            'SELECT * FROM auditorium_type WHERE auditorium_type = $1',
            [auditoriumTypeId]
        );

        if (selectResult.rowCount === 0) {
            return res.status(404).json({ error: 'Тип аудитории не найден' });
        }

        await pool.query(
            'DELETE FROM auditorium WHERE auditorium_type = $1',
            [auditoriumTypeId]
        );

        await pool.query(
            'DELETE FROM auditorium_type WHERE auditorium_type = $1',
            [auditoriumTypeId]
        );

        res.json({ message: 'Тип аудитории и её аудитории успешно удалены' });
    } catch (error) {
        console.error('Ошибка при удалении типа аудитории:', error);
        res.status(500).json({ error: 'Ошибка удаления типа аудитории' });
    }
});

app.delete('/api/auditoriums/:id', async (req, res) => {
    const auditorium = req.params.id;

    try {
        const result = await pool.query(
            'DELETE FROM auditorium WHERE auditorium = $1',
            [auditorium]
        );

        if (result.rowCount === 0) {
            return res.status(404).json({ error: 'Аудитория не найдена' });
        }

        res.json({ message: 'Аудитория успешно удалена' });
    } catch (error) {
        console.error('Ошибка при удалении аудитории:', error);
        res.status(500).json({ error: 'Ошибка удаления аудитории' });
    }
});

app.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
});
