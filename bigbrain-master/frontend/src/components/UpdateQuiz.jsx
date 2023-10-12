import React from 'react';
//  import Button from './Button';
import Box from '@mui/material/Box';
// import { View } from 'react-native';
// import Card from '@mui/material/Card';
// import CardContent from '@mui/material/CardContent';
// import { Navigate } from 'react-router-dom';
import Grid from '@mui/material/Grid';
import Paper from '@mui/material/Paper';
// import Typography from '@mui/material/Typography';
import Input from '@mui/material/Input';
import Button from '@mui/material/Button';
// import Card from '@mui/material/Card';
import IconButton from '@mui/material/IconButton';
import PhotoCamera from '@mui/icons-material/PhotoCamera';
import DeleteIcon from '@mui/icons-material/Delete';
import Checkbox from '@mui/material/Checkbox';
import { useParams, useNavigate } from 'react-router-dom';
import { v4 as uuidv4 } from 'uuid';

function UpdateQuiz ({ token }) {
  // console.log(token);
  const { id } = useParams();
  const [questions, setQuestions] = React.useState([]);
  const [questionUpdate, setQuestionUpdate] = React.useState(false);

  const [name_, setName] = React.useState('');
  const [url_, setUrl_] = React.useState('');

  const [title_, setTitle] = React.useState('');
  const [choiceA, setChoiceA] = React.useState('');
  const [choiceB, setChoiceB] = React.useState('');
  const [choiceC, setChoiceC] = React.useState('');
  const [choiceD, setChoiceD] = React.useState('');
  const [idA, setidA] = React.useState('');
  const [idB, setidB] = React.useState('');
  const [idC, setidC] = React.useState('');
  const [idD, setidD] = React.useState('');
  const [IsA, setIsA] = React.useState(false);
  const [IsB, setIsB] = React.useState(false);
  const [IsC, setIsC] = React.useState(false);
  const [IsD, setIsD] = React.useState(false);
  const [photo_, setPhoto] = React.useState('');
  const [timelimit, setTimelimit] = React.useState(0);
  const [typeQ, setTypeQ] = React.useState('S');
  const [points, setPoints] = React.useState(5);
  const [No_, setNo] = React.useState(0);
  // const [id_, setId_] = React.useState();
  const navigate = useNavigate();
  const [QuestionArray, setQuestionArray] = React.useState([]);
  let data;

  // const [flatContent, setFlatContent] = React.useState('');
  const myRef = React.useRef(null);
  console.log(id);
  async function fetchOneQuiz () {
    const response = await fetch('http://localhost:5005/admin/quiz/' + `${id}`, {
      method: 'GET',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      }
    })
    data = await response.json();
    console.log(data.questions);
    // setQuestionArray(data.questions);
    // console.log(data.name);
    // if (data.questions !== 0) {
    setQuestions(data.questions);
    // } else setQuestions([]);
  }

  React.useEffect(async () => {
    await fetchOneQuiz();
  }, [questionUpdate]);

  const newQuestion = {
    id: uuidv4(),
    title: title_,
    photo: photo_,
    StartAt: '0',
    Type: typeQ,
    TimeLimit: timelimit,
    point: points,
    choice: [
      {
        choiceId: uuidv4(),
        choiceContent: choiceA,
        IsAnswer: IsA,
      },
      {
        choiceId: uuidv4(),
        choiceContent: choiceB,
        IsAnswer: IsB,
      },
      {
        choiceId: uuidv4(),
        choiceContent: choiceC,
        IsAnswer: IsC,
      },
      {
        choiceId: uuidv4(),
        choiceContent: choiceD,
        IsAnswer: IsD,
      },
      {}
    ]
  };

  function appendQuestionArray () {
    const newArray = [...QuestionArray, newQuestion];
    setQuestionArray(newArray);
  }

  async function Create () {
    appendQuestionArray();
    await fetch('http://localhost:5005/admin/quiz/' + `${id}`, {
      method: 'PUT',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      },
      body: JSON.stringify({
        questions: QuestionArray,
        name: name_,
        thumbnail: url_.toString()
      })
    });
    await fetchOneQuiz();
  }

  function beforeUpdate (e) {
    const updatedQuestions = QuestionArray.map((question) => {
      // if (id_ === '' || idA === '' || idB === '' || idC === '' || idD === '' || choiceA === '') {
      //   // alert('Please fill all the input!');
      //   // id_ = question.id;
      //   // idA = question.choice[0].choiceId;
      //   // idB = question.choice[1].choiceId;
      //   // idC = question.choice[2].choiceId;
      //   // idD = question.choice[3].choiceId;
      //   return question;
      // }
      if (question.id === e) {
        question.id = e;
        question.No = No_;
        question.title = title_;
        question.photo = photo_;
        question.StartAt = '0';
        question.Type = typeQ;
        question.TimeLimit = timelimit;
        question.point = points;
        question.choice = [
          {
            choiceId: idA,
            choiceContent: choiceA,
            IsAnswer: IsA,
          },
          {
            choiceId: idB,
            choiceContent: choiceB,
            IsAnswer: IsB,
          },
          {
            choiceId: idC,
            choiceContent: choiceC,
            IsAnswer: IsC,
          },
          {
            choiceId: idD,
            choiceContent: choiceD,
            IsAnswer: IsD,
          },
          {}
        ];
      }
      return question;
    });

    setQuestionArray(updatedQuestions);
  }

  async function Update (e) {
    await beforeUpdate(e);
    await fetch('http://localhost:5005/admin/quiz/' + `${id}`, {
      method: 'PUT',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      },
      body: JSON.stringify(
        {
          questions: QuestionArray,
          name: name_,
          thumbnail: url_.toString()
        }
      )
    });
    await fetchOneQuiz();
  }

  const removeQuestion = (e) => {
    const updatedQuestions = questions.filter((question) => question.id !== e);
    setQuestionArray(updatedQuestions);
  };

  async function Delete (e) {
    removeQuestion(e);
    await fetch('http://localhost:5005/admin/quiz/' + `${id}`, {
      method: 'PUT',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      },
      body: JSON.stringify(
        {
          questions: QuestionArray,
          name: name_,
          thumbnail: url_.toString()
        }
      )
    });
    await fetchOneQuiz();
  }

  return (
      <>
        <Box>
          <Button variant="contained" onClick={() => { navigate('/dashboard') }}>Back</Button>
        </Box>
        <br/><hr/><br/>
        <Box>
          <Grid item xs={12}>
            Quiz Name
            <Input value={name_} defaultValue="Title" onChange={(e) => setName(e.target.value)}></Input><br/>
            Thumbnail
            <Input value={url_} onChange={(e) => setUrl_(e.target.value)}></Input><br/>
            Question
            Question No. <Input type = "number" placeholder={'No. Q'} onChange={(e) => setNo(e.target.value)}></Input><br/>
            Question Title <Input value={title_} defaultValue="Title" placeholder={'AB'} onChange={(e) => setTitle(e.target.value)}></Input><br/>
            <PhotoCamera /><Input value={photo_} onChange={(e) => setPhoto(e.target.value)} /><br/><br/>
            TimeLimit <Input type = "number" onChange={(e) => setTimelimit(e.target.value)}></Input> <br/>
            Point <Input type = "number" onChange={(e) => setPoints(e.target.value)}></Input><br/>
            Mul or Single? <input type="radio" value="M" checked={typeQ === 'M'} onChange={(e) => setTypeQ(e.target.value)}></input>|<input type="radio" value="S" checked={typeQ === 'S'} onChange={(e) => setTypeQ(e.target.value)}></input><b/>
            Choice Update:<br/>
            A<Checkbox id='checkboxA' checked={IsA} onChange={() => setIsA(!IsA)}/>Content  <Input value = {choiceA} onChange={(e) => setChoiceA(e.target.value)} /><br/>
            B<Checkbox id='checkboxB' checked={IsB} onChange={() => setIsB(!IsB)}/>Content  <Input value = {choiceB} onChange={(e) => setChoiceB(e.target.value)} /><br/>
            C<Checkbox id='checkboxC' checked={IsC} onChange={() => setIsC(!IsC)}/>Content  <Input value = {choiceC} onChange={(e) => setChoiceC(e.target.value)} /><br/>
            D<Checkbox id='checkboxD' checked={IsD} onChange={() => setIsD(!IsD)}/>Content  <Input value = {choiceD} onChange={(e) => setChoiceD(e.target.value)} /><br/>
            <Button variant="contained" onClick={Create}>Click twice Create Question</Button>
            <br/><hr/><br/>
            Click question tile to update each question
          </Grid>
            <br/>
        </Box>
        <Box sx={{
          display: 'grid',
          gridTemplateColumns: 'repeat(auto-fit, minmax(200px, 1fr))',
          gap: '20px'
        }}>
          <Grid
            container spacing={5}
            direction="column-reverse"
            justifyContent="space-around"
            alignItems="stretch"
          >
        {questions.map(question => (
          <>
              <Grid container item spacing={1}>
                <Grid onClick = {() => setQuestionUpdate(!questionUpdate)} item xs={12}>
                  <Button onClick={() => { navigate(`/question/${id}/${question.id}`) }}>Edit Question</Button>
                  <h3>{question.title}</h3>
                </Grid>
                <Grid id = {question.choice[0].choiceId} item xs={6}>
                  <Paper>A {
                    question.choice[0].choiceContent
                  }
                  </Paper>
                </Grid>
                <Grid id = {question.choice[1].choiceId} item xs={6}>
                  <Paper>B {
                    question.choice[1].choiceContent
                  }</Paper>
                </Grid>
                <Grid id = {question.choice[2].choiceId} item xs={6}>
                  <Paper>C {
                    question.choice[2].choiceContent
                  }</Paper>
                </Grid>
                <Grid id = {question.choice[3].choiceId} item xs={6}>
                  <Paper>D {
                    question.choice[3].choiceContent
                  }</Paper>
                </Grid>
                <IconButton color="primary" value = {question.id} onClick ={(e) => Delete(e.target.value)}>
                  <DeleteIcon />
                </IconButton>
                {questionUpdate && (
                  <>
                  <Grid id={question.id + 'Uid'} item xs={12}>
                    Question
                    <Input id={question.id + 'tit'} placeholder={question.title} defaultValue="choice" onChange={(e) => setTitle(e.target.value)}></Input><br/>
                    {/* <Input value={question.id + 'IDID'} onChange={(e) => setId_(e.target.value)} placeholder={question.id}/><br/> */}
                    A<Checkbox id={question.id + 'input0'} checked={IsA} onChange={() => setIsA(!IsA)}/><Input id={question.id + 'input00'} placeholder={question.choice[0].choiceId} value = {question.choice[0].choiceId} onChange={(e) => setidA(e.target.value)} /><Input id={question.id + 'input000'} value = {choiceA} onChange={(e) => setChoiceA(e.target.value)} /><br/>
                    B<Checkbox id={question.id + 'input1'} checked={IsB} onChange={() => setIsB(!IsB)}/><Input id={question.id + 'input11'} placeholder={question.choice[1].choiceId} value = {question.choice[1].choiceId} onChange={(e) => setidB(e.target.value)} /><Input id={question.id + 'input111'} value = {choiceB} onChange={(e) => setChoiceB(e.target.value)} /><br/>
                    C<Checkbox id={question.id + 'input2'} checked={IsC} onChange={() => setIsC(!IsC)}/><Input id={question.id + 'input22'} placeholder={question.choice[2].choiceId} value = {question.choice[2].choiceId} onChange={(e) => setidC(e.target.value)} /><Input id={question.id + 'input222'} value = {choiceC} onChange={(e) => setChoiceC(e.target.value)} /><br/>
                    D<Checkbox id={question.id + 'input3'} checked={IsD} onChange={() => setIsD(!IsD)}/><Input id={question.id + 'input33'} placeholder={question.choice[3].choiceId} value = {question.choice[3].choiceId} onChange={(e) => setidD(e.target.value)} /><Input id={question.id + 'input333'} value = {choiceD} onChange={(e) => setChoiceD(e.target.value)} /><br/>
                    <Button id={question.id + 'button'} value={question.id} ref={myRef} onClick={(e) => Update(e.target.value)}>Update</Button>
                  </Grid>
                    <br/>

                  </>
                )}
              </Grid>
          </>
        ))
      }
          </Grid>
        </Box>
      </>
  );
}

export default UpdateQuiz;
