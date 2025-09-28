<template>
    <div>
        <p class="title">{{ title }}</p>
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

<style>
header {
    background-color: lightgrey;
    display: flex;
    justify-content: space-between;
    height: 8vh;
    padding: 10px;
    margin-bottom: 10px;
}

.title {
    font-size: 1.5em;
    color: black;
    margin-bottom: 1em;
}
</style>