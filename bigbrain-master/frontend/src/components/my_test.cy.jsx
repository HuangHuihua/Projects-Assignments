context('Register - happy path', () => {
    beforeEach(() => {
        cy.visit('localhost:3000');
    });

    it('Successfully sign up', () => {
		// Log in with the correct credentials
        const name = 'hhh';
		const email = 'hhh@gmail.com';
		const password = 'super secure password';
		const welcomeText = "You're logged in!";

        cy.get('input[name=name]').focus().type(name);
		cy.get('input[name=email]').focus().type(email);
		cy.get('input[name=password]').focus().type(password);
		// cy.get('button[type=submit]').click();

		// Check that it is successful
		cy.get('h1').then((h1) => {
			expect(h1.text()).to.contain(welcomeText);
		});

    })
})