const UserService = require('./userService');

async function createUser(req, res) {
	const { name, email, password, id_tag, setor, equipe } = req.body;
 
	try {
		const userCreated = await UserService.createUser(name, email, password, id_tag, setor, equipe);
		return res.status(201).json(userCreated);
	} catch (error) {
		console.log(error);
		return res.status(500).json({ error: 'Internal server error' });
	}
}

async function getAllUsers(req, res) {
	try {
		const allUsers = await UserService.getAllUsers();
		return res.status(200).json(allUsers);
	} catch (error) {
		console.log(error);
		return res.status(500).json({ error: 'Internal server error' });
	}
}

async function getUserById(req, res) {
	const { id } = req.params;

	try {
		const user = await UserService.getUserById(id);

		if (!user) {
			return res.status(404).json({ error: 'User not found' });
		}

		return res.status(200).json(user);
	} catch (error) {
		console.log(error);
		return res.status(500).json({ error: 'Internal server error' });
	}
}

async function updateUser(req, res) {
	const { id } = req.params;
	const { name, email, password, id_tag, setor, equipe } = req.body;

	try {
		const userUpdated = await UserService.updateUser(id, name, email, password, id_tag, setor, equipe);

		if (!userUpdated) {
			return res.status(404).json({ error: 'User not found' });
		}

		return res.status(200).json(userUpdated);
	} catch (error) {
		console.log(error);
		return res.status(500).json({ error: 'Internal server error' });
	}
}

async function deleteUser(req, res) {
	const { id } = req.params;

	try {
		const userDeleted = await UserService.deleteUser(id);

		if (!userDeleted) {
			return res.status(404).json({ error: 'User not found' });
		}

		return res.status(200).json(userDeleted);
	} catch (error) {
		console.log(error);
		return res.status(500).json({ error: 'Internal server error' });
	}
}

module.exports = {
	createUser,
	getAllUsers,
	getUserById,
	deleteUser,
	updateUser
};