//  import bootstrap from 'bootstrap';
import React from 'react';
import 'bootstrap/dist/css/bootstrap.min.css';
import { Link, useNavigate } from 'react-router-dom';

const IDS = {
  emailInput: 'email',
  emailError: 'login-email-error',
  passwordInput: 'password',
  passwordError: 'login-password-error',
}

function SignIn ({ onSuccess, onSubmit, errors = {} }) {
  const [email, setEmail] = React.useState('');
  const [password, setPassword] = React.useState('');
  const navigate = useNavigate();

  async function login (e) {
    e.preventDefault();
    if (!email || !password) {
      alert('Please insert the email and password');
      return;
    }
    try {
      const response = await fetch('http://localhost:5005/admin/auth/login', {
        method: 'POST',
        headers: {
          'Content-type': 'application/json',
        },
        body: JSON.stringify({
          email,
          password,
        })
      });
      const data = await response.json();
      onSuccess(data.token);
      navigate('/dashboard');
    } catch (error) {
      alert(error);
    }
  }
  return (
        <>
            <div className='card col-4 mx-auto p-3'>
            <label htmlFor={IDS.emailInput}>Email:</label>
            <input
            id={IDS.emailInput}
            name="email"
            type="email"
            required
            aria-invalid={errors.email ? 'true' : 'false'}
            aria-describedby={errors.email ? IDS.emailError : undefined}
            value={email}
            className='form-control' onChange={(e) => setEmail(e.target.value)}></input>
            {errors.email
              ? (
              <p id={IDS.emailError} aria-live="polite">
                {errors.email}
              </p>
                )
              : null}
            <br/>

            <label htmlFor={IDS.passwordInput}>Password:</label>
            <input
            id={IDS.passwordInput}
            name="password"
            type="password"
            required
            aria-invalid={errors.password ? 'true' : 'false'}
            aria-describedby={errors.password ? IDS.passwordError : undefined}
            value={password}
            className='form-control'
            onChange={(e) => setPassword(e.target.value)}></input><br/>
            <button onClick={login} className='btn'>Sign in</button>
            {errors.password
              ? (
              <p id={IDS.passwordError} aria-live="polite">
                {errors.password}
              </p>
                )
              : null}
            </div>
            <hr />
            <Link to="/signup">Not yet a user? Sign up now</Link>
        </>
  );
}

export default SignIn;
