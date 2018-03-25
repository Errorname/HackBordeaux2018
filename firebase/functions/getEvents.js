const functions = require('firebase-functions')
const _ = require('lodash')

const database = require('./database').database

exports.default = functions.https.onRequest((request, response) => {
  const createdAt = Number.parseInt(request.body.split('=').pop())

  return database
    .ref('events')
    .orderByChild('timestamp')
    .once('value')
    .then(snapshot => {
      let events = snapshot.val()

      if (!events) events = {}

      events = Object.keys(events).map(id => {
        let event = events[id]
        event['id'] = id
        return event
      })

      events = _.orderBy(
        _.filter(events, e => e.createdAt > createdAt),
        'timestamp',
        'asc'
      )

      return response.json(events)
    })
})
