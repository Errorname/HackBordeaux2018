const functions = require('firebase-functions')

const database = require('./database').database

exports.default = functions.https.onRequest((request, response) => {
  return database
    .ref('/events')
    .push({
      name: 'Test',
      owner: 'thibaud',
      comment: 'This is a test',
      timestamp: Date.now()
    })
    .then(snapshot => {
      return response.json(snapshot)
    })
})
