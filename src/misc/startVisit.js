var request = require('request'),
	conf = require('../conf/conf.js');

var postUrl = conf.APP_URL;
if (conf.APP_URL === 'http://localhost') postUrl += ':' + conf.PORT;
postUrl += conf.API_PREFIX + '/visit';

console.log(postUrl);

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
