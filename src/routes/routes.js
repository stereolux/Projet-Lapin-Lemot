'use strict';
var uuid = require('uuid');
/**
 * Routes of the API
 * @class Routes
 * @constructor
 * @module routes
 * @param Waypoint the Waypoint model to query
 * @param client the redis client to get current visit Id from tag Id
 */
var Routes = function(Waypoint, client) {

	var _getAllVisits = function (req, res) {
		// query waypoints and aggregate them by visits
		var aggregate = Waypoint.aggregate();
		aggregate.group({
			_id: '$visitId',
			waypoints: {
				$push: {
					sensorId: '$sensorId',
					date: '$date'
				}
			}
		});
		aggregate.project({
			_id: 0,
			visitId: '$_id',
			waypoints: 1
		});
		// query mongodb
		aggregate.exec(function (err, visits) {
			if (err) throw err;
			res.status(200).send(visits);
		});
	};

	var _getVisit = function(req, res) {
		// get current visitId from the tag id from redis
		client.get(req.params.tagId, function (err, value) {
			if (err) throw err;
			// filter results from the visit id
			var filter = {
				visitId: value
			};
			// construct the query
			var query = Waypoint.find(filter);
			// sort by date, oldest first
			query.sort('date');
			// query mongodb
			query.exec(function (err, waypoints) {
				if (err) throw err;
				res.status(200).send(waypoints);
			});
		});
	};

	var _createVisit = function(req, res) {
		var visitId = uuid.v1();
		client.set(req.body.tagId, visitId, function() {
			res.status(201).send({ uuid: visitId });
		});
	};

	return {
		getAllVisits: _getAllVisits,
		getVisit: _getVisit,
		createVisit: _createVisit
	};
};

module.exports = Routes;
