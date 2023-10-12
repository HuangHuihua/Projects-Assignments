import React from 'react';
import { useNavigate } from 'react-router-dom';

function PlayerJoin () {
  // const { session } = useParams;
  const [sessionID, setSessionID] = React.useState();

  const navigate = useNavigate();

  function NavigateToGameSession (e) {
    setSessionID(e);
  }

  function NavigateToGameSessionClick () {
    navigate(`/PlayerPlay/${sessionID}`);
  }

  return (
    <>
    <div className='card col-4 mx-auto p-3'>
        <input className='form-control' placeholder = {'Enter a game (URL)'} onChange={(e) => NavigateToGameSession(e.target.value)} ></input><br></br>
        <button onClick = {NavigateToGameSessionClick} className='btn'>Go</button>
    </div>
    </>
  );
}

export default PlayerJoin;
