const functions = require('firebase-functions')

const database = require('./database').database

exports.default = functions.https.onRequest((request, response) => {
  const {idUser, idEvent} = request.query
  const updates = {}
  updates[idUser] = true
  return database
    .ref('/events/'+idEvent+'/participants/')
    .update(updates)
    .then(() => {
      return response.send("OK")
    })
})
