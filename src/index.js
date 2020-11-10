require('dotenv').config();
const express = require('express');
const app = express();
const mongoose = require('mongoose');
const bodyParser = require('body-parser');
const Lab = require('./models/labs.js');
const Course = require('./models/courses.js');
const mongoLogin = require('./mongodbLogin.js');
const expressSanitizer = require('express-sanitizer');
const methodOverride = require('method-override');
app.use(express.static("public"));

mongoose.set("useFindAndModify", false);

mongoose
  .connect('mongodb+srv://jintaohuang:' + mongoLogin.password + '@cluster0.rvtll.mongodb.net/codecker?retryWrites=true&w=majority', {
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

app.use(
  require('express-session')({
    secret: process.env.APP_SECRET,
    resave: true,
    saveUninitialized: false
  })
);

const {
  ExpressOIDC
} = require('@okta/oidc-middleware');
const oidc = new ExpressOIDC({
  appBaseUrl: process.env.HOST_URL,
  issuer: `${process.env.OKTA_ORG_URL}/oauth2/default`,
  client_id: process.env.OKTA_CLIENT_ID,
  client_secret: process.env.OKTA_CLIENT_SECRET,
  redirect_uri: `${process.env.HOST_URL}/callback`,
  scope: 'openid profile',
  routes: {
    loginCallback: {
      path: '/callback'
    },
  }
});

app.use(oidc.router);
app.use('/register', require('./routes/register'));

app.get('/', (req, res) => {
  res.redirect("courses");
});
//logout
app.get("/logout", (req, res) => {
  req.logout();
  res.redirect("/");
});
//show page
app.get("/courses", (req, res) => {
  Course.find({}, (err, allCourses) => {
    if (err) {
      console.log(err);
    } else {
      const {
        userContext
      } = req;
      // console.log(userContext);
      Course.find({}).sort({
        name: 'asc'
      }).exec(function (err, sortedCourses) {
        res.render("landing", {
          courses: sortedCourses,
          userContext: userContext
        });
      });

    }
  })
})
app.get("/courses/new", (req, res) => {
  if (typeof req.userContext === 'undefined') {
    req.userContext = false;
  }
  const userContext = req.userContext;
  res.render("newcourse", {
    userContext: userContext
  });
});
app.post("/courses", (req, res) => {
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
});
//show

app.get("/courses/:id", (req, res) => {
  //  console.log(req.userContext);
  Course.findById(req.params.id, (err, foundCourse) => {
    if (err) {
      redirect("/courses");
    } else {

      if (typeof req.userContext === 'undefined') {
        req.userContext = false;
      }
      const userContext = req.userContext;

      res.render("examplecoursepage", {
        course: foundCourse,
        userContext: userContext
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
//edit course
app.get("/courses/:id/edit", (req, res) => {
  if (typeof req.userContext === 'undefined') {
    req.userContext = false;
  }
  const userContext = req.userContext;
  Course.findById(req.params.id, (err, foundCourse) => {
    if (err) {
      res.redirect("/courses");
    } else {
      res.render("editCourse.ejs", {
        course: foundCourse,
        user: userContext
      });
    }
  });
});
//update
app.put("/courses/:id", (req, res) => {
  Course.findByIdAndUpdate(req.params.id, req.body.course, (err, updatedCourse) => {
    if (err) {
      res.redirect("/courses");
      console.log(err);
    } else {
      res.redirect("/courses/" + req.params.id);
    }
  });
});
//new lab
app.get("/courses/:id/new", (req, res) => {
  if (typeof req.userContext === 'undefined') {
    req.userContext = false;
  }
  const userContext = req.userContext;
  Course.findById(req.params.id, (err, foundCourse) => {
    if (err) {
      console.log("errorrrrrr");
      res.redirect("courses/" + req.params.id);
    } else {
      res.render("newlab", {
        course: foundCourse,
        user: userContext
      });
    }
  })
});
//create lab
app.post("/courses/:id", (req, res) => {
  Course.findById(req.params.id, (err, foundCourse) => {
    if (err) {
      res.redirect("courses/" + req.params.id + "/new");
    } else {
      // console.log(foundCourse);
      foundCourse.labs.push(req.body.lab);
      foundCourse.save(function (err) {
        if (err) return handleError(err)
        res.redirect("/courses/" + req.params.id);
      });
    }
  })
});
//show lab
app.get("/courses/:id/:labID", (req, res) => {
  if (typeof req.userContext === 'undefined') {
    req.userContext = false;
  }
  const userContext = req.userContext;
  Course.findById(req.params.id, (err, foundCourse) => {
    if (err) {
      res.redirect("/courses");
    } else {
      Lab.findById(req.params.labID, (err, foundLab) => {
        if (err) {
          res.redirect("/courses/" + req.params.id);
        } else {
          res.render("labpage", {
            lab: foundLab,
            course: foundCourse,
            labID: req.params.labID,
            user: userContext
          });
        }
      });
    }
  });
});
//edit lab
app.get("/courses/:id/:labID/edit", (req, res) => {
    if (typeof req.userContext === 'undefined') {
    req.userContext = false;
  }
  const userContext = req.userContext;
  Course.findById(req.params.id, (err, foundCourse) => {
    if (err) {
      res.redirect("/courses");
    } else {
      Lab.findById(req.params.labID, (err, foundLab) => {
        if (err) {
          res.redirect("/courses/" + req.params.id + '/' + req.params.labID);
        } else {
          res.render("labedit", {
            lab: foundLab,
            course: foundCourse,
            labID: req.params.labID,
            user: userContext
          });
        }
      });
    }
  });
});
// update lab
app.put("/courses/:id/:labID", (req, res) => {
  Course.findOneAndUpdate(

    {
      "_id": req.params.id,
      "labs._id": req.params.labID
    }, {
      "$set": {
        "labs.$": req.body.updatedlab
      }
    },
    function (err, lab) {
      if (err) {
        console.log(err);
        res.redirect("/courses/" + req.params.id + '/' + req.params.labID);
      } else {
        Course.findById(req.params.id, (err, course) => {
          if (err) {
            console.log(err);
          } else {
            console.log(course);
            Lab.findById(req.params.labID, (err, foundLab) => {
              if (err) {
                res.redirect("/courses");
              } else {
                res.redirect("/courses/" + req.params.id);
              }
            });

          }
        })

      }
    }
  );
});
//delete lab
app.delete("/courses/:id/:labID", (req, res) => {
  Course.findById(req.params.id, (err, course) => {
    if (err) {
      console.log(err);
    } else {
      course.labs.id(req.params.labID).remove();
      course.save(function (err) {
        if (err) return handleError(err);
        console.log('the lab was removed');
      });
      res.redirect("/courses/" + req.params.id);
    }
  })
})



app.listen(3000, () => {
  console.log("Codecker Server has started");
})