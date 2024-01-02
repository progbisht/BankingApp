const jwt = require("jsonwebtoken");
const User = require("../models/user-registration");

const auth = async (req, res, next) => {
    try{
        const token = req.cookies.user_login;
        const verifyUser = jwt.verify(token, "newbankuserregisterationsecuretoken");
        const userData = await User.findOne({_id : verifyUser.id});
        
        req.user = userData;
        req.token = token;
        
        next();
    }
    catch(err){
        res.status(401).send(err);
    }

    
}

module.exports = auth;