const mongoose = require("mongoose");

const userSchema = new mongoose.Schema({
    firstname : {
        type: String,
        required : true
    },
    lastname : {
        type: String,
        required : true
    },
    gender : {

    },
    phone : {

    },
    address : {

    },
    city : {

    },
    pin : {

    },
    username : {
        type: String,
        required : true,
        unique : true
    },
    email : {
        type: String,
        required : true,
        unique : true
    },
    password : {

    },
    confirmpassword : {

    }
});

const User = new mongoose.model("User", userSchema);

module.exports = User;