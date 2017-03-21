'use strict';
var util = require('util'),
	EventEmitter = require('events').EventEmitter,
	serialport = require('serialport'),
	SerialPort = serialport.SerialPort;

var Gateway = function(path) {
	EventEmitter.call(this);
	this.sp = new SerialPort(path, {
		baudrate: 38400,
		parser: serialport.parsers.readline('#')
	});

	var _self = this;
	var readyMsg = new Buffer([0xe2, 0x34]).toString('utf8');
	_self.sp.on('open',function() {
		console.log('open');
		_self.sp.on('data', function(data) {
			console.log(data);
			if (data.indexOf(readyMsg) > -1) {
				data = data.replace(readyMsg,'');
				_self.emit('ready');
			}
			_self.emit('data', data.trim());
		});
	});
};
util.inherits(Gateway, EventEmitter);

module.exports = Gateway;
