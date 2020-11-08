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
var courseSchema = new mongoose.Schema({
  name: String,
  description: String,
  labs: [labSchema]
});

module.exports = mongoose.model("Course", courseSchema);