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

const User = mongoose.model('User', userSchema);

module.exports = User;