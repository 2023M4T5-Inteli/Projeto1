const UserService = require('./userService');

async function createUser(req, res) {
	const { name, email, password, id_tag } = req.body;
 
	try {
		const userCreated = await UserService.createUser(name, email, password, id_tag);
		return res.status(201).json(userCreated);
	} catch (error) {
		console.log(error);
		return res.status(500).json({ error: 'Internal server error' });
	}
}

module.exports = {
	createUser,
};