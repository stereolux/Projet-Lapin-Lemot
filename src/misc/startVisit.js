var request = require('request'),
	conf = require('../conf/conf.js');

var postUrl = conf.APP_URL + ':' + conf.PORT + conf.API_PREFIX + '/visit'

// TODO: link with arduino and NFC
request.post(
	{
		url: postUrl,
		form: { tagId: 1 }
	},
	function(err, httpResponse, body) {
		console.log(err);
		console.log(httpResponse.statusCode);
		console.log(body);
		process.exit();
	}
);
