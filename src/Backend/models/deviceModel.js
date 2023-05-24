const mongoose = require('mongoose');
const Schema = mongoose.Schema;

const deviceSchema = new Schema({
	model: {type: String, required: true},
	marca: { type: String, required: true},
	nota_fiscal: { type: String, required: true},
	data_compra: { type: String, required: true },
	periodo_leasing: { type: String, required: true },
	numero_rastreador: { type: String, required: true },
	numero_de_serie: { type: String, required: true },
});

const Device = mongoose.model('Device', deviceSchema);

module.exports = Device;