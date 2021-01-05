//Type "npm install" to install required node package
//index.ejs is in the views folder

//required modules
var express = require('express'); //express framework
var bodyParser = require('body-parser'); //parse the body 
var router = express.Router(); //router for routing

//create express app
var app = express();

app.set('port', process.env.PORT || 8080); // set the port of the web server 8080 or environment port
app.set('view engine', 'ejs'); // use ejs as the view engine
app.use(bodyParser.json()); 
app.use(bodyParser.urlencoded({extended:true}));

var gasSensor = 0;

//mount router middleware
app.use('/', router);

//when the users send GET request, the respond is rendering the index page with the gas value passed into the index 
router.get('/', function (req, res) {
    res.render('index', {gasValue: gasSensor});
});

//when the users send POST request, the web server will take the gas value in the body of the request
router.post('/', function(req,res){
        console.log(req.body);
        gasSensor = req.body.gasSensor;
        res.render('index', {gasValue: gasSensor});
});


//app listens to the configured port.
app.listen(app.get('port'), function () {
    console.log(`Server is running on ${app.get('port')}`);
});

module.exports = app;
