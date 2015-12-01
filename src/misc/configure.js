'use strict';
var inquirer = require('inquirer'),
	SerialPort = require('serialport').SerialPort,
	crc = require('crc');

var serialPort = new SerialPort('/dev/ttyUSB0', {
	baudrate: 38400
});
var readyBuffer = new Buffer([0xe2, 0x34]);
var chunks = '';

console.log('Hi, welcome to LoRa Gateway configurator');

var questions = [
	{
		type: 'list',
		name: 'freq',
		message: 'Frequency (CH_X_Y)',
		choices: [ '868 MHz', '900 MHz' ],
		filter: function(value) { return parseInt(value); }
	},
	{
		type: 'list',
		name: 'channel',
		message: 'Channel (CH_X_868)',
		choices: [ '10', '11', '12', '13', '14', '15', '16', '17' ],
		when: function (answers) {
			return answers.freq === 868;
		},
		filter: Number
	},
	{
		type: 'list',
		name: 'channel',
		message: 'Channel (CH_X_900)',
		choices: [ '00', '01', '02', '03', '04', '05', '06', '07', '08', '09', '10', '11', '12' ],
		when: function (answers) {
			return answers.freq === 900;
		},
		filter: Number
	},
	{
		type: 'input',
		name: 'address',
		message: 'Address (ADDR)',
		validate: function(value) {
			var valid = !isNaN(parseFloat(value)) && 0 < value && value < 256;
			return valid || "Please enter a number between 1 and 255";
		},
		filter: Number
	},
	{
		type: 'list',
		name: 'bandwidth',
		message: 'Bandwidth (BW_X)',
		choices: [ '125 KHz', '250 KHz', '500KHz' ],
		filter: function(value) { return parseInt(value); }
	},
	{
		type: 'list',
		name: 'codingRate',
		message: 'Coding Rate (CR_X)',
		choices: [ '5', '6', '7', '8' ],
		filter: Number
	},
	{
		type: 'list',
		name: 'spreadingFactor',
		message: 'Spreading factor (SF_X)',
		choices: [ '6', '7', '8', '9', '10', '11', '12' ],
		filter: Number
	}
];

serialPort.on('open',function() {
	serialPort.on('data', function(data) {
		if (data.equals(readyBuffer)) {
			// launch conf tool
			inquirer.prompt(questions, function(answers) {
				// construct configuration string from answers
				var conf = 'SET#';
				conf += 'FREC:CH_' + answers.channel + '_' + answers.freq + ';';
				conf += 'ADDR:' + answers.address + ';';
				conf += 'BW:BW_' + answers.bandwidth + ';';
				conf += 'CR:CR_' + answers.codingRate + ';';
				conf += 'SF:SF_' + answers.spreadingFactor;

				// calculate crc
				var crc16Modbus = crc.crc16modbus(conf).toString(16).toUpperCase();

				// construct serial command
				var cmd = [1];
				cmd = cmd.concat(conf.split('').map(function(c) { return c.charCodeAt(); }));
				cmd = cmd.concat([13, 10])
				cmd = cmd.concat(crc16Modbus.split('').map(function(c) { return c.charCodeAt(); }))
				cmd = cmd.concat([4]);

				console.log('Configuration : ' + conf);
				console.log('CRC : ' + crc16Modbus);
				serialPort.write(cmd);
			});
		}
		else {
			data.forEach(function(current) {
				if (current === 0x04) {
					var msg = chunks.split('\n');
					console.log('Answer : ' + msg[0]);
					process.exit();
				}
				else if (current !== 0x01) {
					chunks += String.fromCharCode(current);
				}
			});
		}
	});


});

