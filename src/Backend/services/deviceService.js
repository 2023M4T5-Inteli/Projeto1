const Device = require('../models/deviceModel');

async function createDevice(model, marca, nota_fiscal, data_compra, periodo_leasing, numero_rastreador, numero_de_serie) {
	try {
		const deviceExist = await Device.findOne({ numero_de_serie });

		if (deviceExist) {
			throw new Error('Device already exists');
		}

		const newDevice = new Device({ 
			model, 
			marca,
			nota_fiscal, 
			data_compra,
			periodo_leasing,
			numero_rastreador,
			numero_de_serie
		});

		const deviceSaved = await newDevice.save();
		return deviceSaved;
	} catch (error) {
		throw new Error('Erro ao criar dispositivo' + error.message);
	}
}

async function getAllDevices() {
	try {
		const allDevices = await Device.find();
		return allDevices;
	} catch (error) {
		throw new Error('Erro ao buscar dispositivos' + error.message);
	}
}

async function getDeviceById(id) {
	try {
		const device = await Device.findById(id);
		return device;
	} catch (error) {
		throw new Error('Erro ao buscar dispositivo' + error.message);
	}
}

async function updateDevice(id, model, marca, nota_fiscal, data_compra, periodo_leasing, numero_rastreador, numero_de_serie) {
	try {
		const updateDevice = await Device.findByIdAndUpdate(id, { model, marca, nota_fiscal, data_compra, periodo_leasing, numero_rastreador, numero_de_serie }, {new: true});
		return updateDevice;
	} catch (error) {
		throw new Error('Erro ao atualizar dispositivo' + error.message);
	}
}

async function deleteDevice(id) {
	try {
		const deviceDeleted = await Device.findByIdAndDelete(id);
		return deviceDeleted;
	} catch (error) {
		throw new Error('Erro ao deletar dispositivo' + error.message);
	}
}

module.exports = { 
	createDevice,
	getAllDevices,
	getDeviceById,
	updateDevice,
	deleteDevice
};