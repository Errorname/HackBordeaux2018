const functions = require('firebase-functions')

const database = require('./database').database

exports.default = functions.https.onRequest((request, response) => {
  const userId = request.query.user

  const mood = {
    type: request.query.mood_type,
    comment: request.query.mood_comment
  }

  return database
    .ref(`users/${userId}`)
    .update({
      mood: mood
    })
    .then(snapshot => {
      return response.send('Mood updated')
    })
})
