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
			lastOpen: historic.lastOpen,
			lastClose: historic.lastOpen,
			HourOpen: historic.lastOpen,
			HourClose: historic.lastOpen
		});
		const cabinetSaved = await newCabinet.save();
		return cabinetSaved;
	} catch (error) {
		throw new Error('Erro ao criar armário' + error.message);
	}
}

async function getAllCabinet() {
	try {
		const allCabinet = await Cabinet.find();
		return allCabinet;
	} catch (error) {
		throw new Error('Erro ao buscar armários' + error.message);
	}
}

async function getCabinetById(id) {
	try {
		const cabinet = await Cabinet.findById(id);
		return cabinet;
	} catch (error) {
		throw new Error('Erro ao buscar armário' + error.message);
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
