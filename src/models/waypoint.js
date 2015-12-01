'use strict';
var mongoose = require('mongoose');

/**
 * Waypoint schema, created when the NFC sensor is activated
 * Contains the following infos :
 * - sensorId : the id of the NFC sensor (e.g. id of the rabbit)
 * - visitId : the id of the tag that activated the sensor
 * - date : date of the event
 */
var waypointSchema = mongoose.Schema({
	sensorId: { type: String, required: true },
	visitId: { type: String, required: true },
	date: { type: Date, required: true }
});

/**
 * Waypoint model
 * @class Waypoint
 * @module models
 */
var Waypoint = mongoose.model('Waypoint', waypointSchema);

module.exports = Waypoint;
