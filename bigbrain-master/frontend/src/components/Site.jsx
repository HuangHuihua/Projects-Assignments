// import { Paper } from '@mui/material';
// import Grid2 from '@mui/material/Unstable_Grid2/Grid2';
import React from 'react';
import { useLocation, useNavigate, Outlet } from 'react-router-dom';
import Button from '@mui/material/Button';

function Site ({ setToken }) {
  const [page, setPage] = React.useState('signup');
  const navigate = useNavigate();
  const location = useLocation();

  function logout () {
    setToken(null);
    localStorage.removeItem('token');
    navigate('/signin');
    if (page === 'signin') {
      console.log();
    }
  }

  function ToDashboard () {
    navigate('/dashboard')
  }

  function ToJoinGame () {
    navigate('/Playersession')
  }

  return (<>
    <header>
      <nav>
        {!['/signup', '/signin'].includes(location.pathname)
          ? <>
              <a href="#" onClick={logout}>Logout</a>
              <Button onClick={ToDashboard}>Dashboard</Button>
              |
              <Button onClick={ToJoinGame}>Join Game</Button>
            </>
          : <>
              <a href="#" onClick={() => setPage('signup')}>Sign Up</a>
              |
              <a href="#" onClick={() => setPage('signin')}>Sign In</a>
            </>
        }
      </nav>
      <hr />
    </header>
    <main>
      <Outlet />
    </main>
  </>)
}

export default Site;
