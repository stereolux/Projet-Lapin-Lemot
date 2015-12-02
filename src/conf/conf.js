'use strict';
var Conf = {};
Conf.APP_URL = process.env.APP_URL || 'http://localhost';
Conf.MONGO_URL = process.env.MONGOLAB_URI || 'mongodb://localhost/lapinlemot';
Conf.REDIS_URL = process.env.REDIS_URL || 'redis://:@localhost:6379';
Conf.PORT = (process.env.PORT === '') ? '' : 3000;
Conf.API_VERSION = 1;
Conf.API_PREFIX = '/api/' + Conf.API_VERSION;

module.exports = Conf;
