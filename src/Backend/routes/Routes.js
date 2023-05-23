import { Router } from 'express';
import userController from '../controllers/userController.js';


const routes = new Router();

routes.get('/', (req, res) => {
	res.status(200).json({ ok: 'connected' });
});

routes.get('/users', userController.createUser);

export default routes;