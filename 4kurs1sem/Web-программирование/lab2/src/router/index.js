import { createRouter, createMemoryHistory } from 'vue-router'
import SigninScreen from '../components/SigninScreen.vue'
import SignupScreen from '../components/SignupScreen.vue'
import MainScreen from '../components/MainScreen.vue'

const routes = [
    {
        path: '/',
        redirect: '/signin'
    },
    {
        path: '/signin',
        component: SigninScreen
    },
    {
        path: '/signup',
        component: SignupScreen
    },
    {
        path: '/main',
        component: MainScreen
    },
]

const router = createRouter({
    history: createMemoryHistory(),
    routes
})

export default router