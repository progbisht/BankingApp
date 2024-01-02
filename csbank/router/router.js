const express = require('express');
const router = express.Router();
const controller = require('../controllers/controller');

router.route('/').get(controller.home);

router.route('/about').get(controller.about);


// app.get("/", (req,res)=>{
//     res.render("index");
// });

// app.get("/dashboard", auth, (req,res)=>{
//     res.render("dashboard");
// });

// app.get("/account", auth, (req,res)=> {
//     res.render("account")
// });

// app.get("/transactions", auth, (req,res)=> {
//     res.render("transactions")
// });

// app.get("/profile", auth, (req,res)=> {
//     res.render("profile")
// });


// app.get("/secret", auth, (req,res)=>{
//     console.log(req.cookies.user_login);
//     res.render("secret");
// });

// // display about page
// app.get("/about", (req,res)=>{
//     res.render("about");
// });

// // display service page
// app.get("/service", (req,res)=>{
//     res.render("service");
// });

// // display team page
// app.get("/team", (req,res)=>{
//     res.render("team");
// });

// // display why page
// app.get("/why", (req,res)=>{
//     res.render("why");
// });

// // display login page
// app.get("/login", (req,res)=>{
//     res.render("login");
// });


// module.exports = router;

