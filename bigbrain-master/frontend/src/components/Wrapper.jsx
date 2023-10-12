import React from 'react';
import { Routes, Route, useNavigate, useLocation } from 'react-router-dom';

import Site from './Site';
import SignIn from './SignIn';
import SignUp from './SignUp';
import Dashboard from './Dashboard';
import UpdateQuiz from './UpdateQuiz';
import UpdateQuestion from './UpdateQuestion';
import Session from './Session';
import PlayerJoin from './PlayerJoin';
import PlayerPlayGame from './PlayerPlayGame.jsx';

function Wrapper () {
//   const [page, setPage] = React.useState('signup');
  const [token, setToken] = React.useState(null);
  const [globalLoad, setGlobalLoad] = React.useState(true);
  const navigate = useNavigate();
  const location = useLocation();
  const [errors, setErrors] = React.useState({})

  // const [id, setId] = React.useState('');

  // function getId (props) {
  //   console.log(props.match.params.id);
  //   setId(props.match.params.id);
  // }

  // const props_ = { id_: id, token_: token };

  function manageTokenSet (token) {
    setToken(token);
    localStorage.setItem('token', token);
  }

  React.useEffect(function () {
    if (localStorage.getItem('token')) { // signed in
      setToken(localStorage.getItem('token'));
      if (['/signup', '/signin'].includes(location.pathname)) {
        navigate('/dashboard');
      }
    } else { // not signed in
      if (!['/signup', '/signin'].includes(location.pathname)) {
        navigate('/signin');
      }
    }
    setGlobalLoad(false);
  }, []);

  if (globalLoad) {
    return <>Loading...</>
  }

  return (
    <>
      <Routes>
        <Route path="/" element={<Site setToken={setToken} />}>
          <Route path="/dashboard" element={<Dashboard token={token} />} />
          <Route path="/signup" element={<SignUp onSuccess={manageTokenSet} />} />
          <Route
          path="/signin"
          element=
          {<SignIn
            errors={errors}
            onSubmit={({ email, password }) => {
              // Reset errors before validating & running business logic
              setErrors({})
              const newErrors = {}

              if (!email) {
                newErrors.email = 'Email is required'
              }

              if (!password) {
                newErrors.password = 'Password is required'
              }

              if (newErrors.email || newErrors.password) {
                setErrors(newErrors)
                return
              }

              if (email !== 'soorria@email.com') {
                setErrors({ email: 'User does not exist' })
                return
              }

              if (password !== 'super secure password') {
                setErrors({ password: 'Incorrect password' })
              }
            }}
            onSuccess = { manageTokenSet }
          />} />
          <Route path={'/quiz/:id'} element={<UpdateQuiz token={token}/>}></Route>
          <Route path={'/question/:id/:Qid'} element={<UpdateQuestion token={token}/>}></Route>
          <Route path={'/:id/:session'} element={<Session token={token}/>}></Route>
          <Route path={'/Playersession'} element={<PlayerJoin/>}></Route>
          <Route path={'/PlayerPlay/:session'} element={<PlayerPlayGame token={token}/>}></Route>
        </Route>
      </Routes>
      {/**/}
    </>
  );
}

export default Wrapper;
