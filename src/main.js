'use strict';
var express = require('express'),
	app = express(),
	server = require('http').Server(app),
	redis = require('redis'),
	bodyParser = require('body-parser'),
	compression = require('compression'),
	mongoose = require('mongoose'),
	Waypoint = require('./models/waypoint'),
	Routes = require('./routes/routes'),
	conf = require('./conf/conf');

// connect to mongo
mongoose.connect(conf.MONGO_URL);
var db = mongoose.connection;
db.on('error', console.error.bind(console, 'mongo error:'));
db.once('open', function() {
	console.log('Connected to mongo');
});

// connect to redis
var client = redis.createClient(conf.REDIS_URL);
client.on('error', console.error.bind(console, 'redis error:'));
client.once('ready', function() {
	console.log('Connected to redis');
});

// app conf
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({
	extended: true
}));
app.use(compression());
app.use(express.static(__dirname + '/../www/'));

// routes configuration
var routes = new Routes(Waypoint, client),
	router = express.Router();
app.use(conf.API_PREFIX, router);
router.get('/visit', routes.getAllVisits);
router.get('/visit/:tagId', routes.getVisit);
router.post('/visit', routes.createVisit);

// start the server
server.listen(conf.PORT, function () {
	var host = server.address().address;
	var port = server.address().port;
	console.log('Serveur Lapin Lemot listening at ' + conf.APP_URL + ':' + conf.PORT);
});

// LoRa listener
// var gateway = new Gateway('/dev/ttyUSB0');
// gateway.on('ready', function() {
// 	console.log('Connected to LoRa gateway');
// });
// gateway.on('data', function(data) {
// 	var msg = data.split(';');
// 	var sensorId = msg[0];
// 	var tagId = msg[1];
// 	client.get(tagId, function(err, value) {
// 		// something went wrong
// 		if (err) {
// 			throw err;
// 		}
// 		// everything seems ok
// 		else if (value) {
// 			var waypoint = new Waypoint({
// 				sensorId: sensorId,
// 				visitId: value,
// 				date: new Date()
// 			});
// 			waypoint.save(function (err) {
// 				if (err) throw err;
// 				console.log('Saved waypoint: ' + waypoint);
// 			});
// 		}
// 		// no visitId match the tagId
// 		else {
// 			throw new Error('Cannot save a waypoint if it is not bind to a visit!');
// 		}
// 	});
// });
