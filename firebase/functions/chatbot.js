const functions = require('firebase-functions')
const moment = require('moment')

const database = require('./database').database

exports.default = functions.https.onRequest((request, response) => {
  const params = request.body.result.contexts.filter(
    c => c.name === 'actions_capability_screen_output'
  )[0].parameters

  // Time to timestamp
  let time = String(params['date-time'])

  if (time.indexOf('/')) {
    time = time.split('/')[0].replace('Z', '')
  }

  if (time.indexOf('T') >= 0) {
    time = moment(time, 'YYYY-MM-DD[T]HH:mm:ss').unix()
  } else if (time.indexOf(':') >= 0) {
    time = moment(time, 'HH:mm:ss').unix()
  }

  // Check if comment = no
  let comment = params['comment']

  if (comment && comment.startsWith('no')) {
    comment = ''
  }

  if (params['event_type']) {
    // Create an event

    return database
      .ref('events')
      .push({
        type: params['event_type'],
        owner: 'pierre',
        location: params['location'],
        comment: comment,
        timestamp: time,
        createdAt: Math.floor(Date.now() / 1000)
      })
      .then(snapshot =>
        response.json({
          speech: "Okay, I'm sending everyone a notification!",
          displayText: "Okay, I'm sending everyone a notification!",
          messages: {},
          data: {},
          contextOut: [],
          source: '',
          followupEvent: {}
        })
      )
  } else if (params['mood']) {
    // Update a mood
    return database
      .ref('users/pierre')
      .update({
        mood: { type: params['mood'], comment: params['mood_comment'] }
      })
      .then(snapshot => response.json(snapshot))
  }

  return response.send('What ?')
})
