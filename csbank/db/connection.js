const mongoose = require("mongoose");

mongoose.connect("mongodb://localhost:27017/usersdb", {

}).then(()=>{
    console.log("Connection Successfull");
}).catch((err)=>{
    console.log(err);
});