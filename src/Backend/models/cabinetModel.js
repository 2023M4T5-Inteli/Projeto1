const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const cabinetSchema = new Schema({
	number: {
		type: Number,
		required: true
	},

	isEmpty: {
		type: Boolean,
		required: true
	},

	historic: {
		lastOpen: {
			type: String,
			required: true
		},

		HourOpen: {
			type: Date,
			default: new Date(),
			required: true
		},


	}
});

const Cabinet = mongoose.model('Cabinet', cabinetSchema);

module.exports = Cabinet;
