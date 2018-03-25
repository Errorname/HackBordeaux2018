var firebaseApp = firebase.initializeApp({
  apiKey: 'AIzaSyDGAFenaCuernqTyrkgCEpN5iGP-yQ1XLE',
  authDomain: 'hackbordeaux2018.firebaseapp.com',
  databaseURL: 'https://hackbordeaux2018.firebaseio.com',
  projectId: 'hackbordeaux2018',
  storageBucket: 'hackbordeaux2018.appspot.com',
  messagingSenderId: '496499208213'
})
var db = firebaseApp.database()

const app = new Vue({
  el: '#app',
  firebase: {
    users: db.ref('users'),
    events: db.ref('events')
  },
  computed: {
    moment() {
      return window.moment
    },
    nextEvents() {
      return _.orderBy(this.events, 'timestamp', 'asc')
    },
    popularEvents() {
      return _.take(
        _.orderBy(
          this.events,
          o => (o.participants ? o.participants.length : 0),
          'desc'
        ),
        3
      )
    }
  },
  methods: {
    participants(event) {
      if (!event.participants) return 1

      return Object.keys(event.participants).length + 1
    },
    user(name) {
      return this.users
        ? this.users.filter(u => u.name.toLowerCase() === name)[0]
        : {}
    }
  }
})
