import React from 'react';
//  import bootstrap from 'bootstrap';
import 'bootstrap/dist/css/bootstrap.min.css';
import { Link, useNavigate } from 'react-router-dom';

function SignUp ({ onSuccess }) {
  const [email, setEmail] = React.useState('');
  const [password, setPassword] = React.useState('');
  const [name, setName] = React.useState('');
  const navigate = useNavigate();

  async function register (e) {
    e.preventDefault();
    if (!email) {
      alert('please input your email');
      return;
    }

    if (!password) {
      alert('please set your password');
      return;
    }

    if (!name) {
      alert('please input user name');
      return;
    }
    try {
      const response = await fetch('http://localhost:5005/admin/auth/register', {
        method: 'POST',
        headers: {
          'Content-type': 'application/json',
        },
        body: JSON.stringify({
          email,
          password,
          name,
        })
      });
      const data = await response.json();
      onSuccess(data.token);
      alert('Register successful');
      navigate('/dashboard');
    } catch (error) {
      alert(error);
    }
  }

  return (
        <>
            <div className='card col-4 mx-auto p-3'>
            Email: <input name="email" type="emial" className='form-control' onChange={(e) => setEmail(e.target.value)} ></input><br></br>
            Password: <input name="password" type="password" className='form-control' onChange={(e) => setPassword(e.target.value)}></input><br></br>
            Name: <input name="name" className='form-control' onChange={(e) => setName(e.target.value)}></input><br/>
            <button onClick = {register} className='btn'>Sign Up</button>
            </div>
            <hr />
            <Link to="/signin">Already a user? Sign in now</Link>
        </>
  );
}

export default SignUp;
