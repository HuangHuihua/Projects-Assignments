// import React from 'react';
// //  import Button from './Button';
// import Box from '@mui/material/Box';
// // import { View } from 'react-native';
// // import Card from '@mui/material/Card';
// // import CardContent from '@mui/material/CardContent';
// // import { Navigate } from 'react-router-dom';
// import Grid from '@mui/material/Grid';
// import Paper from '@mui/material/Paper';
// // import Typography from '@mui/material/Typography';
// import Input from '@mui/material/Input';
// import Button from '@mui/material/Button';
// // import Card from '@mui/material/Card';
// import IconButton from '@mui/material/IconButton';
// import PhotoCamera from '@mui/icons-material/PhotoCamera';
// import DeleteIcon from '@mui/icons-material/Delete';
// import Checkbox from '@mui/material/Checkbox';
// import { useParams, useNavigate } from 'react-router-dom';
// import { v4 as uuidv4 } from 'uuid';

// function UpdateQuiz ({ token }) {
//   // console.log(token);
//   const { id } = useParams();
//   const [questions, setQuestions] = React.useState([]);
//   const [questionUpdate, setQuestionUpdate] = React.useState(false);

//   const [name_, setName] = React.useState('');
//   const [url_, setUrl_] = React.useState('');

//   const [title_, setTitle] = React.useState('');
//   const [choiceA, setChoiceA] = React.useState('');
//   const [choiceB, setChoiceB] = React.useState('');
//   const [choiceC, setChoiceC] = React.useState('');
//   const [choiceD, setChoiceD] = React.useState('');
//   const [idA, setidA] = React.useState('');
//   const [idB, setidB] = React.useState('');
//   const [idC, setidC] = React.useState('');
//   const [idD, setidD] = React.useState('');
//   const [IsA, setIsA] = React.useState(false);
//   const [IsB, setIsB] = React.useState(false);
//   const [IsC, setIsC] = React.useState(false);
//   const [IsD, setIsD] = React.useState(false);
//   const [photo_, setPhoto] = React.useState('');
//   const [id_, setId_] = React.useState();
//   const navigate = useNavigate();
//   const [QuestionArray, setQuestionArray] = React.useState([]);

//   // const [flatContent, setFlatContent] = React.useState('');
//   const myRef = React.useRef(null);

//   async function fetchOneQuiz () {
//     const response = await fetch('http://localhost:5005/admin/quiz/' + `${id}`, {
//       method: 'GET',
//       headers: {
//         'Content-type': 'application/json',
//         Authorization: `Bearer ${token}`,
//       }
//     })
//     const data = await response.json();
//     console.log(data.questions);
//     console.log(data.name);
//     setQuestions(data.questions);
//   }

//   React.useEffect(async () => {
//     await fetchOneQuiz();
//   }, []);

//   const newQuestion = {
//     id: uuidv4(),
//     title: title_,
//     photo: photo_,
//     choice: [
//       {
//         choiceId: uuidv4(),
//         choiceContent: choiceA,
//         IsAnswer: IsA,
//       },
//       {
//         choiceId: uuidv4(),
//         choiceContent: choiceB,
//         IsAnswer: IsB,
//       },
//       {
//         choiceId: uuidv4(),
//         choiceContent: choiceC,
//         IsAnswer: IsC,
//       },
//       {
//         choiceId: uuidv4(),
//         choiceContent: choiceD,
//         IsAnswer: IsD,
//       },
//       {}
//     ]
//   };

//   function appendQuestionArray () {
//     const newArray = [...QuestionArray, newQuestion];
//     setQuestionArray(newArray);
//   }

//   async function Update () {
//     await appendQuestionArray();
//     await fetch('http://localhost:5005/admin/quiz/' + `${id}`, {
//       method: 'PUT',
//       headers: {
//         'Content-type': 'application/json',
//         Authorization: `Bearer ${token}`,
//       },
//       body: JSON.stringify({
//         questions: QuestionArray,
//         name: name_,
//         thumbnail: url_.toString()
//       })
//     });
//     await fetchOneQuiz();
//   }

//   async function Delete () {

//   }

//   return (
//       <>
//         <Box>
//           <Button variant="contained" onClick={() => { navigate('/dashboard') }}>Back</Button>
//         </Box>
//         <br/><hr/><br/>
//         <Grid>
//           <Grid item xs={12}>
//             Quiz Name
//             <Input value={name_} onChange={(e) => setName(e.target.value)}></Input><br/>
//             Thumbnail
//             <Input value={url_} onChange={(e) => setUrl_(e.target.value)}></Input><br/>
//             Question
//             Question Title<Input value={title_} onChange={(e) => setTitle(e.target.value)}></Input><br/>
//             <PhotoCamera /><Input value={photo_} onChange={(e) => setPhoto(e.target.value)} />
//             A<Checkbox checked={IsA} onChange={() => setIsA(!IsA)}/>Id<Input value = {idA} onChange={(e) => setidA(e.target.value)} />Content<Input value = {choiceA} onChange={(e) => setChoiceA(e.target.value)} /><br/>
//             B<Checkbox checked={IsB} onChange={() => setIsB(!IsB)}/>Id<Input value = {idB} onChange={(e) => setidB(e.target.value)} />Content<Input value = {choiceB} onChange={(e) => setChoiceB(e.target.value)} /><br/>
//             C<Checkbox checked={IsC} onChange={() => setIsC(!IsC)}/>Id<Input value = {idC} onChange={(e) => setidC(e.target.value)} />Content<Input value = {choiceC} onChange={(e) => setChoiceC(e.target.value)} /><br/>
//             D<Checkbox checked={IsD} onChange={() => setIsD(!IsD)}/>Id<Input value = {idD} onChange={(e) => setidD(e.target.value)} />Content<Input value = {choiceD} onChange={(e) => setChoiceD(e.target.value)} /><br/>
//             <Button variant="contained" onClick={Update}>Create Question</Button>
//             <br/><hr/><br/>
//             Click question tile to update each question
//           </Grid>
//             <br/>
//         </Grid>
//         <Box sx={{
//           display: 'grid',
//           gridTemplateColumns: 'repeat(auto-fit, minmax(200px, 1fr))',
//           gap: '20px'
//         }}>
//           <Grid
//             container spacing={5}
//             direction="column-reverse"
//             justifyContent="space-around"
//             alignItems="stretch"
//           >
//         {questions.map(question => (
//           <>
//               <Grid container item spacing={1}>
//                 <Grid id = {question.id + 'grid'} onClick = {() => setQuestionUpdate(!questionUpdate)} item xs={12}>
//                   <h3>{question.title}</h3>
//                 </Grid>
//                 <Grid id = {question.choice[0].choiceId} item xs={6}>
//                   <Paper>A {
//                     question.choice[0].choiceContent
//                   }
//                   </Paper>
//                 </Grid>
//                 <Grid id = {question.choice[1].choiceId} item xs={6}>
//                   <Paper>B {
//                     question.choice[1].choiceContent
//                   }</Paper>
//                 </Grid>
//                 <Grid id = {question.choice[2].choiceId} item xs={6}>
//                   <Paper>C {
//                     question.choice[2].choiceContent
//                   }</Paper>
//                 </Grid>
//                 <Grid id = {question.choice[3].choiceId} item xs={6}>
//                   <Paper>D {
//                     question.choice[4].choiceContent
//                   }</Paper>
//                 </Grid>
//                 <IconButton color="primary" onClick ={Delete}>
//                   <DeleteIcon />
//                 </IconButton>
//                 {questionUpdate && (
//                   <>
//                   <Grid item xs={12}>
//                     Question
//                     <Input placeholder={question.title} value={title_} onChange={(e) => setTitle(e.target.value)}></Input><br/>
//                     <Input value={id_} onChange={(e) => setId_(e.target.value)} placeholder={question.id}/><br/>
//                     A<Checkbox id={question.id + 'input0'} checked={IsA} onChange={() => setIsA(!IsA)}/><Input placeholder={question.choice[0].choiceId} value = {idA} onChange={(e) => setidA(e.target.value)} /><Input value = {choiceA} onChange={(e) => setChoiceA(e.target.value)} /><br/>
//                     B<Checkbox id={question.id + 'input1'} checked={IsB} onChange={() => setIsB(!IsB)}/><Input placeholder={question.choice[1].choiceId} value = {idB} onChange={(e) => setidB(e.target.value)} /><Input value = {choiceB} onChange={(e) => setChoiceB(e.target.value)} /><br/>
//                     C<Checkbox id={question.id + 'input2'} checked={IsC} onChange={() => setIsC(!IsC)}/><Input placeholder={question.choice[2].choiceId} value = {idC} onChange={(e) => setidC(e.target.value)} /><Input value = {choiceC} onChange={(e) => setChoiceC(e.target.value)} /><br/>
//                     D<Checkbox id={question.id + 'input3'}checked={IsD} onChange={() => setIsD(!IsD)}/><Input placeholder={question.choice[3].choiceId} value = {idD} onChange={(e) => setidD(e.target.value)} /><Input value = {choiceD} onChange={(e) => setChoiceD(e.target.value)} /><br/>
//                     <Button id={question.id + 'button'} ref={myRef} onClick={Update}>Update</Button>
//                   </Grid>
//                     <br/>

//                   </>
//                 )}
//               </Grid>
//           </>
//         ))
//       }
//           </Grid>
//         </Box>
//       </>
//   );
// }

// export default UpdateQuiz;
