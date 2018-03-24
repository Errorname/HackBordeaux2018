const functions = require('firebase-functions')

const database = require('./database').database

exports.default = functions.https.onRequest((request, response) => {
  return database
    .ref('/events')
    .push({
      type: request.query.type,
      owner: request.query.user,
      location: request.query.loc,
      comment: request.query.comm || "",
      timestamp: request.query.time,
      createdAt: Date.now()
    })
    .then(snapshot => {
      return response.json(snapshot)
    })
})
