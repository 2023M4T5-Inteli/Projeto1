const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const cabinetSchema = new Schema({
	number: {
		type: Number,
	},

	isEmpty: {
		type: Boolean
	},

	historic: {
		lastOpen: String, //guardar id
		lastClose: String, //guardar id
		HourOpen: Date,
		HourClose: Date,
	}


});

const Cabinet = mongoose.model('Cabinet', cabinetSchema);

module.exports = Cabinet;
