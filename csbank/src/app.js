
const User = require("./models/register");
require("./db/connection");
const express = require("express");
const app = express();
const port = process.env.PORT || 3000

app.use(express.json());

app.get("/", (req,res)=>{
    res.send("Hello World");
})
app.post("/users", async(req,res)=>{
    try{
        const newUser = new User(req.body)
        const userCreated = await newUser.save();
        res.status(200).send(userCreated);
    }catch(e){
        res.status(400).send(e);
    }
});

app.get("/users", async(req,res)=>{
    try{
        const userData = await User.find();
        res.status(200).send(userData);
    }catch(e){
        res.status(400).send(e);
    }
});

app.patch("/users/:id", async(req,res)=>{
    try{
        const userId = req.params.id;
        const updatedUser = await User.findByIdAndUpdate(userId, req.body, {
            new:true
        });
        res.send(updatedUser);
    }catch(e){
        res.send(e);
    }
});


app.delete("/users/:id", async(req,res)=>{
    try{
        const userId = req.params.id;
        const deletedUser = await User.findByIdAndDelete(userId);
        res.send(deletedUser);
    }catch(e){
        res.send(e);
    }
})

app.listen(port,()=>{
    console.log(`Connection established at ${port}`)
});
