<template>
    <div>
        <header>
            <div>
                <h3>{{ title }}</h3>
            </div>
        </header>
        <main>
            <div class="auth-container">
                <form @submit.prevent="onSubmit">
                    <div class="input-container">
                        <input type="text" id="login" v-model="login" required />
                        <label for="login">Логин</label>
                    </div>
                    <div class="input-container">
                        <input type="password" id="password" v-model="password" required />
                        <label for="password">Пароль</label>
                    </div>
                    <div class="login-button">
                        <button type="submit">Войти</button>
                    </div>
                </form>
                <div v-show="error">{{ error }}</div>
                <a href="underHref">{{ underText }}</a>
            </div>
        </main>
    </div>
</template>

<script>
export default {
    name: "Login",
    data() {
        return {
            login: "",
            password: "",
            error: ""
        };
    },
    props: {
        title: {
            type: String,
            require: true
        },
        onSubmitFunc: {
            type: Function,
            require: true
        },
        underHref: {
            type: String,
            require: true
        },
        underText: {
            type: String,
            require: true
        }
    },
    methods: {
        validateForm() {
            if (!this.login) {
                error = "Логин обязателен.";
            }
            else if (!this.password) {
                error = "Пароль обязателен.";
            } else {
                error = ""
            }
        },
        onSubmit() {
            this.validateForm();

            if (this.error && this.error.length) return;

            this.onSubmitFunc(this.login, this.password)

            // alert(`Логин: ${this.login}\nПароль: ${this.password}`);
        }
    }
};
</script>

<style scoped>
@import '../assets/main.css';
.auth-container {
    display: flex;
    height: 100%;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    top: 50%;
    left: 50%;
}

.input-container {
    position: relative;
    margin-bottom: 20px;
}

.input-container input {
    padding: 15px 10px;
    border: 1px solid #ccc;
    border-radius: 4px;
    font-size: 16px;
}

.input-container label {
    position: absolute;
    top: 0;
    left: 5px;
    transform: translateY(-50%);
    padding: 0 4px;
    color: black;
    background-color: white;
}

.login-button {
    padding: 0 12px;
    margin-bottom: 30px;
}

.login-button button {
    height: 30px;
    background-color: black;
    border-radius: 12px;
    color: white;
    border: none;
    cursor: pointer;
}
</style>