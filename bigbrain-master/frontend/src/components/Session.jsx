import React from 'react';
import Button from '@mui/material/Button';
import { Navigate, useParams } from 'react-router-dom';
import { Box } from '@mui/material';

function Session ({ token }) {
  const { id, session } = useParams();
  const [quizStatus, setQuizStatus] = React.useState(false);
  const [time, setTime] = React.useState(10);
  const [position, setPosition] = React.useState(-1);
  const [questions, setQuestionArray] = React.useState();

  //  get quiz question and timelimit
  async function fetchOneQuiz () {
    const response = await fetch('http://localhost:5005/admin/quiz/' + `${id}`, {
      method: 'GET',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      }
    })
    const data = await response.json();
    console.log(data.questions);
    setQuestionArray(data.questions);
    console.log(data.name);
  }

  React.useEffect(() => {
    fetchOneQuiz();
  }, [])

  async function getSessionStatus () {
    try {
      const response = await fetch('http://localhost:5005/admin/session/' + `${session}` + '/status', {
        method: 'GET',
        headers: {
          'Content-type': 'application/json',
          Authorization: `Bearer ${token}`,
        }
      })
      const data = await response.json();
      console.log(session);
      console.log(data);
      console.log(data.result.active);
      console.log(data.result.position);
      setQuizStatus(data.result.active);
      setPosition(data.result.position);
    } catch (error) {
      console.log(error);
      alert('');
      Navigate('/dashboard');
    }
  }

  async function stopGame () {
    await fetch('http://localhost:5005/admin/quiz/' + `${id}` + '/end', {
      method: 'POST',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      }
    })
    await getResults();
  }

  async function advanceQues () {
    await fetch('http://localhost:5005/admin/quiz/' + `${session}` + '/advance', {
      method: 'POST',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      }
    });
  }

  if (position === -1) {
    advanceQues();
  }

  React.useEffect(() => {
    let time_ = time;
    setTime(--time_);
    if (time === 0 && questions.length) {
      advanceQues();
    }
  }, 1000);

  async function getResults () {
    const response = await fetch('http://localhost:5005/admin/quiz/' + `${session}` + '/results', {
      method: 'GET',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      }
    });
    await response.json();
  }

  React.useEffect(async () => {
    await getSessionStatus();
  }, []);

  return (
    <>
      Session!
      {
        quizStatus
          ? <Box>
            <Button onClick={stopGame}>Stop Game</Button> | <Button onClick={advanceQues}>Advance</Button>
            </Box>
          : <Box>
              Result: <br/><hr/><br/>
              <></>
            </Box>
      }
    </>
  );
}

export default Session;
