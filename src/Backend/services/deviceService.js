const Device = require('../models/deviceModel');
const mqtt = require('mqtt');

function connectToMQTT() {
	const client = mqtt.connect('mqtt://mqtt-broker-url'); // colocar o endereço do broker

	client.on('connect', function () {
		console.log('Connected to MQTT Broker');
		client.subscribe('localizacoes');
	});

	client.on('message', async function (topic, message) {
		if (topic === 'localizacoes') {
			const localizacoes = JSON.parse(message.toString());
			console.log('Recived new locations: ', localizacoes);

			try {
				const deviceId = localizacoes.deviceId;

				const device = await Device.findById(deviceId);

				if (!device) {
					throw new Error('Device not found', deviceId);
				}

				device.localizacoes.push(...localizacoes.localizacoes);
				await device.save();

				console.log('Locations added to device: ', deviceId);
			} catch (error) {
				console.log('Error adding locations', error);
			}
		}
	});
}

connectToMQTT();

async function createDevice(model, marca, nota_fiscal, data_compra, periodo_leasing, numero_de_serie, localizacoes) {
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
			numero_de_serie,
			localizacoes: [
				{
					setor: localizacoes.setor,
					data: localizacoes.data
				}
			]
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

async function updateDevice(id, model, marca, nota_fiscal, data_compra, periodo_leasing, numero_de_serie, localizacoes) {
	try {
		const updateDevice = await Device.findByIdAndUpdate(id, { 
			model, 
			marca, 
			nota_fiscal, 
			data_compra, 
			periodo_leasing, 
			numero_de_serie,
			$push: { localizacoes: { $each: localizacoes } } }, {new: true});
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