const Cabinet = require('../models/cabinetModel');

async function createCabinet(number, isEmpty, historic) {
	try {
		const cabinetExist = await Cabinet.findOne({ number });
		if (cabinetExist) {
			throw new Error('Este numero de armário já existe');
		}

		const newCabinet = new Cabinet({
			number,
			isEmpty,
			historic: {
				lastOpen: historic.lastOpen,
				HourOpen: historic.HourOpen
			}
		});
		const cabinetSaved = await newCabinet.save();
		return cabinetSaved;
	} catch (error) {
		throw new Error('Erro ao criar armário: ' + error);
	}
}

async function getAllCabinet() {
	try {
		const allCabinet = await Cabinet.find();
		return allCabinet;
	} catch (error) {
		throw new Error('Erro ao buscar armários' + error.menssage);
	}
}

async function getCabinetById(id) {
	try {
		const cabinet = await Cabinet.findById(id);
		return cabinet;
	} catch (error) {
		throw new Error('Erro ao buscar armário' + error.menssage);
	}
}

async function updateCabinet(id, number, isEmpty) {
	try {
		const updateCabinet = await Cabinet.findByIdAndUpdate(
			id,
			{ number, isEmpty },
		);
		return updateCabinet;
	} catch (error) {
		throw new Error('Erro ao atualizar armário' + error.message);
	}
}

async function deleteCabinet(id) {
	try {
		const deleteCabinet = await Cabinet.findByIdAndDelete(id);
		return deleteCabinet;
	} catch (error) {
		throw new Error('Erro ao deletar armário' + error.message);
	}
}

module.exports = {
	createCabinet,
	getAllCabinet,
	getCabinetById,
	updateCabinet,
	deleteCabinet,
};
