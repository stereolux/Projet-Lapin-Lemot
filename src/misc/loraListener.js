'use strict';
var redis = require('redis'),
	mongoose = require('mongoose'),
	conf = require('../conf/conf.js'),
	tags = require('../conf/tags.js'),
	Waypoint = require('../models/waypoint.js'),
	Gateway = require('../gateway.js');

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

// LoRa listener
var gateway = new Gateway('/dev/ttyUSB0');
gateway.on('ready', function() {
	console.log('Connected to LoRa gateway');
});
gateway.on('data', function(data) {
	console.log(data);
	var msg = data.split(';');
	var sensorId = msg[0];
	var tagId = tags[msg[1].trim()];
	client.get(tagId, function(err, value) {
		// something went wrong
		if (err) {
			throw err;
		}
		// everything seems ok
		else if (value) {
			var waypoint = new Waypoint({
				sensorId: sensorId,
				visitId: value,
				date: new Date()
			});
			waypoint.save(function (err) {
				if (err) throw err;
				console.log('Saved waypoint: ' + waypoint);
			});
		}
		// no visitId match the tagId
		else {
			throw new Error('Cannot save a waypoint if it is not bind to a visit!');
		}
	});
});
