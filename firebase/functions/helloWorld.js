const functions = require('firebase-functions')

exports.default = functions.https.onRequest((request, response) => {
  response.send('Hello from HackBordeaux 2018!')
})
