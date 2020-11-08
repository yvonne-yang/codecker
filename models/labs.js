var mongoose = require("mongoose");

var labSchema = new mongoose.Schema({
  name: String,
  downloadLink: String,
  correctOutputFile: String,
  created: {
    type: Date,
    default: Date.now
  }
});
module.exports = mongoose.model("Lab", labSchema);