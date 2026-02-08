const { createServer } = require('http');
const { readFileSync } = require('fs');
const { graphql, buildSchema } = require('graphql');

const config = require('./config').http;
const resolvers = require('./resolvers/index');
const schema = buildSchema(readFileSync('./schemas/schema.gql').toString());
const Db = require('./db/db');

const context = new Db();

const server = createServer((request, response) => {
  let body = '';
  request.on('data', chunk => body += chunk);
  request.on('end', () => {

    response.statusCode = 200;
    response.setHeader('Content-Type', 'application/json');

    if (request.url === '/' && request.method === 'POST') {
      let graphqlRequest = '';
      try {
        graphqlRequest = JSON.parse(body);
        if (graphqlRequest.query) {
          console.log(graphqlRequest.query);
          console.log(graphqlRequest.variables);
          graphql({ schema, source: graphqlRequest.query, rootValue: resolvers, contextValue: context, variableValues: graphqlRequest.variables })
            .then(result => {
              if (result.errors) {
                response.statusCode = 400;
              }
              response.end(JSON.stringify(result, null, '  '));
            }).catch(err => {
              response.statusCode = 500;
              response.end(JSON.stringify({ error: err }, null, '  '));
            });
        } else {
          response.statusCode = 400;
          response.end();
        }
      } catch (err) {
        response.statusCode = 500;
        response.end(JSON.stringify({ error: err }, null, '  '));
      }
    } else {
      response.statusCode = 404;
      response.end();
    }
  });
});

server.listen(config.port, config.host, () => {
  console.log(`Listening to http://${config.host}:${config.port}`);
});
//faculty
/*
 {
  "query": "query {getFaculties {FACULTY, FACULTY_NAME }}",
     "variables":{}
 }

   {
  "query": "query($FACULTY: String) {getFaculties(FACULTY: $FACULTY){FACULTY, FACULTY_NAME }}",
      "variables":{"FACULTY": "1"}
   }



   // GET PULPIT
  {   "query": "query {getPulpits {PULPIT, PULPIT_NAME,FACULTY }}",
     "variables":{}
 }

 {
    "query": "query($PULPIT: String) {getPulpits(PULPIT: $PULPIT) {PULPIT, PULPIT_NAME,FACULTY }}",
    "variables":{"PULPIT":"2"}
}


/////Get Teacher
 {   "query": "query {getTeachers {TEACHER, TEACHER_NAME,PULPIT }}",
     "variables":{}
 }

  {   "query": "query($TEACHER: String) {getTeachers(TEACHER: $TEACHER) {TEACHER, TEACHER_NAME,PULPIT }}",
     "variables":{"TEACHER":"2"}
 }



 //////
 {   "query": "query {getSubjects {SUBJECT, SUBJECT_NAME,PULPIT }}",
     "variables":{}
 }

  {   "query": "query($SUBJECT: String) {getSubjects(SUBJECT: $SUBJECT) {SUBJECT, SUBJECT_NAME,PULPIT }}",
     "variables":{"SUBJECT":"1"}
 }


 ///
  {   "query": "query($FACULTY: String!) {getSubjectsByFaculties(FACULTY: $FACULTY) {SUBJECT, SUBJECT_NAME,PULPIT}}",
     "variables":{"FACULTY":""}
 }

  { "query": "mutation($FACULTY: String!, $FACULTY_NAME:String!) { setFaculty(FACULTY: $FACULTY, FACULTY_NAME: $FACULTY_NAME) {FACULTY, FACULTY_NAME} }",
  "variables": {"FACULTY": "3","FACULTY_NAME":"FNgn"}
  }

 {
   "query": "mutation($FACULTY: String!, $FACULTY_NAME:String!) { setFaculty(FACULTY: $FACULTY, FACULTY_NAME: $FACULTY_NAME) {FACULTY, FACULTY_NAME} }",
  "variables": {"FACULTY": "3","FACULTY_NAME":"1"}
  }

  ////////
 {
   "query": "mutation($PULPIT: String!, $PULPIT_NAME: String!, $FACULTY: String!) { setPulpit(PULPIT: $PULPIT, PULPIT_NAME: $PULPIT_NAME, FACULTY: $FACULTY) {PULPIT, PULPIT_NAME, FACULTY} }",
  "variables": {"PULPIT": "10","PULPIT_NAME":"PUKЛ", "FACULTY":"2"}
    }

    {
   "query": "mutation($PULPIT: String!, $PULPIT_NAME: String!, $FACULTY: String!) { setPulpit(PULPIT: $PULPIT, PULPIT_NAME: $PULPIT_NAME, FACULTY: $FACULTY) {PULPIT, PULPIT_NAME, FACULTY} }",
  "variables": {"PULPIT": "10","PULPIT_NAME":"jkjkjkj", "FACULTY":"2"}
    }


  /////////////////////////////////
     {
   "query": "mutation($SUBJECT: String!, $SUBJECT_NAME: String!, $PULPIT: String!) { setSubject(SUBJECT: $SUBJECT, SUBJECT_NAME:$SUBJECT_NAME, PULPIT: $PULPIT){SUBJECT, SUBJECT_NAME, PULPIT} }",
  "variables": {"SUBJECT": "12","SUBJECT_NAME":"PUKFЛ", "PULPIT":"1"}
    }
     {
   "query": "mutation($SUBJECT: String!, $SUBJECT_NAME: String!, $PULPIT: String!) { setSubject(SUBJECT:$SUBJECT, SUBJECT_NAME:$SUBJECT_NAME, PULPIT:$PULPIT){SUBJECT, SUBJECT_NAME, PULPIT}}",
  "variables": {"SUBJECT": "PUKf","SUBJECT_NAME":"PUKFЛ", "PULPIT":"PUK"}
    }


///////////////////////////
{
   "query": "mutation($TEACHER: String!, $TEACHER_NAME: String!, $PULPIT: String!) { setTeacher(TEACHER: $TEACHER, TEACHER_NAME: $TEACHER_NAME, PULPIT: $PULPIT) {TEACHER, TEACHER_NAME, PULPIT} }",
  "variables": {"TEACHER": "4","TEACHER_NAME":"PUKЛ", "PULPIT":"2"}
    }

{
   "query": "mutation($TEACHER: String!, $TEACHER_NAME: String!, $PULPIT: String!) { setTeacher(TEACHER: $TEACHER, TEACHER_NAME: $TEACHER_NAME, PULPIT: $PULPIT) {TEACHER, TEACHER_NAME, PULPIT} }",
  "variables": {"TEACHER": "4","TEACHER_NAME":"kkkkk", "PULPIT":"2"}
    }
//////////////////////


    {
   "query": "mutation($PULPIT: String!, $PULPIT_NAME: String!, $FACULTY: String!) { setPulpit(PULPIT: $PULPIT, PULPIT_NAME: $PULPIT_NAME, FACULTY: $FACULTY) {PULPIT, PULPIT_NAME, FACULTY} }",
  "variables": {"PULPIT": "PUK","PULPIT_NAME":"PUKЛ", "FACULTY":"ИТ"}
    }


/////////////////////
     {
   "query": "mutation($FACULTY: String!) { delFaculty(FACULTY:$FACULTY)}",
  "variables": {"FACULTY": "3"}
    }
  
 {
   "query": "mutation($TEACHER: String!) { delTeacher(TEACHER:$TEACHER)}",
  "variables": {"TEACHER": "4"}
    }

 {
   "query": "mutation($PULPIT: String!) { delPulpit(PULPIT:$PULPIT)}",
  "variables": {"PULPIT": "5"}
    }
 {
   "query": "mutation($SUBJECT: String!) { delSubject(SUBJECT:$SUBJECT)}",
  "variables": {"SUBJECT": "4"}
    }


{
  "query": "query($FACULTY: String!) {getTeachersByFaculty(FACULTY: $FACULTY) {TEACHER, TEACHER_NAME, PULPIT}}",
  "variables": {
    "FACULTY": "2"
  }
}

      {   "query": "query($FACULTY: String!) {getSubjectsByFaculties(FACULTY: $FACULTY) {SUBJECT, SUBJECT_NAME,PULPIT}}",
     "variables":{"FACULTY":""}
 }

*/