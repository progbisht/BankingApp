const mongoose = require("mongoose");
const bcrypt = require("bcryptjs");
const jwt = require("jsonwebtoken");

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
        type: String,
        required : true
    },
    phone : {
        type: Number,
        required : true,
        unique : true
    },
    address : {
        type: String,
        required : true
    },
    city : {
        type: String,
        required : true
    },
    pin : {
        type: Number,
        required : true
    },
    adhaar : {
        type: Number,
        required : true,
        unique : true
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
        type: String,
        required : true
    },
    confirmpassword : {
        type: String,
        required : true
    },
    account : {
        accountNumber:{
            type : Number,
            required : true
        },
        accountType : {
            type : String,
            required : true
        },
        balance : {
            type : Number,
            default : 0
        }
    },
    transactions : [{
        transaction : {
            username : String,
            recipientAccount: Number,
            amount: Number,
            transactionType: String,
            timestamp: Date
        }
    }],
    tokens : [{
        token : {
            type: String,
            required : true
        }
    }]
});

userSchema.methods.generateAuthToken = async function(){
    try{
        const token = jwt.sign({id:this._id.toString()},"newbankuserregisterationsecuretoken");
        this.tokens = this.tokens.concat({token});

        await this.save();

        return token;
        
    }catch(err){
        res.send(err);
    }
}

userSchema.pre("save", async function(next) {
    if(this.isModified("password")){
        this.password = await bcrypt.hash(this.password, 10);
        this.confirmpassword = await bcrypt.hash(this.confirmpassword, 10);
    }
    next();
});

const User = new mongoose.model("User", userSchema);

module.exports = User;