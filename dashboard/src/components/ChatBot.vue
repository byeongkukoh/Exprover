<template>
    <div class="chatbot-container">
        <div class="chat-history">
            <div v-for="(msg, idx) in messages" :key="idx" :class="msg.role === 'user' ? 'user-msg' : 'bot-msg'">
                <div class="msg-box">
                    {{ msg.content }}
                </div>
                <!-- <span class="msg-role">{{ msg.role === 'user' ? '나' : '챗봇' }}:</span>
                <span class="msg-content">{{ msg.content }}</span> -->
            </div>
        </div>

        <div class="chat-input">
            <input v-model="userInput" @keyup.enter="sendMessage" placeholder="메세지를 입력하세요." />
            <button @click="sendMessage">보내기</button>
        </div>
    </div>
</template>

<script setup>
import { ref } from "vue";
import axios from "axios";

const messages = ref([]);
const userInput = ref("");

const API_URL = "http://54.180.119.169:5000/chat";      // EC2 IP로 변경

async function sendMessage() {
    const content = userInput.value.trim();

    if (!content) return;

    messages.value.push({ role: "user", content });
    userInput.value = "";

    try {
        const res = await axios.post(API_URL, { message: content });
        const answer = res.data.answer || "(응답 없음)";
        messages.value.push({ role: "bot", content: answer });
    } catch (err) {
        messages.value.push({
            role: "bot",
            content: "서버에 연결할 수 없습니다."
        });
    }
}
</script>

<style scoped>
.chatbot-container {
    width: 500px;
    margin: 50px auto;
    border: 1px solid #f8f9fa;
    border-radius: 12px;
    background-color: #f8f9fa;
    display: flex;
    flex-direction: column;
    height: 600px;
}

.chat-history {
    flex: 1;
    overflow-y: auto;
    padding: 15px;
}

.user-msg,
.bot-msg {
    display: flex;
    text-align: left;
    margin: 5px 0;
}

.user-msg {
    justify-content: flex-end;
}

.user-msg .msg-box {
    border: 1px solid #eeff0070;
    background-color: #eeff0070;
}

.bot-msg {
    justify-content: flex-start;

}

.bot-msg .msg-box {
    border: 1px solid #cccccc8e;
    background-color: #cccccc8e;
}

.msg-box {
    display: inline-block;
    padding: 10px 20px;
    max-width: 60%;
    word-break: break-word;
    font-size: 1rem;
    border-radius: 12px;
}

.msg-role {
    font-weight: bold;
    margin-right: 4px;
}

.chat-input {
    display: flex;
    border-top: 1px solid #eee;
    padding: 10px;
    background-color: #fff;
}

.chat-input input {
    flex: 1;
    padding: 8px;
    border-radius: 6px;
    border: 1px solid #ccc;
}

.chat-input button {
    margin-left: 8px;
    padding: 8px 16px;
    border: none;
    border-radius: 6px;
    background: #2979ff;
    color: #fff;
    cursor: pointer;
}
</style>