const mongoose = require("mongoose");

const userSchema = mongoose.Schema({
  name: {
    type: String,
    required: true,
  },

  id_tag: {
    type: String,
    unique: true,
    required: true,
  },
});

const User = mongoose.model('User', userSchema)

module.exports = User
