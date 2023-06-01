const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const userSchema = new Schema({
	name: {
		type: String,
		required: true
	},
	email: {
		type: String,
		unique: true,
		required: true
	},
	password: {
		type: String,
		required: true
	},
	id_tag: {
		type: String,
		required: true
	},
	setor: {
		type: String,
		required: true
	},
	equipe: {
		type: String,
		required: true
	},
});

const UserModel = mongoose.model('Users', userSchema);

module.exports = UserModel;