const Router =  require('express');
const userController = require('../controllers/userController');
const deviceController = require('../controllers/deviceController');
const cabinetController = require('../controllers/cabinetController');


const routes = new Router();

routes.get('/', (req, res) => {
	res.status(200).json({ ok: 'connected' });
});

// Rota para criar um usuário
routes.post('/users', userController.createUser);

// Rota para obter todos os usuários
routes.get('/users', userController.getAllUsers);

// Rota para obter um usuário pelo id
routes.get('/users/:id', userController.getUserById);

// Rota para deletar um usuário pelo id
routes.delete('/users/:id', userController.deleteUser);

// Rota para atualizar um usuário pelo id
routes.put('/users/:id', userController.updateUser);

// Rota para criar um dispositivo
routes.post('/devices', deviceController.createDevice);

// Rota para obter todos os dispositivos
routes.get('/devices', deviceController.getAllDevices);

// Rota para obter um dispositivo pelo id
routes.get('/devices/:id', deviceController.getDeviceById);

// Rota para deletar um dispositivo pelo id
routes.delete('/devices/:id', deviceController.deleteDevice);

// Rota para atualizar um dispositivo pelo id
routes.put('/devices/:id', deviceController.updateDevice);


routes.post('/cabinet', cabinetController.createCabinet);

// Rota para obter todos os usuários
routes.get('/cabinet', cabinetController.getAllCabinet);

// Rota para obter um usuário pelo id
routes.get('/cabinet/:id', cabinetController.getCabinetById);

// Rota para deletar um usuário pelo id
routes.delete('/cabinet/:id', cabinetController.updateCabinet);

// Rota para atualizar um usuário pelo id
routes.put('/cabinet/:id', cabinetController.deleteCabinet);

module.exports = routes;