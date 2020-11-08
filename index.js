const express = require('express');
const app = express();
const mongoose = require('mongoose');
const bodyParser = require('body-parser');
const Lab = require('./models/labs.js');
const Course = require('./models/courses.js');

const expressSanitizer = require('express-sanitizer');
const methodOverride = require('method-override');
app.use(express.static("public"));
mongoose.set("useFindAndModify", false);

mongoose
  .connect("mongodb://localhost:27017/newhacks2020", {
    useNewUrlParser: true,
    useUnifiedTopology: true,
  })
  .then(() => console.log("Connected to DB!"))
  .catch((error) => console.log(error.message));


app.use(bodyParser.urlencoded({
  extended: true
}));
app.use(methodOverride("_method"));
app.use(expressSanitizer());
app.set("view engine", "ejs");

// Course.create({
//     name: "ECE297",
//     description: "Second year course for ECE students, C++.",
//     labs: [
//       {
//         name: "Lab1",
//         downloadLink: "www.google.com",
//         correctOutputFile: "1234"
//       },{
//         name: "Lab2",
//         downloadLink: "www.google.com",
//         correctOutputFile: "1234"
//       }
//     ]
//   },(err, course) => {
//   if (err) {
//     console.log(err);
//   } else {
//      console.log("New course created");
//      console.log(course);
//   }
// });

app.get('/', (req, res) => {
  res.redirect("/courses");
});
app.get("/courses", (req, res) => {
  Course.find({}, (err, allCourses) => {
    if (err) {
      console.log(err);
    } else {
      res.render("landing", {
        courses: allCourses
      });
    }
  })
})
app.get("/courses/new", (req, res) => {
  res.render("newcourse");
});
app.post("/courses", (req, res) => {
  // req.body.course.body = req.sanitize(req.body.course.body);
  const name = req.body.name;
  const description = req.body.description;
  const newCourse = {
    name: name,
    description: description
  }
  Course.create(newCourse, (err, newlyCreated) => {
    if (err) {
      console.log(err);
    } else {
       res.redirect("/courses");
    }
  });
  // Course.create(req.body.blog, (err, newCourse) => {
  //   if (err) {
  //     res.render("new");
  //   } else {
  //     res.redirect("/courses");
  //   }
  // });
});
//show
app.get("/courses/:id", (req, res) => {
  Course.findById(req.params.id, (err, foundCourse) => {
    if (err) {
      redirect("/courses");
    } else {
      res.render("examplecoursepage", {
        course: foundCourse
      });
    }
  });
});
app.delete("/courses/:id", (req, res) => {
  //destroy course
  Course.findByIdAndRemove(req.params.id, (err) => {
    if (err) {
      console.log(err);
      res.redirect("/courses");
    } else {
      res.redirect("/courses");
    }
  });
});
//show?
app.get("/courses/:id/:labID", (req, res) => {
  Course.findById(req.params.id, (err, foundCourse) => {
    if (err) {
      redirect("/courses");
    } else {
      Lab.findById(req.params.labID, (err, foundLab) => {
        if (err) {
          redirect("/courses" + req.params.id);
        } else {
          res.render("labs", {lab: foundLab});
        }
      });
    }
  });
});
//new lab
app.get("/courses/:id/new", (req, res) => {
  Course.findById(req.params.id, (err, foundCourse) => {
    if(err){
      res.redirect("courses/"+req.params.id);
    }else{
      res.render("newLab", {course: foundCourse});
    }
  })
});
//create lab
app.post("/courses/:id", (req, res) => {
  req.body.course.body = req.sanitize(req.body.course.body);
  Course.create(req.body.course, (err, newLab) => {
    if (err) {
      res.render("newLab");
    } else {
      res.redirect("/courses/"+req.params.id);
    }
  });
});


app.listen(3000, () => {
  console.log("YelpCamp Server has started");
})