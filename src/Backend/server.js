const express =  require('express');
const app = express();
const port = 3000;
const User = require('./models/user');
app.use(express.json()); // undefined


require('dotenv').config();
require('./db/mongoose')();

app.post('/create', async (req, res) => {
	try {
		console.log(req.body);
		const user = new User(req.body);
		await user.save();
		res.send();
	} catch (error) {
		console.log(error);
		res.status(500).send();
	}
});

app.get('/users', async (req, res) => {
	try {
		const allUsers = await User.find();
		res.send(allUsers);
	} catch (error) {
		console.log(error);
		res.status(500).send();
	}
});

app.delete('/users/:id', async (req, res) => {
	try {
		const deletedUser = await User.findByIdAndDelete(req.params.id);
		res.send(deletedUser);
	} catch (error) {
		console.log(error);
		res.status(500).send();
	}
});


app.listen(port, () => {
	console.log(`server is connect in http://localhost:${port}`);
});

