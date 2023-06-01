const UserModel = require('../models/userModel');

async function createUser(name, email, password, id_tag, setor, equipe) {
	try {
		const userExist = await UserModel.findOne({ email });

		if (userExist) {
			throw new Error('User already exists');
		}

		const newUser = new UserModel({ 
			name, 
			email, 
			password, 
			id_tag,
			setor,
			equipe
		});

		const userSaved = await newUser.save();
		return userSaved;
	} catch (error) {
		throw new Error('Erro ao criar usuário' + error.message);
	}
}

async function getAllUsers() {
	try {
		const allUsers = await UserModel.find();
		return allUsers;
	} catch (error) {
		throw new Error('Erro ao buscar usuários' + error.message);
	}
}

async function getUserById(id) {
	try {
		const user = await UserModel.findById(id);
		return user;
	} catch (error) {
		throw new Error('Erro ao buscar usuário' + error.message);
	}
}

async function updateUser(id, name, email, password, id_tag, setor, equipe) {
	try {
		const updateUser = await UserModel.findByIdAndUpdate(id, {name, email, password, id_tag, setor, equipe}, {new: true});
		return updateUser;
	} catch (error) {
		throw new Error('Erro ao atualizar usuário' + error.message);
	}
}

async function deleteUser(id) {
	try {
		const userDeleted = await UserModel.findByIdAndDelete(id);
		return userDeleted;
	} catch (error) {
		console.log(error);
		throw new Error('Erro ao deletar usuário' + error.message);
	}
}


module.exports = { 
	createUser, 
	getAllUsers, 
	getUserById, 
	updateUser, 
	deleteUser 
};