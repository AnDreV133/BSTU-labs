<script setup>
import LoginHeader from './items/LoginHeader.vue';
import LoginForm from './items/LoginForm.vue';
</script>

<template>
    <header>
        <LoginHeader title="Регистрация" />
    </header>

    <main>
        <LoginForm class="form" :onSubmitFunc="submit" under-text="Есть аккаунт?" under-href="/signin" />
    </main>
</template>

<script>
export default {
    components: { LoginHeader, LoginForm },
    methods: {
        submit(login, password) {
            alert("signup with cred: " + login + " " + password)
        }
    }
}
</script>

<style scoped>
.form {
    margin-top: 80px;
}
</style>
