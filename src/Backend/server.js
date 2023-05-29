const express =  require('express');
const routes = require('./routes/Routes');
const connectDB = require('./db/mongoose');

const app = express();
const PORT = 3000;

require('dotenv').config();

connectDB();

app.use(express.json());

app.use(routes);


app.listen(PORT, () => {
	console.log(`server is connect in http://localhost:${PORT}`);
});

