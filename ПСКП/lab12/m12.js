const fs = require('fs');
const url = require('url');





function FilesModule(serverWS) {
    this.serverWS = serverWS;
    const filePath = 'StudentList.json';
    const backupPath = 'backup';
    let studentList = JSON.parse(fs.readFileSync(filePath, "utf8"));



    //  GET:/
    this.GetAllStudents = (req, res) => {
        res.writeHead(200, { 'Content-Type': 'application/json; charset=utf-8' });
        res.end(JSON.stringify(studentList, null, 4));
    }

    //  GET: /n
    this.GetStudentById = (req, res) => {
        let isFoundStudentById = false;
        let id = url.parse(req.url).pathname.split('/')[1];

        studentList.forEach(student => {
            if (student.id === +id) {
                isFoundStudentById = true;
                res.writeHead(200, { 'Content-Type': 'application/json; charset=utf-8' });
                res.end(JSON.stringify(student, null, 4));
            }
        })

        if (!isFoundStudentById) {
            this.ErrorJsonResponse(res, 1, `There is no student with ID = ${+id}.`)
        }
    }

    //  POST: /
    this.InsertStudent = (req, res) => {
        let isExistsStudentWithId = false;
        let body = '';

        req.on('data', chunk => {
            body += chunk;
        })

        req.on('end', () => {
            studentList.forEach(student => {
                if (student.id === +JSON.parse(body).id) {
                    isExistsStudentWithId = true;
                    this.ErrorJsonResponse(res, 2, `There is already a student with ID = ${student.id}.`);
                }
            });

            if (!isExistsStudentWithId) {
                studentList.push(JSON.parse(body));
                fs.writeFile(filePath, JSON.stringify(studentList, null, 4), error => {
                    if (error) {
                        this.ErrorJsonResponse(res, 10, `Error during writing file ${filePath}.`);
                    }
                    else {
                        res.writeHead(200, { 'Content-Type': 'application/json; charset=utf-8' });
                        res.end(JSON.stringify(JSON.parse(body), null, 4));
                    }
                });
            }
        })
    }

    //  PUT: /
    this.UpdateStudent = (req, res) => {
        let isFoundStudentWithId = false;
        let body = '';

        req.on('data', chunk => {
            body += chunk;
        })

        req.on('end', () => {
            studentList.forEach(student => {
                if (student.id === +JSON.parse(body).id) {
                    isFoundStudentWithId = true;
                    student.name = JSON.parse(body).name;
                    student.bday = JSON.parse(body).bday;
                    student.speciality = JSON.parse(body).speciality;
                    fs.writeFile(filePath, JSON.stringify(studentList, null, 4), error => {
                        if (error) {
                            this.ErrorJsonResponse(res, 10, `Error during writing file ${filePath}.`);
                        }
                        else {
                            res.writeHead(200, { 'Content-Type': 'application/json; charset=utf-8' });
                            res.end(JSON.stringify(JSON.parse(body), null, 4));
                        }
                    });

                }
            });

            if (!isFoundStudentWithId) {
                this.ErrorJsonResponse(res, 3, `There is no student with ID = ${+JSON.parse(body).id}.`)
            }
        })
    }

    //  DELETE:  /n
    this.DeleteStudentById = (req, res) => {
        const id = parseInt(url.parse(req.url).pathname.split('/')[1], 10);
        if (isNaN(id)) {
            this.ErrorJsonResponse(res, 1, 'Invalid student ID.');
            return;
        }
        const studentIndex = studentList.findIndex(student => student.id === id);
        if (studentIndex === -1) {
            this.ErrorJsonResponse(res, 4, `No student found with ID = ${id}.`);
            return;
        }
        const removedStudent = studentList.splice(studentIndex, 1)[0];
        fs.writeFile(filePath, JSON.stringify(studentList, null, 4), 'utf8', writeError => {
            if (writeError) {
                this.ErrorJsonResponse(res, 5, `Error writing to file ${filePath}.`);
                return;
            }
            res.writeHead(200, { 'Content-Type': 'application/json; charset=utf-8' });
            res.end(JSON.stringify(removedStudent, null, 4));
        });
    };

    //  POST    /backup
    this.BackupStudentList = (req, res) => {
        let dateNow = new Date();
        let dateISOSplitted = new Date().toISOString().split('-');
        let dateFormat = dateISOSplitted[0] + dateISOSplitted[1] + (dateISOSplitted[2])[0] + (dateISOSplitted[2])[1] + dateNow.getHours() + dateNow.getSeconds();
        let backupFilePath = `./${backupPath}/${dateFormat}_${filePath}`;

        setTimeout(() => {
            fs.copyFile(filePath, backupFilePath, error => {
                if (error) {
                    this.ErrorJsonResponse(res, 11, `Error during backing up file ${backupFilePath}.`);
                }
                else {
                    console.log('Successfully backed up!\n');
                    this.statusJson(res, 200, "Successfully backed up!");
                }
            })
        }, 2000);
    }

    // DELETE   /backup/YYYYDDMM
    this.DeleteOldBackups = (req, res) => {
        let dateParam = url.parse(req.url).pathname.split('/')[2];

        fs.readdir(backupPath, { withFileTypes: false }, (error, files) => {
            if (error) {
                return this.ErrorJsonResponse(res, 12, `Cannot access ${backupPath} backup directory.`);
            }
            if (files.length === 0) {
                return this.statusJson(res, 201, `There are no files in ${backupPath} directory.`);
            }

            console.log(`Requested date: ${dateParam}`);
            console.log(`Files in backup:`, files);

            let deleted = false;

            files.forEach(file => {
                let rawDate = file.substring(0, 8);
                let formattedDate = rawDate.substring(0, 4) + rawDate.substring(6, 8) + rawDate.substring(4, 6);


                console.log(`Checking file: ${file}, extracted date: ${formattedDate}`);

                if (+formattedDate < +dateParam) {
                    console.log(`Deleting file: ${file}`);
                    fs.unlink(`${backupPath}/${file}`, err => {
                        if (err) {
                            console.error(`Error deleting file ${file}:`, err);
                            return this.ErrorJsonResponse(res, 13, `Cannot delete file ${file}.`);
                        }
                        console.log(`Deleted file: ${file}`);
                    });
                    deleted = true;
                }
            });

            if (deleted) {
                this.statusJson(res, 200, "Successfully deleted old backups!");
            } else {
                this.statusJson(res, 202, `There are no backups older than ${dateParam}.`);
            }
        });
    };

    // GET /backup
    this.GetAllBackups = (req, res) => {
        let backupsList = [];

        fs.readdir(backupPath, { withFileTypes: false }, (error, files) => {
            if (error) {
                this.ErrorJsonResponse(res, 14, `Cannot access ${backupPath} backup directory.`);
            }
            else {
                console.log(`There are ${files.length} files in ${backupPath} directory:`);
                if (files.length === 0) {
                    this.statusJson(res, 202, `There are ${files.length} files in ${backupPath} directory.`);
                }
                else {
                    for (let i = 0; i < files.length; ++i) {
                        console.log(i + '. ' + files[i]);
                        backupsList.push({ id: i, name: files[i] });
                    }
                    res.writeHead(200, { 'Content-Type': 'application/json; charset=utf-8' });
                    res.end(JSON.stringify(backupsList, null, 4));
                }
            }
        })
    }


    this.statusJson = (res, statusCode, statusMessage) => {
        res.writeHead(+statusCode, { 'Content-Type': 'application/json; charset=utf-8' });
        res.end(JSON.stringify({ status: statusMessage }, null, 4));
    }

    this.ErrorJsonResponse = (res, errorCode, errorMessage) => {
        res.writeHead(408, errorMessage, { 'Content-Type': 'application/json; charset=utf-8' });
        res.end(JSON.stringify({ errorCode: errorCode, errorMessage: errorMessage }, null, 4));
    }

    this.ErrorIncorrectMethod = res => {
        res.writeHead(409, 'Incorrect method', { 'Content-Type': 'application/json; charset=utf-8' });
        res.end(JSON.stringify({ errorCode: 409, errorMessage: "Incorrect method." }, null, 4));
    }

    this.ErrorIncorrectURL = res => {
        res.writeHead(410, 'Incorrect URL', { 'Content-Type': 'application/json; charset=utf-8' });
        res.end(JSON.stringify({ errorCode: 410, errorMessage: "Incorrect URL." }, null, 4));
    }



    // serverWS.event

    // serverWS.on('connection', ws => {
    //     console.log('Connected.');

    //     fs.watch(filePath, (eventType, filename) => {
    //         if (eventType === 'change') {
    //             serverWS.clients.forEach(client => {
    //                 if (client.readyState === ws.OPEN) {
    //                     client.send(`${filename} was modified.`);
    //                 }
    //             })
    //         }
    //     });
    //     fs.readdir(backupPath, (error, files) => {
    //         if (error)
    //             this.ErrorJsonResponse(res, 15, `Cannot access directory ${backupPath}.`);
    //         else {
    //             for (let i = 0; i < files.length; i++) {
    //                 fs.watch((backupPath + '/' + files[i]), (eventType, filename) => {
    //                     if (eventType === 'change') {
    //                         serverWS.clients.forEach((client) => {
    //                             if (client.readyState === ws.OPEN) {
    //                                 client.send(`${backupPath}/${filename} was modified.`);
    //                             }
    //                         })
    //                     }
    //                 })
    //             }
    //         }
    //     })
    // })

    serverWS.on('connection', ws => {
        console.log('Connected.');

        let lastModifiedTime = null;//время последнего изменения отслеживаемого
        let changeInProgress = false; // Флаг для предотвращения повторных изменений


        fs.watch(filePath, (eventType, filename) => {
            if (eventType === 'change' && !changeInProgress) {

                changeInProgress = true;


                let currentModifiedTime = fs.statSync(filePath).mtime;


                setTimeout(() => {

                    if (currentModifiedTime.getTime() !== lastModifiedTime?.getTime()) {
                        serverWS.clients.forEach(client => {
                            if (client.readyState === ws.OPEN) {
                                client.send(`${filename} was modified.`);
                            }
                        });


                        lastModifiedTime = currentModifiedTime;
                    }

                    changeInProgress = false;
                }, 200);
            }
        });

        // Для отслеживания изменений резервных копий в папке
        fs.readdir(backupPath, (error, files) => {
            if (error) {
                this.ErrorJsonResponse(res, 15, `Cannot access directory ${backupPath}.`);
            } else {
                files.forEach(file => {
                    let filePathToWatch = `${backupPath}/${file}`;
                    let lastBackupModifiedTime = null;
                    let backupChangeInProgress = false;

                    fs.watch(filePathToWatch, (eventType, filename) => {
                        if (eventType === 'change' && !backupChangeInProgress) {

                            backupChangeInProgress = true;


                            let currentModifiedTime = fs.statSync(filePathToWatch).mtime;
                            setTimeout(() => {
                                if (currentModifiedTime.getTime() !== lastBackupModifiedTime?.getTime()) {
                                    serverWS.clients.forEach(client => {
                                        if (client.readyState === ws.OPEN) {
                                            client.send(`${filePathToWatch} was modified.`);
                                        }
                                    });
                                    lastBackupModifiedTime = currentModifiedTime;
                                }

                                backupChangeInProgress = false;
                            }, 200);
                        }
                    });
                });
            }
        });
    });

}




module.exports = serverWS => new FilesModule(serverWS);