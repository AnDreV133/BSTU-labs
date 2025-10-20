<template>
    <div class="file-card-bg" @click="handleClick">
        <div class="file-card-content">
            <h4 class="font">{{ fileName }}</h4>
            <h6 class="tint-font">{{ fileSize }}</h6>
        </div>
        <div class="file-card-img-holder">
            <img :src="getFileIcon" :alt="fileExtension" class="file-icon">
        </div>
    </div>
</template>

<script>
export default {
    name: "FileCard",
    props: {
        fileName: {
            type: String,
            require: true
        },
        fileSize: {
            type: String,
            require: true
        },
        fileExtension: {
            type: String,
            require: true
        }
    },
    computed: {
        getFileIcon() {
            try {
                return new URL(`/src/assets/extensions/${this.fileExtension}.svg`, import.meta.url).href;
            } catch (e) {
                return new URL(`/src/assets/extensions/file.svg`, import.meta.url).href;
            }
        }
    },
    methods: {
        handleClick() {
            alert(
                "click on file: "
                + this.fileName
                + " (" + this.fileExtension + ") "
                + this.fileSize
            );
        }
    }
}
</script>

<style scoped>
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
    width: 100%;
    justify-content: space-between;
    align-items: center;
    border-style: solid;
    border-width: 2px;
    border-color: gray;
    border-radius: 10px;
    padding: 10px 20px;
    margin: 0 10px 10px;
    cursor: pointer;
    transition: all 0.3s ease;
}

.file-card-bg:hover {
    border-color: #42b883;
    background-color: #f9f9f9;
    transform: translateY(-2px);
    box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
}

.file-card-content {
    flex: 1;
}

.file-card-content h4 {
    margin: 0 0 5px 0;
    font-size: 16px;
}

.file-card-content h6 {
    margin: 0;
    font-size: 12px;
}

.file-card-img-holder {
    display: flex;
    align-items: center;
    justify-content: center;
    margin-left: 15px;
}

.file-icon {
    width: 32px;
    height: 32px;
    object-fit: contain;
}
</style>