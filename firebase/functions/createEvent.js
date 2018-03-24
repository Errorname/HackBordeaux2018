const functions = require('firebase-functions')

const admin = require('firebase-admin')
admin.initializeApp(functions.config().firebase)

exports.default = functions.https.onRequest((request, response) => {
  return admin
    .database()
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
