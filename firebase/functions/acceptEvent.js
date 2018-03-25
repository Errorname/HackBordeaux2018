const functions = require('firebase-functions')

const database = require('./database').database

exports.default = functions.https.onRequest((request, response) => {
  const { idEvent, name } = request.body.split('&').reduce((acc, p) => {
    const [name, value] = p.split('=')
    acc[name] = value
    return acc
  }, {})

  return database
    .ref('/events/' + idEvent + '/participants/' + name)
    .set(true)
    .then(() => {
      return response.send('OK')
    })
})
