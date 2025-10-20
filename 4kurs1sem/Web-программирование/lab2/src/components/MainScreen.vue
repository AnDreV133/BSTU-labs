<template>
    <div class="cloud-container">
        <header class="cloud-header">
            <div class="header-left">
                <button class="logout-btn" @click="handleLogout">
                    <img src="@/assets/img/im_logout.svg" alt="Выход">
                </button>
            </div>
            <div class="header-right clickable-container">
                <h4 class="font">Пользователь {{ userName }}</h4>
                <h6 class="tint-font">Уровень: {{ userLevel }}</h6>
            </div>
        </header>

        <main class="cloud-main">
            <FileCardItem fileName="file-sample.txt" fileSize="300 KB" fileExtension="txt" />

            <FileCardItem fileName="photo-sample.png" fileSize="4500 KB" fileExtension="png" />

            <FileCardItem v-for="file in files" :key="file.id" :fileName="file.name" :fileSize="file.size"
                :fileExtension="file.extension" />
        </main>

        <div class="fab-container">
            <button class="add-fab-bg" @click="handleAddFile">
                <img class="add-fab-fg" src="@/assets/img/im_plus.svg" alt="Добавить файл">
            </button>
        </div>
    </div>
</template>

<script>
import FileCardItem from '@/components/items/FileCardItem.vue'

export default {
    name: "UserCloud",
    components: {
        FileCardItem
    },
    data() {
        return {
            userName: "Иван Иванов",
            userLevel: "Стандартный",
            files: []
        }
    },
    methods: {
        handleLogout() {
            if (confirm("Вы уверены, что хотите выйти?")) {
                this.$router.push('/signin')
                .then(() => console.log('Navigation successful'))
          .catch(err => console.error('Navigation error:', err));
            }
        },
        handleAddFile() {
            this.files.push({
                id: this.files.length,
                name: "new-file.txt",
                size: "10 MB",
                extension: "txt"
            })
        }
    }
}
</script>

<style scoped>
.cloud-container {
    min-height: 100vh;
    background-color: white;
    position: relative;
}

.cloud-header {
    justify-content: space-between;
    align-items: center;
    padding: 1rem 2rem;
    border-bottom: 1px solid #e0e0e0;
    display: flex;
    flex-direction: row;
}

.header-left {
    display: flex;
    flex-shrink: 0;
    align-items: center;
}

.header-right {
    flex-shrink: 0;
    text-align: right;
    cursor: pointer;
    padding: 0.5rem;
    border-radius: 8px;
    transition: background-color 0.3s ease;
}

.header-right:hover {
    background-color: #f5f5f5;
}

.cloud-main {
    display: flex;
    flex-direction: column;
    padding: 2rem;
    max-width: 800px;
    margin: 0 auto;
}

.fab-container {
    position: fixed;
    bottom: 20px;
    right: 20px;
    z-index: 1000;
}

.font {
    color: black;
}

.bold-font {
    color: black;
    font-weight: bold;
}

.tint-font {
    color: black;
    opacity: 0.7;
}

.main-bg {
    color: white;
}

.file-card-bg {
    color: darkgrey;
    display: flex;
    justify-content: space-between;
    border-style: solid;
    border-width: 2px;
    border-color: gray;
    border-radius: 10px;
    padding: 10px 20px;
    margin: 0 10px 10px;
    cursor: pointer;
}

.add-fab-bg {
    width: 50px;
    height: 50px;
    background-color: black;
    border-radius: 20px;
    position: fixed;
    bottom: 20px;
    right: 20px;
    cursor: pointer;
    border: none;
    display: flex;
    align-items: center;
    justify-content: center;
    transition: transform 0.3s ease, background-color 0.3s ease;
}

.add-fab-bg:hover {
    transform: scale(1.1);
    background-color: #333;
}

.add-fab-fg {
    filter: invert(100%) sepia(9%) saturate(481%) hue-rotate(249deg) brightness(121%) contrast(100%);
    width: 24px;
    height: 24px;
}

.clickable-container {
    cursor: pointer;
}

.logout-btn {
    width: 40px;
    height: 40px;
    background: none;
    border: none;
    cursor: pointer;
    padding: 8px;
    border-radius: 50%;
    transition: background-color 0.3s ease;
}

.logout-btn:hover {
    background-color: #f0f0f0;
}

.logout-btn img {
    width: 100%;
    height: 100%;
    object-fit: contain;
}

@media (max-width: 768px) {
    .cloud-header {
        padding: 1rem;
        gap: 1rem;
    }

    .cloud-main {
        padding: 1rem;
    }
}
</style>