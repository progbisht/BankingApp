const express = require("express");
const path = require("path");
const hbs = require("hbs");
require("../db/connection");
const User = require("../models/user-registration");
const bcrypt = require("bcryptjs");
const cookieParser = require("cookie-parser");
const auth = require("../middleware/auth");
const router = require("../router/router")

const app = express();
const port = process.env.PORT || 5000

const staticPath = path.join(__dirname,"../public");
const templatePath = path.join(__dirname, "../templates/views");
const partialsPath = path.join(__dirname, "../templates/partials");

// adding the middlewares 
// app.use('/api/auth', router);
app.set("view engine", "hbs");
app.set("views", templatePath);
app.use(express.static(staticPath));
hbs.registerPartials(partialsPath);
app.use(express.json());
app.use(express.urlencoded({extended : false}));
app.use(cookieParser());


// display home page
app.get("/", (req,res)=>{
    res.render("index", {
        validUser : false
    });
});

app.get("/dashboard", auth, (req,res)=>{
    
    res.render("dashboard", {
        user:req.user
    });
});

app.get("/transactions", auth, (req,res)=> {
    res.render("transactions", {
        user:req.user
    });
});

app.get("/profile", auth, (req,res)=> {
    res.render("profile", {
        user:req.user
    });
});


// app.get("/secret", auth, (req,res)=>{
//     console.log(req.cookies.user_login);
//     res.render("secret");
// });

// display about page
app.get("/about", (req,res)=>{
    res.render("about");
});

// display service page
app.get("/service", (req,res)=>{
    res.render("service");
});

// display team page
app.get("/team", (req,res)=>{
    res.render("team");
});

// display why page
app.get("/why", (req,res)=>{
    res.render("why");
});

// display login page
app.get("/login", (req,res)=>{
    res.render("login");
});

// logout and clear cookies
app.get("/logout", auth, async(req,res)=>{
    try{

        // single device logout
        // req.user.tokens = req.user.tokens.filter((currElem) => {
        //     return currElem.token !== req.token;
        // });

        // logout from all devices
        req.user.tokens = [];
        

        res.clearCookie("user_login");
        await req.user.save();

        console.log("Logout Successfully");

        res.render("login");
    }
    catch(err){
        res.status(500).send(err);
    }
    
});


// display register page
app.get("/register", (req,res)=>{
    res.render("register");
});

// display error page
app.get("*", (req,res)=>{
    res.render("error", {
        errorMessage : "Oops! Page Not Found",
    });
});


// method for user registration and storing user details in database 
app.post("/register", async(req,res)=>{
    try{
        const pass = req.body.password;
        const cpass = req.body.confirmpassword;

        // storing the data to database if match in passwords entered by the user
        if(pass === cpass){
            const newUser = new User({
                firstname : req.body.firstname,
                lastname : req.body.lastname,
                gender : req.body.gender,
                phone : req.body.phone,
                address : req.body.address,
                city : req.body.city,
                pin : req.body.pin,
                adhaar : req.body.adhaar,
                username : req.body.username,
                email : req.body.email,
                password : req.body.password,
                confirmpassword : req.body.confirmpassword
            });

            newUser.account.accountType = req.body.accountType;
            newUser.account.accountNumber = req.body.phone;

            

            // method for generating jsonwebtoken
            // const token = await newUser.generateAuthToken();

            // res.cookie("user_registration", token, {
            //     expires : new Date(Date.now() + 300000),
            //     httpOnly : true
            // });

            const userCreated = await newUser.save();
            console.log(userCreated);
            res.redirect("http://localhost:5000/login");
            // res.status(201).send("New User Created Successfully. Log in Now");
        
        }
        else{
            res.status(400).send("Password are not matching");
        }
    }catch(e){
        res.status(400).send(e);
    }
});

// method for checking valid user
app.post("/login", async (req,res) => {
    try {
        const uname = req.body.username;
        const pass = req.body.password;

        // check for a valid user for login
        const validUname = await User.findOne({username:uname});
        const isValid = await bcrypt.compare(pass, validUname.password);
        
    
        if(isValid){

            const token = await validUname.generateAuthToken();
            res.cookie("user_login", token, {
                // expires : new Date(Date.now() + 300000),
                httpOnly : true
            });

            console.log(validUname.tokens.token);

            res.render("dashboard", {
                user : validUname
            });
        }
        else{
            res.status(400).send("Invalid Login Credentials")
        }
    }
    catch(e){
        res.status(400).send("Invalid Login Credentials");
    }
})

app.post("/transactions", auth, async(req,res) => {
    
    try{
        const uname = req.body.username;
        const recipientAccount = req.body.recipientAccount;
        const amount = Number(req.body.amount);
        const transactionType = req.body.transactionType; 
        const timestamp = new Date();
       
       
        if(uname === req.user.username && Number(recipientAccount) === req.user.account.accountNumber){
            
            if(transactionType === "Deposit"){
                req.user.account.balance += amount;
            }
            else{
                if(req.user.account.balance - amount > 0){
                    req.user.account.balance -= amount;
                }
                else{
                    res.send("Insufficient Balance");
                }                
            }

            userTrans = {
                username : uname,
                recipientAccount: recipientAccount,
                amount: amount,
                transactionType: transactionType,
                timestamp: timestamp
            }

            req.user.transactions = req.user.transactions.concat({transaction : userTrans});

            await req.user.save();

            res.status(201).render("dashboard", {
                user : req.user
            });
        }
        else{
            res.send("Please Check the Credentials");
        }
    }
    catch(err){
        console.log(err);
    }
    

    
});


app.listen(port, ()=>{
    console.log(`listening to the port number ${port}`);
});
