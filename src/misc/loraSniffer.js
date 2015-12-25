'use strict';
var Gateway = require('../gateway.js');

// LoRa listener
var gateway = new Gateway('/dev/ttyUSB0');
gateway.on('ready', function() {
	console.log('Connected to LoRa gateway');
});
gateway.on('data', function(data) {
	console.log(data);
});
