const functions = require('firebase-functions')

exports.helloWorld = require('./helloWorld.js').default
exports.createEvent = require('./createEvent').default
exports.acceptEvent = require('./acceptEvent').default

// Create and Deploy Your First Cloud Functions
// https://firebase.google.com/docs/functions/write-firebase-functions
