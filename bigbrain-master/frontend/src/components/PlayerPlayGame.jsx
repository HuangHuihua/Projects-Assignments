import Input from '@mui/material/Input';
import Button from '@mui/material/Button';
import Box from '@mui/material/Box';
import React from 'react';
import { useParams } from 'react-router-dom';

function PlayerPlayGame ({ token }) {
  const { session } = useParams();
  const [playerName, setplayerName] = React.useState('');
  const [playerId, setPlayerId] = React.useState('');
  const [globalLoad, setGlobalLoad] = React.useState(true);
  const [question, setQuestion] = React.useState('');
  let data;
  // React.useEffect(async () => {
  //   await getPlayerId();
  // }, []);

  // async function getQuizStatus () {
  //   const response = await fetch('http://localhost:5005/admin/session/' + `${session}` + '/status', {
  //     method: 'GET',
  //     headers: {
  //       'Content-type': 'application/json',
  //       Authorization: `Bearer ${token}`,
  //     }
  //   })
  //   const data = await response.json();
  //   console.log(session);
  //   console.log(data.active);
  // }

  console.log(session);

  async function getPlayerId () {
    const response = await fetch('http://localhost:5005/play/join/' + `${session}`, {
      method: 'POST',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      },
      body: JSON.stringify({
        name: playerName,
      })
    });
    data = await response.json();
    console.log(session);
    console.log(playerName);
    console.log(data.playerId);
    await getPlayerQuestion();
    await getPlayerStatus();
  }

  React.useEffect(() => {
    getPlayerId();
  }, [playerId]);

  setPlayerId(data.playerId);

  console.log(playerId);

  async function getPlayerQuestion () {
    await getPlayerId();
    if (playerId !== '') {
      const response = await fetch('http://localhost:5005/play/' + `${data.playerId}` + '/status', {
        method: 'GET',
        headers: {
          'Content-type': 'application/json',
          Authorization: `Bearer ${token}`,
        }
      });
      const data_ = await response.json();
      console.log(session);
      console.log('getPlayerQuestion');
      console.log(data_);
      setQuestion();
    }
  }

  async function getPlayerStatus () {
    const response = await fetch('http://localhost:5005/play/' + `${playerId}` + '/status', {
      method: 'GET',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      }
    });
    const data = await response.json();
    console.log(session);
    console.log(data.started);
    setGlobalLoad(data.started);
  }

  // get question
  // async function getCurrentQuestion () {
  //   const response = await fetch()
  // }

  if (globalLoad === false && playerId !== '') {
    return <>Please Wait</>
  }

  React.useEffect(() => {
    // getCurrentQuestion();
  }, [1000]);

  return (
    <>
    <Box>Player Play</Box>
    {playerId === ''
      ? <>
        <form onSubmit={(e) => { e.preventDefault() }}>
          <Input placeholder='name' value={playerName} onChange={(e) => setplayerName(e.target.value) }></Input>
          <Button onClick={getPlayerId}>Start</Button>
        </form>
        </>
      : <>
        {question.title}
        </>
    }
    </>
  );
}

export default PlayerPlayGame;
