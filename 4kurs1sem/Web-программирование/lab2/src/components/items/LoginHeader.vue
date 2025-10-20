<template>
    <div class="main">
        <h2 class="title">{{ title }}</h2>
    </div>
</template>

<script>
export default {
    name: "LoginHeader",
    props: {
        title: {
            type: String,
            require: true
        }
    }
}
</script>

<style scoped>
.main {
    background-color: lightgrey;
    display: flex;
    justify-content: space-between;
    align-items: center;
    padding: 10px;
}

.title {
    color: black;
}
</style>