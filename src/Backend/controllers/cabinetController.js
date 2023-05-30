const cabinetService = require('../services/cabinetService');

async function createCabinet(req, res) {
	const { number, isEmpty, lastOpen, HourOpen } = req.body;
	console.log(req.body);
	try {
		console.log(number, isEmpty, { lastOpen, HourOpen });
		const cabinetCreated = await cabinetService.createCabinet(number, isEmpty, { lastOpen, HourOpen });
		console.log(cabinetCreated);
		return res.status(201).json(cabinetCreated);
	} catch (error) {
		console.log(error);
		return res.status(500).json({ error: 'Internal server error' });
	}
}

async function getAllCabinet(req, res) {
	try {
		const allCabinet = await cabinetService.getAllCabinet();
		return res.status(200).json(allCabinet);
	} catch (error) {
		console.log(error);
		return res.status(500).json({ error: 'Internal server error' });
	}
}

async function getCabinetById(req, res) {
	const { id } = req.params;
	try {
		const cabinet = await cabinetService.getCabinetById(id);
		return res.status(200).json(cabinet);
	} catch (error) {
		console.log(error);
		return res.status(404).json({ error: 'Usuario não encontrado' });
	}
}

async function updateCabinet(req, res) {
	const { id } = req.params;
	const { number, isEmpty } = req.body;
	try {
		const cabinetUpdate = await cabinetService.updateCabinet(
			id,
			number,
			isEmpty
		);
		return res.status(200).json(cabinetUpdate);
	} catch (error) {
		console.log(error);
		return res.status(404).json({ error: 'Usuario não encontrado' });
	}
}

async function deleteCabinet(req, res) {
	const { id } = req.params;
	try {
		const cabinetDelete = await cabinetService.deleteCabinet(id);

		if (!cabinetDelete) {
			return res.status(404).json({ error: 'User not found' });
		}

		return res.status(200).json(cabinetDelete);
	} catch (error) {
		console.log(error);
		return res.status(404).json({ error: 'Internal server error' });
	}
}

module.exports = {
	createCabinet,
	getAllCabinet,
	getCabinetById,
	updateCabinet,
	deleteCabinet,
};
