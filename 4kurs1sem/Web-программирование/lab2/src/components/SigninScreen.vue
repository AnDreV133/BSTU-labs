<script setup>
import LoginHeader from './components/items/LoginHeader';
import LoginForm from './components/items/LoginForm';
</script>

<template>
    <header>
        <LoginHeader title="Авторизация" />
    </header>
    
    <main>
        <LoginForm :onSubmit=submit under-text="Еще не зарегистрированы?" under-href="kk" />
    </main>
</template>

<script>
export default {
    components: { LoginHeader, LoginForm },
    methods: {
        submit(login, password) {
            alert("test " + login + " " + password)
        }
    }
}
</script>

<style scoped></style>
