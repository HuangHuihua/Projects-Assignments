import React from 'react';
// import Dashboard from './components/Dashboard';
// import SignIn from './components/SignIn';
// import SignUp from './components/SignUp';
import 'bootstrap/dist/css/bootstrap.min.css';
import { BrowserRouter as Router } from 'react-router-dom';
import Wrapper from './components/Wrapper';

// import Box from '@mui/material/Box';
// import { StyleSheet } from 'react-native';

function App () {
//   const [page, setPage] = React.useState('signup');
//   const [token, setToken] = React.useState(null);

  //   // const styles = StyleSheet.create({})

  //   function manageTokenSet (token) {
  //     setToken(token);
  //     localStorage.setItem('token', token);
  //     console.log(token);
  //   }

  //   function logout () {
  //     setToken(null);
  //     localStorage.removeItem('token');
  //   }

  //   React.useEffect(function () {
  //     if (localStorage.getItem('token')) {
  //       setToken(localStorage.getItem('token'));
  //     } else {
  //     }
  //   }, [])

  return (
    <>
      <Router>
        <Wrapper/>
      </Router>
      {/* <header>
        <nav>
          {token
            ? <>
              <a href = "#" onClick={logout}>Logout</a>
            </>
            : <>
              <a href = "#" onClick= { () => { setPage('signup') } }>Sign Up</a>
              <a href = "#" onClick= { () => { setPage('signin') } }>Sign In</a>
            </>
          }
        </nav>
      </header>
      <main>
        {token !== null
          ? <Dashboard token={token}/>
          : page === 'signup'
            ? <SignUp onSuccess={manageTokenSet}></SignUp>
            : <SignIn onSuccess={manageTokenSet}></SignIn>
        }
      </main> */}
    </>
  );
}

export default App;
