const User = require('../models/userModel');

async function createUser(name, email, password, id_tag) {
	try {
		const userExist = await User.findOne({ email });

		if (userExist) {
			throw new Error('User already exists');
		}

		const newUser = new User({ 
			name, 
			email, 
			password, 
			id_tag 
		});

		const userSaved = await newUser.save();
		return userSaved;
	} catch (error) {
		throw new Error('Erro ao criar usuário' + error.message);
	}
}

module.exports = {createUser};