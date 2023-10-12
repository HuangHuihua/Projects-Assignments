import React from 'react';
//  import Button from './Button';
import Box from '@mui/material/Box';
// import { View } from 'react-native';
import Card from '@mui/material/Card';
import CardContent from '@mui/material/CardContent';
import { useNavigate } from 'react-router-dom';
// import { IconButton } from '@mui/material';
// import { Link } from 'react-router-dom';
import Button from '@mui/material/Button';
// import DeleteIcon from '@mui/icons-material/Delete';
import CopyButton from './CopyToClipboard';
import Dialog from '@mui/material/Dialog';
import DialogActions from '@mui/material/DialogActions';
import DialogContent from '@mui/material/DialogContent';
import DialogContentText from '@mui/material/DialogContentText';

function Dashboard ({ token }) {
  const [newGameShow, setNewGameShow] = React.useState(false);
  const [quizzes, setQuizzes] = React.useState([]);

  const [newQuizName, setNewQuizName] = React.useState('');

  // const [showAlert, setShowAlert] = React.useState(false);

  const [sessionID, setShowSessionID] = React.useState(null);

  const [activeQuiz, setActiveQuiz] = React.useState(null);
  // const [GameStatus, setGameStatus] = React.useState(false);
  const [stopGame2, setStopGame2] = React.useState(null);

  // const [goDiffQuizzes, setDiffQuizzes] = React.useState('');

  const [open, setOpen] = React.useState(false);
  const [open2, setOpen2] = React.useState(false);

  const handleClickOpen = (e) => {
    setShowSessionID(e);
    console.log(e);
    console.log('hopen')
    console.log(sessionID);
    setOpen(true);
  };

  const handleClose = () => {
    setOpen(false);
  };

  const handleClickOpen2 = (e) => {
    // setShowSessionID(e);
    console.log(e);
    console.log('hopen2')
    console.log(sessionID);
    setOpen2(true);
  };
  const handleClose2 = () => {
    setOpen2(false);
  };

  const myRef = React.useRef(null);
  // const myRefDelete = React.useRef(null);
  // const myRefShowId = React.useRef(null);

  const navigate = useNavigate();

  async function fetchAllQuizzes () {
    const response = await fetch('http://localhost:5005/admin/quiz', {
      method: 'GET',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      }
    })
    const data = await response.json();
    setQuizzes(data.quizzes);
    // setShowSessionID(data.quizzes[0].active);
    // console.log(sessionID);
  }

  React.useEffect(async () => {
    await fetchAllQuizzes();
  }, [newGameShow]);

  async function createNewGame () {
    await fetch('http://localhost:5005/admin/quiz/new', {
      method: 'POST',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      },
      body: JSON.stringify({
        name: newQuizName,
      })
    });
    await fetchAllQuizzes();
  }

  function linkToDiffQuizzes (e) {
    const eId = e;
    const elementId = myRef.current.id;
    console.log(elementId);
    console.log(eId);
    navigate(`/quiz/${eId}`);
  }

  // async function NavigateToDiffQuizUpdate () {
  //   try {

  //   } catch {

  //   }
  // }

  async function stopGame (e) {
    handleClickOpen2();
    setStopGame2(e);
    const eId = e;
    console.log(e);
    await fetch('http://localhost:5005/admin/quiz/' + `${eId}` + '/end', {
      method: 'POST',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      }
    })
  }

  async function deleteQuiz (e) {
    console.log(e);
    await fetch('http://localhost:5005/admin/quiz/' + `${e}`, {
      method: 'DELETE',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      }
    })
    await fetchAllQuizzes();
    // const data = await response.json();
  }

  async function startGame (e) {
    alert('Game start! wait or fresh page and click button to get session id');
    // handleClickOpen(e);
    const eId = e;
    console.log(e);
    console.log(activeQuiz);
    await fetch('http://localhost:5005/admin/quiz/' + `${eId}` + '/start', {
      method: 'POST',
      headers: {
        'Content-type': 'application/json',
        Authorization: `Bearer ${token}`,
      }
    })
    // await fetchAllQuizzes();
  }

  // function showAlertClose () {
  //   setShowAlert(false)
  // }

  return <>
    Dashboard! list games...<br />
    <Box sx={{
      bgcolor: (theme) =>
        theme.palette.mode === 'dark' ? '#101010' : 'grey.100',
      color: (theme) =>
        theme.palette.mode === 'dark' ? 'grey.300' : 'grey.800',
      border: '1px solid',
      borderColor: (theme) =>
        theme.palette.mode === 'dark' ? 'grey.800' : 'grey.300',
      p: 1,
      borderRadius: 2,
      fontSize: '0.875rem',
      fontWeight: '700',
    }}>
      <Box sx={{
        display: 'grid',
        gridTemplateColumns: 'repeat(auto-fit, minmax(200px, 1fr))',
        gap: '20px'
      }}>
    {quizzes.map(quiz => (
      <>
    <Card style = { { width: '100%', flex: 1 } } >
      <CardContent>
    <img src="https://developer.android.com/static/images/cluster-illustrations/controllers.svg" className='card-img-top' alt="..."/>
    <div className="card-body">
        <b id = {`${quiz.id}`} className='card-title' >{quiz.name}</b><br />
        {quiz.id}<br/>
    {/* <Link to="/" className="btn btn-primary" id = {`${quiz.id}`}>Go somewhere</Link> */}
    <button id = {quiz.id} ref={myRef} value={quiz.id} onClick={(e) => linkToDiffQuizzes(e.target.value)} className="btn btn-primary">Edit Game</button><br/>
    <Button id = {quiz.id + 'startGame'} variant="contained" value={quiz.id} onClick={(e) => { startGame(e.target.value); setActiveQuiz(e.target.value) }}>Start Game</Button>
    <Button id = {quiz.id + 'stopGame'} variant="contained" value={quiz.id} onClick={(e) => { stopGame(e.target.value) }}>Stop Game</Button>
    <Button id = {quiz.id + 'showSessionId2'} value={quiz.active} onClick= {(e) => handleClickOpen(e.target.value) } >Show sessionID</Button>
    <button id = {activeQuiz + 'gosession' } value={activeQuiz} onClick={() => { navigate(`/${quiz.id}/${quiz.active}`) }} className="btn">Go to session control game</button>
    {console.log(quiz.active)}
    <Button id = {quiz.id + 'deleteGame'} value={quiz.id} onClick={(e) => deleteQuiz(e.target.value)}>Delete</Button>
    {/* <IconButton id = {quiz.id + '_delete'} value={quiz.id} key={quiz.id + '_delete'} onClick={(e) => deleteQuiz(e.target.value)}><DeleteIcon/></IconButton> */}
    </div>
    </CardContent>
    </Card>
      </>
    ))}
    {(
      <>
        {/* {alert(sessionID)} */ console.log('sessionId:' + sessionID)}
        <Dialog id = {'dial'} key={'dial'}
        open={open}
        onClose={handleClose}
        aria-labelledby="alert-dialog-title"
        aria-describedby="alert-dialog-description"
      >
        <DialogContent id = {'dial1'} key={'dial1'}>
          <DialogContentText id = {'dial2'} key={'dial2'}>
            {sessionID}
          </DialogContentText>
        </DialogContent>
        <DialogActions id = {'dial3'} key={'dial3'}>
          <Button id={'Alert'} onClick={handleClose}>Close</Button>
          <CopyButton sessionID={sessionID}/>
        </DialogActions>
      </Dialog>

      <Dialog id = {'diall'} key={'diall'}
        open={open2}
        onClose={handleClose2}
        aria-labelledby="alert-dialog-title"
        aria-describedby="alert-dialog-description"
      >
        <DialogContent id = {'diall1'} key={'dial1'}>
          <DialogContentText id = {'diall2'} key={'diall2'}>
          Would you like to view the results?
          </DialogContentText>
        </DialogContent>
        <DialogActions id = {'diall3'} key={'diall3'}>
          <Button id={'Alert'} onClick={ () => { navigate(`/${stopGame2}/${sessionID}`) }}>Yes</Button>
          <Button id={'Alert'} onClick={handleClose2}>Close</Button>
          {/* <CopyButton sessionID={sessionID}/> */}
        </DialogActions>
      </Dialog>
      </>
    )}
    </Box>
    </Box>
    <br /><hr /><br />
    <button className='btn btn-primary' onClick={() => setNewGameShow(!newGameShow)}>
      {newGameShow ? 'Hide' : 'Show'} Create New Game
    </button>
    {newGameShow && (
      <>
        <br />
        Form here for new game!<br />
        Name: <input value={newQuizName} onChange={(e) => setNewQuizName(e.target.value)} /><br />
        <button className='btn btn-primary' onClick={createNewGame}>Create new game</button>
      </>
    )}
    <br /><hr /><br />
    <></>
  </>;
}

export default Dashboard;
