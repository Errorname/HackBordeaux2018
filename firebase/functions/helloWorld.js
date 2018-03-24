const functions = require('firebase-functions')

exports.default = functions.https.onRequest((request, response) => {
  return response.json(request.body)
})
