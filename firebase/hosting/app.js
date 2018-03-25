var firebaseApp = firebase.initializeApp({
  apiKey: 'AIzaSyDGAFenaCuernqTyrkgCEpN5iGP-yQ1XLE',
  authDomain: 'hackbordeaux2018.firebaseapp.com',
  databaseURL: 'https://hackbordeaux2018.firebaseio.com',
  projectId: 'hackbordeaux2018',
  storageBucket: 'hackbordeaux2018.appspot.com',
  messagingSenderId: '496499208213'
})
var db = firebaseApp.database()

Vue.component('my-event', {
  template: '#event',
  props: ['event'],
  computed: {
    moment() {
      return window.moment
    }
  },
  mounted() {
    this.$el.classList.add('flash')
    setTimeout(() => this.$el.classList.remove('flash'), 5000)
  },
  updated() {
    this.$el.classList.add('flash')
    setTimeout(() => this.$el.classList.remove('flash'), 5000)
  },
  methods: {
    participants(event) {
      if (!event.participants) return 1

      return Object.keys(event.participants).length + 1
    }
  }
})

Vue.component('my-user', {
  template: '#user',
  props: ['user'],
  mounted() {
    this.$el.classList.add('flash')
    setTimeout(() => this.$el.classList.remove('flash'), 5000)
  },
  updated() {
    this.$el.classList.add('flash')
    setTimeout(() => this.$el.classList.remove('flash'), 5000)
  }
})

const app = new Vue({
  el: '#app',
  firebase: {
    users: db.ref('users'),
    events: db.ref('events')
  },
  computed: {
    nextEvents() {
      const events = Object.keys(this.events).map(id => {
        const event = this.events[id]
        event['id'] = id
        return event
      })
      return _.orderBy(events, 'timestamp', 'asc')
    },
    popularEvents() {
      const events = Object.keys(this.events).map(id => {
        const event = this.events[id]
        event['id'] = id
        return event
      })
      const ret = _.take(
        _.orderBy(
          events,
          o => (o.participants ? Object.keys(o.participants).length : 0),
          'desc'
        ),
        3
      )

      return ret
    }
  }
})
