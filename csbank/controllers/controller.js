
exports.home = (req,res) => {
	res.status(200).send("Hello Controller");
}

exports.about = (req,res) => {
	res.status(200).send("Hello Controller About");
}


