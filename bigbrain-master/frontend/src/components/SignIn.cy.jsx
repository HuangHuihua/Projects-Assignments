import React from 'react'
import SignIn from './SignIn'
import { render, screen } from '@testing-library/react'
// import userEvent from '@testing-library/user-event'

const noop = () => {}

describe('<SignIn />', () => {
  it('renders the email field, password field and login button', () => {
    // see: https://on.cypress.io/mounting-react
    cy.mount(<SignIn onSubmit={noop} />)
    render(<SignIn onSubmit={noop} />)
    expect(screen.getByRole('textbox', { name: /email:/i })).toBeInTheDocument()
    expect(screen.getByLabelText(/Password:/i)).toBeInTheDocument()
    expect(screen.getByRole('button', { name: /login/i })).toBeInTheDocument()
  })

  it('renders the email and password inputs as required fields', () => {
    render(<SignIn />)

    expect(screen.getByRole('textbox', { name: /Email:/i })).toBeRequired()
    expect(screen.getByLabelText(/Password:/i)).toBeRequired()
  })



})
