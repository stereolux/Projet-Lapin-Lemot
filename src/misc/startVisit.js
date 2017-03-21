'use strict';
var request = require('request'),
	serialport = require('serialport'),
	SerialPort = serialport.SerialPort,
	notifier = require('node-notifier'),
	path = require('path'),
	conf = require('../conf/conf.js'),
	tags = require('../conf/tags.js');


var notification = {
	title: 'Data Rabbit',
	icon: path.join(__dirname, 'assets', 'logo.jpg')
};

var serialPort = new SerialPort('/dev/ttyUSB1', {
	parser: serialport.parsers.readline("\n")
});

serialPort.on('open',function() {
	serialPort.on('data', function(data) {
		console.log(data);
		if (!data.startsWith('Error')) {
			var tagId = tags[data.trim()];
			if (tagId) {
				startVisit(tagId, function(err, status, body) {
					console.log(err);
					if (!err && status === 201) {
						notification.message = 'La visite peut commencer';
						notifier.notify(notification);
					}
					else {
						notification.message = 'Une erreur est survenue, veuillez rescanner le badge';
						notifier.notify(notification);
					}
				});
			}
			else {
				notification.message = 'Le badge n\'est pas reconnu. Est-il bien configuré?';
				notifier.notify(notification);
			}
		}
	});
});

var startVisit = function(tagId, callback) {
	var postUrl = conf.APP_URL;
	if (conf.APP_URL === 'http://localhost') postUrl += ':' + conf.PORT;
	postUrl += conf.API_PREFIX + '/visit';

	request.post(
		{ url: postUrl, form: { tagId: tagId } },
		function(err, httpResponse, body) {
			var status = httpResponse ? httpResponse.statusCode : null;
			callback(err, status, body);
		}
	);
};
