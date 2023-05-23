import mongoose from 'mongoose';

const connectDB = async () => {
	try {
		await mongoose.connect(process.env.DB_URL, {
			useNewUrlParser: true,
			useUnifiedTopology: true,
		});
		console.log('MongoDb connect');
	} catch (error) {
		console.error('deu ruim demais!!');
		process.exit(1);
	}
};

module.exports = connectDB;