import React from 'react'
import CopyButton from './CopyToClipboard'

describe('<CopyButton />', () => {
  it('renders', () => {
    // see: https://on.cypress.io/mounting-react
    cy.mount(<CopyButton />)
  })
})