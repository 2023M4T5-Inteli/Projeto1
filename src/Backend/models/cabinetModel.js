const mongoose = require('mongoose');
const Schema = mongoose.Schema;
const date = require('date-and-time');
  

const cabinetSchema = new Schema({
	number: {
		type: Number,
		required: true,
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

		lastClose: {
			type: String,
			required: true
		},

		HourOpen: {
			type: Date,
			required: true,
			default: () => date.format(new Date(),'YYYY/MM/DD HH:mm:ss')
		},
        
		HourClose: {
			type: Date,
			required: true,
			default: () => date.format(new Date(),'YYYY/MM/DD HH:mm:ss')
		}
	}
});


const Cabinet = mongoose.model('Cabinet', cabinetSchema);

module.exports = Cabinet;
