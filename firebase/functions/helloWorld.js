const functions = require('firebase-functions')

const database = require('./database').database

exports.default = functions.https.onRequest((request, response) => {
  return database
    .ref('logs')
    .push(JSON.stringify(request.body))
    .then(snapshot => {
      return response.json(snapshot)
    })
})
