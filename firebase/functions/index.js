const functions = require('firebase-functions')

exports.helloWorld = require('./helloWorld').default
exports.getEvents = require('./getEvents').default
exports.createEvent = require('./createEvent').default
exports.acceptEvent = require('./acceptEvent').default
exports.updateMood = require('./updateMood').default
exports.chatbot = require('./chatbot').default

// Create and Deploy Your First Cloud Functions
// https://firebase.google.com/docs/functions/write-firebase-functions
