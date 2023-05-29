const DeviceService = require('../services/deviceService');

async function createDevice(req, res) {
	const { model, marca, nota_fiscal, data_compra, periodo_leasing, numero_rastreador, numero_de_serie } = req.body;

	try {
		const deviceCreated = await DeviceService.createDevice(model, marca, nota_fiscal, data_compra, periodo_leasing, numero_rastreador, numero_de_serie);
		return res.status(201).json(deviceCreated);
	} catch (error) {
		console.log(error);
		return res.status(500).json({
			error: 'Internal server error'
		});
	}
}

async function getAllDevices(req, res) {
	try {
		const allDevices = await DeviceService.getAllDevices();
		return res.status(200).json(allDevices);
	} catch (error) {
		console.log(error);
		return res.status(500).json({
			error: 'Internal server error'
		});
	}
}

async function getDeviceById(req, res) {
	const { id } = req.params;
  
	try {
		const device = await DeviceService.getDeviceById(id);

		if (!device) {
			return res.status(404).json({
				error: 'Device not found'
			});
		}

		return res.status(200).json(device);
	} catch (error) {
		console.log(error);
		return res.status(500).json({
			error: 'Internal server error'
		});
	}
}

async function updateDevice(req, res) {
	const { id } = req.params;
	const { model, marca, nota_fiscal, data_compra, periodo_leasing, numero_rastreador, numero_de_serie } = req.body;

	try {
		const deviceUpdated = await DeviceService.updateDevice(id, model, marca, nota_fiscal, data_compra, periodo_leasing, numero_rastreador, numero_de_serie);

		if (!deviceUpdated) {
			return res.status(404).json({
				error: 'Device not found'
			});
		}

		return res.status(200).json(deviceUpdated);
	} catch (error) {
		console.log(error);
		return res.status(500).json({
			error: 'Internal server error'
		});
	}
}

async function deleteDevice(req, res) {
	const { id } = req.params;

	try {
		const deviceDeleted = await DeviceService.deleteDevice(id);

		if (!deviceDeleted) {
			return res.status(404).json({
				error: 'Device not found'
			});
		}

		return res.status(200).json(deviceDeleted);
	} catch (error) {
		console.log(error);
		return res.status(500).json({
			error: 'Internal server error'
		});
	}
}


module.exports = { 
	createDevice, 
	getAllDevices, 
	getDeviceById, 
	deleteDevice, 
	updateDevice 
};