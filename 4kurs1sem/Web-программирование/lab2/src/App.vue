<script setup>
import Login from './components/Login.vue';
</script>

<template>
  <header>
    
  </header>

  <main>
    <Login 
      title="Авторизация"
      :onSubmit=submit 
      under-text="Еще не зарегистрированы?"
      under-href="kk"
    />
  </main>
</template>

<script>
  export default {
    components: { Login },
    methods: {
      submit(login, password) {
        alert("hui " + login + " " + password)
      }
    }
  }
</script>

<style scoped>
</style>
